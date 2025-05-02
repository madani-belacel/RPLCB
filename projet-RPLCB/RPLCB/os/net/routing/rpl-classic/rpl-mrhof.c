/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         The Minimum Rank with Hysteresis Objective Function (MRHOF)
 *
 * \author Joakim Eriksson <joakime@sics.se>, Nicolas Tsiftes <nvt@sics.se>
 */

#include "contiki.h"
#include "net/routing/rpl-classic/rpl.h"
#include "net/routing/rpl-classic/rpl-private.h"
#include "net/routing/rpl-classic/rpl-mrhof.h"
#include "net/routing/rpl-classic/rpl-callback.h"
#include "net/link-stats.h"
#include "sys/log.h"

#define DEBUG DEBUG_NONE
#include "net/ipv6/uip-debug.h"

/* Log configuration */
#define LOG_MODULE "RPL-MRHOF"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Constants for MRHOF */
#define MAX_LINK_METRIC 256
#define MIN_HOP_RANK_INCREASE 256
#define PARENT_SWITCH_THRESHOLD 128

/* Forward declarations */
static void reset(rpl_dag_t *dag);
#if RPL_WITH_DAO_ACK
static void dao_ack_callback(rpl_parent_t *parent, int status);
#endif
static uint16_t parent_link_metric(rpl_parent_t *p);
static int parent_has_usable_link(rpl_parent_t *p);
static uint16_t parent_path_cost(rpl_parent_t *p);
static rpl_rank_t rank_via_parent(rpl_parent_t *p);
static rpl_parent_t *best_parent(rpl_parent_t *p1, rpl_parent_t *p2);
static rpl_dag_t *best_dag(rpl_dag_t *d1, rpl_dag_t *d2);
static void update_metric_container(rpl_instance_t *instance);
static void neighbor_link_callback(rpl_parent_t *parent, int known, int etx);
static uint16_t calculate_rank(rpl_parent_t *parent, uint16_t base_rank);

/* Function implementations */
static void reset(rpl_dag_t *dag)
{
  dag->rank = RPL_INFINITE_RANK;
}

#if RPL_WITH_DAO_ACK
static void dao_ack_callback(rpl_parent_t *parent, int status)
{
  if(status == RPL_DAO_ACK_UNABLE_TO_ADD_ROUTE_AT_ROOT) {
    rpl_remove_parent(parent);
  }
}
#endif

static uint16_t parent_link_metric(rpl_parent_t *p)
{
  const struct link_stats *stats = rpl_get_parent_link_stats(p);
  if(stats == NULL) {
    return MAX_LINK_METRIC;
  }
  return stats->etx;
}

static int parent_has_usable_link(rpl_parent_t *p)
{
  const struct link_stats *stats = rpl_get_parent_link_stats(p);
  if(stats == NULL) {
    return 0;
  }
  return stats->etx < MAX_LINK_METRIC;
}

static uint16_t parent_path_cost(rpl_parent_t *p)
{
  return p->rank;
}

static rpl_rank_t rank_via_parent(rpl_parent_t *p)
{
  if(p == NULL) {
    return RPL_INFINITE_RANK;
  }
  return p->rank;
}

static rpl_parent_t *best_parent(rpl_parent_t *p1, rpl_parent_t *p2)
{
  if(p1 == NULL) {
    return p2;
  }
  if(p2 == NULL) {
    return p1;
  }
  if(p1->rank < p2->rank) {
    return p1;
  }
  if(p2->rank < p1->rank) {
    return p2;
  }
  /* If ranks are equal, use link metrics */
  uint16_t m1 = parent_link_metric(p1);
  uint16_t m2 = parent_link_metric(p2);
  if(m1 < m2) {
    return p1;
  }
  if(m2 < m1) {
    return p2;
  }
  /* If metrics are equal, maintain stability by returning p1 */
  return p1;
}

static rpl_dag_t *best_dag(rpl_dag_t *d1, rpl_dag_t *d2)
{
  if(d1 == NULL) {
    return d2;
  }
  if(d2 == NULL) {
    return d1;
  }
  if(d1->rank < d2->rank) {
    return d1;
  }
  if(d2->rank < d1->rank) {
    return d2;
  }
  /* If ranks are equal, maintain stability by returning d1 */
  return d1;
}

static void update_metric_container(rpl_instance_t *instance)
{
  if(instance == NULL || instance->current_dag == NULL) {
    return;
  }
  rpl_metric_container_t *mc = &instance->mc;
  mc->type = RPL_DAG_MC_ETX;
  mc->flags = (1 << RPL_DAG_MC_FLAG_P) | (1 << RPL_DAG_MC_FLAG_C);
  mc->aggr = RPL_DAG_MC_AGGR_ADDITIVE;
  mc->prec = 0;
  mc->length = 2;
  mc->obj.etx = parent_link_metric(instance->current_dag->preferred_parent);
}

static void neighbor_link_callback(rpl_parent_t *parent, int known, int etx)
{
  if(parent == NULL) {
    return;
  }
  if(!known) {
    rpl_remove_parent(parent);
  }
}

static uint16_t calculate_rank(rpl_parent_t *parent, uint16_t base_rank)
{
  if(parent == NULL) {
    return RPL_INFINITE_RANK;
  }
  if(base_rank == RPL_INFINITE_RANK) {
    return RPL_INFINITE_RANK;
  }
  uint16_t rank = base_rank + parent_link_metric(parent);
  if(rank < RPL_MIN_HOPRANKINC) {
    rank = RPL_MIN_HOPRANKINC;
  }
  return rank;
}

/* Initialize the MRHOF objective function */
const struct rpl_of rpl_mrhof = {
  .reset = reset,
  .neighbor_link_callback = neighbor_link_callback,
  .best_parent = best_parent,
  .best_dag = best_dag,
  .calculate_rank = calculate_rank,
  .update_metric_container = update_metric_container,
  .ocp = RPL_OCP_MRHOF
};
