/**
 * @file rpl-callback.h
 * @brief Interface de gestion des callbacks pour le protocole RPL
 *
 * Ce module définit l'ensemble des fonctions de callback permettant de réagir
 * aux événements importants du protocole RPL (Routing Protocol for Low-Power 
 * and Lossy Networks). Il permet notamment de suivre les changements de topologie,
 * les modifications de rang et les opérations de maintenance du réseau.
 */

#ifndef RPL_CALLBACK_H
#define RPL_CALLBACK_H

#include "net/routing/rpl-classic/rpl-types.h"

/* Fonctions de callback principales pour RPL */
void rpl_callback_parent_switch(rpl_parent_t *old, rpl_parent_t *new);
void rpl_callback_global_repair(void);
void rpl_callback_local_repair(rpl_dag_t *dag);
void rpl_callback_join_instance(rpl_instance_t *instance);
void rpl_callback_leave_instance(rpl_instance_t *instance);
void rpl_callback_join_dag(rpl_dag_t *dag);
void rpl_callback_leave_dag(rpl_dag_t *dag);
void rpl_callback_parent_added(rpl_parent_t *parent);
void rpl_callback_parent_removed(rpl_parent_t *parent);
void rpl_callback_rank_changed(rpl_rank_t new_rank);
void rpl_callback_dio_out(void);
void rpl_callback_dao_out(void);

#ifdef RPL_CALLBACK_EXTENDED
/* Fonctions de callback étendues pour des besoins avancés */
void rpl_callback_parent_link_metric_update(rpl_parent_t *parent, uint16_t old_metric, uint16_t new_metric);
void rpl_callback_parent_path_cost_update(rpl_parent_t *parent, uint16_t old_cost, uint16_t new_cost);
void rpl_callback_trickle_timer_reset(void);
void rpl_callback_dao_ack_received(rpl_parent_t *parent, int status);
#endif /* RPL_CALLBACK_EXTENDED */

#endif /* RPL_CALLBACK_H */ 