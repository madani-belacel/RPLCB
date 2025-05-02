#include "contiki.h"
#include "net/routing/routing.h"
#include "net/routing/rpl-classic/rpl.h"
#include "net/routing/rpl-classic/rpl-private.h"
#include "net/routing/rpl-classic/rpl-dag.h"
#include "net/routing/rpl-classic/rpl-dag-root.h"
#include "net/routing/rpl-classic/rpl-neighbor.h"
#include "net/routing/rpl-classic/rpl-ext-header.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "sys/log.h"
#include "rpl-classic-types.h"
#include "rpl-classic-funcs.h"

#define LOG_MODULE "RPL-MRHOF"
#define LOG_LEVEL LOG_LEVEL_INFO

/* RFC6719 and RFC6551 */
#define MAX_LINK_METRIC 512
#define MAX_PATH_COST   32768

/* Represents 2^n */
#define ETX_DIVISOR     128
#define ETX_FACTOR      128

#define PARENT_SWITCH_THRESHOLD 96

static rpl_of_t rpl_mrhof = {
  .reset = reset,
  .parent_state_callback = parent_state_callback,
  .best_parent = best_parent,
  .update_metric_container = update_metric_container,
  .calculate_rank = calculate_rank
};

/*---------------------------------------------------------------------------*/
static void
reset(rpl_instance_t *instance)
{
  LOG_INFO("Reset MRHOF\n");
}

/*---------------------------------------------------------------------------*/
static uint16_t
parent_link_metric(rpl_parent_t *p)
{
  return rpl_classic_get_parent_link_metric(p);
}

/*---------------------------------------------------------------------------*/
static int
parent_has_usable_link(rpl_parent_t *p)
{
  return rpl_classic_parent_has_usable_link(p);
}

/*---------------------------------------------------------------------------*/
static uint16_t
parent_path_cost(rpl_parent_t *p)
{
  uint16_t base;
  
  if(p == NULL) {
    return MAX_PATH_COST;
  }
  
  base = parent_link_metric(p);
  
  /* path cost upper bound: 32768 */
  if(base > MAX_PATH_COST) {
    return MAX_PATH_COST;
  }
  
  return base;
}

/*---------------------------------------------------------------------------*/
static uint16_t
rank_via_parent(rpl_parent_t *p)
{
  uint16_t min_hoprankinc;
  uint16_t path_cost;
  
  if(p == NULL) {
    return RPL_INFINITE_RANK;
  }
  
  min_hoprankinc = p->instance->min_hoprankinc;
  path_cost = parent_path_cost(p);
  
  /* Rank lower-bound: parent rank + min_hoprankinc */
  return MAX(MIN_LINK_METRIC, p->rank + min_hoprankinc);
}

/*---------------------------------------------------------------------------*/
static rpl_parent_t *
best_parent(rpl_parent_t *p1, rpl_parent_t *p2)
{
  uint16_t p1_cost;
  uint16_t p2_cost;
  int p1_is_acceptable;
  int p2_is_acceptable;
  
  p1_is_acceptable = p1 != NULL && parent_has_usable_link(p1);
  p2_is_acceptable = p2 != NULL && parent_has_usable_link(p2);
  
  if(!p1_is_acceptable) {
    return p2_is_acceptable ? p2 : NULL;
  }
  if(!p2_is_acceptable) {
    return p1_is_acceptable ? p1 : NULL;
  }
  
  p1_cost = parent_path_cost(p1);
  p2_cost = parent_path_cost(p2);
  
  /* Maintain stability of the preferred parent */
  if(p1 == p1->instance->dag_parent || p2 == p2->instance->dag_parent) {
    if(p1_cost < p2_cost + PARENT_SWITCH_THRESHOLD &&
       p1_cost > p2_cost - PARENT_SWITCH_THRESHOLD) {
      return p1->instance->dag_parent;
    }
  }
  
  return p1_cost < p2_cost ? p1 : p2;
}

/*---------------------------------------------------------------------------*/
static void
update_metric_container(rpl_instance_t *instance)
{
  if(instance->dag_parent == NULL) {
    return;
  }
  
  instance->mc.type = RPL_DAG_MC_ETX;
  instance->mc.flags = 0;
  instance->mc.aggr = RPL_DAG_MC_AGGR_ADDITIVE;
  instance->mc.value = parent_link_metric(instance->dag_parent);
}

/*---------------------------------------------------------------------------*/
static uint16_t
calculate_rank(rpl_parent_t *parent, uint16_t base_rank)
{
  uint16_t rank;
  uint16_t parent_rank;
  
  if(parent == NULL) {
    return base_rank;
  }
  
  parent_rank = parent->rank;
  if(parent_rank == 0) {
    return base_rank;
  }
  
  rank = parent_rank + parent_link_metric(parent);
  if(rank < parent_rank) {
    return parent_rank;
  }
  
  return rank;
}

/*---------------------------------------------------------------------------*/
static void
parent_state_callback(rpl_parent_t *parent, int known, int etx)
{
  if(!known) {
    return;
  }
  
  if(etx) {
    parent->link_metric = parent_link_metric(parent);
  }
}

/*---------------------------------------------------------------------------*/
rpl_of_t *
rpl_mrhof_init(void)
{
  return &rpl_mrhof;
}
