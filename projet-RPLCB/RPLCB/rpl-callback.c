/**
 * @file rpl-callback.c
 * @brief Implémentation des callbacks pour le protocole RPL
 *
 * Ce fichier contient l'implémentation des fonctions de callback définies dans
 * rpl-callback.h. Il gère la notification des événements importants du protocole
 * RPL et fournit des informations détaillées via le système de journalisation.
 */

#include "contiki.h"
#include "lib/list.h"
#include "sys/stimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/nbr-table.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "net/routing/rpl-classic/rpl-conf.h"
#include "net/routing/rpl-classic/rpl-private.h"
#include "net/routing/rpl-classic/rpl.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#include "rpl-callback.h"

#define LOG_MODULE "RPL-CB"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Pointeurs vers les fonctions de callback RPL */
static rpl_callback_parent_switch_t parent_switch_callback = NULL;
static rpl_callback_dio_interval_t dio_interval_callback = NULL;
static rpl_callback_instance_join_t instance_join_callback = NULL;
static rpl_callback_instance_leave_t instance_leave_callback = NULL;
static rpl_callback_dag_join_t dag_join_callback = NULL;
static rpl_callback_dag_leave_t dag_leave_callback = NULL;
static rpl_callback_metric_update_t metric_update_callback = NULL;
static rpl_callback_path_cost_t path_cost_callback = NULL;
static rpl_callback_dao_ack_t dao_ack_callback = NULL;

/* Fonctions pour enregistrer les callbacks */
void rpl_callback_register_parent_switch(rpl_callback_parent_switch_t callback) {
    parent_switch_callback = callback;
}

void rpl_callback_register_dio_interval(rpl_callback_dio_interval_t callback) {
    dio_interval_callback = callback;
}

void rpl_callback_register_instance_join(rpl_callback_instance_join_t callback) {
    instance_join_callback = callback;
}

void rpl_callback_register_instance_leave(rpl_callback_instance_leave_t callback) {
    instance_leave_callback = callback;
}

void rpl_callback_register_dag_join(rpl_callback_dag_join_t callback) {
    dag_join_callback = callback;
}

void rpl_callback_register_dag_leave(rpl_callback_dag_leave_t callback) {
    dag_leave_callback = callback;
}

void rpl_callback_register_metric_update(rpl_callback_metric_update_t callback) {
    metric_update_callback = callback;
}

void rpl_callback_register_path_cost(rpl_callback_path_cost_t callback) {
    path_cost_callback = callback;
}

void rpl_callback_register_dao_ack(rpl_callback_dao_ack_t callback) {
    dao_ack_callback = callback;
}

/* Fonctions utilitaires pour RPL */
rpl_rank_t rpl_parent_get_rank(rpl_parent_t *parent) {
    return (parent == NULL) ? RPL_INFINITE_RANK : parent->rank;
}

uint16_t rpl_get_parent_link_metric(void) {
    rpl_parent_t *parent = curr_instance.dag.preferred_parent;
    return (parent == NULL) ? RPL_MAX_LINK_METRIC : parent->link_metric;
}

void rpl_local_repair(void) {
    rpl_local_repair_root(&curr_instance.dag);
}

void rpl_global_repair(void) {
    rpl_global_repair_root(&curr_instance.dag);
}

void rpl_set_dio_interval(uint8_t interval) {
    if(interval >= RPL_DIO_INTERVAL_MIN) {
        curr_instance.dag.dio_intcurrent = interval;
        if(dio_interval_callback != NULL) {
            dio_interval_callback(interval);
        }
    }
}

/* Implémentation des callbacks RPL */
void rpl_callback_parent_switch_trigger(rpl_parent_t *old, rpl_parent_t *new) {
    if(parent_switch_callback != NULL) {
        parent_switch_callback(old, new);
    }
}

void rpl_callback_parent_switch(rpl_parent_t *old, rpl_parent_t *new) {
    if(old != NULL && new != NULL) {
        LOG_INFO("Changement de nœud parent - Ancien: ");
        LOG_INFO_6ADDR(rpl_parent_get_ipaddr(old));
        LOG_INFO_(" (rang: %u) -> Nouveau: ", rpl_parent_get_rank(old));
        LOG_INFO_6ADDR(rpl_parent_get_ipaddr(new));
        LOG_INFO_(" (rang: %u)\n", rpl_parent_get_rank(new));
    }
}

void rpl_callback_parent_link_metric(rpl_parent_t *parent, const uip_ipaddr_t *addr) {
    if(parent != NULL && addr != NULL) {
        LOG_INFO("Mise à jour métrique lien pour parent ");
        LOG_INFO_6ADDR(addr);
        LOG_INFO_("\n");
    }
}

void rpl_callback_global_repair(void) {
    LOG_INFO("Déclenchement d'une réparation globale du réseau RPL\n");
}

void rpl_callback_local_repair(rpl_dag_t *dag) {
    if(dag != NULL) {
        LOG_INFO("Réparation locale initiée pour le DAG (ID: ");
        LOG_INFO_6ADDR(&dag->dag_id);
        LOG_INFO_(", Version: %u)\n", dag->version);
    }
}

void rpl_callback_leaf_only(void) {
    LOG_INFO("Le nœud fonctionne maintenant en mode feuille uniquement\n");
}

void rpl_callback_join_network(uip_ipaddr_t *prefix, rpl_dag_t *dag) {
    if(prefix != NULL && dag != NULL) {
        LOG_INFO("Rejoint le réseau avec préfixe ");
        LOG_INFO_6ADDR(prefix);
        LOG_INFO_(" DAG ");
        LOG_INFO_6ADDR(&dag->dag_id);
        LOG_INFO_("\n");
    }
}

void rpl_callback_remove_parent(rpl_parent_t *parent) {
    if(parent != NULL) {
        LOG_INFO("Parent supprimé ");
        LOG_INFO_6ADDR(rpl_parent_get_ipaddr(parent));
        LOG_INFO_("\n");
    }
}

#if RPL_CALLBACK_EXTENDED
void rpl_callback_rank_update(uint16_t old_rank, uint16_t new_rank) {
    LOG_INFO("Rang mis à jour de %u à %u\n", old_rank, new_rank);
}

void rpl_callback_dio_output(uip_ipaddr_t *uc_addr) {
    if(uc_addr != NULL) {
        LOG_INFO("DIO envoyé à ");
        LOG_INFO_6ADDR(uc_addr);
        LOG_INFO_("\n");
    }
}

void rpl_callback_dao_output(uip_ipaddr_t *addr) {
    if(addr != NULL) {
        LOG_INFO("DAO envoyé à ");
        LOG_INFO_6ADDR(addr);
        LOG_INFO_("\n");
    }
}

void rpl_callback_network_status(void) {
    LOG_INFO("Mise à jour du statut du réseau\n");
}
#endif

void rpl_callback_instance_join(rpl_instance_t *instance) {
    if(instance_join_callback != NULL) {
        instance_join_callback(instance);
    }
    LOG_INFO("Connexion à l'instance RPL %u (rang initial: %u)\n", 
             instance->instance_id, instance->current_dag->rank);
}

void rpl_callback_instance_leave(rpl_instance_t *instance) {
    if(instance_leave_callback != NULL) {
        instance_leave_callback(instance);
    }
    LOG_INFO("Déconnexion de l'instance RPL %u\n", instance->instance_id);
}

void rpl_callback_dag_join(rpl_dag_t *dag) {
    if(dag_join_callback != NULL) {
        dag_join_callback(dag);
    }
    LOG_INFO("Rejoint le DAG ");
    LOG_INFO_6ADDR(&dag->dag_id);
    LOG_INFO_("\n");
}

void rpl_callback_dag_leave(rpl_dag_t *dag) {
    if(dag_leave_callback != NULL) {
        dag_leave_callback(dag);
    }
    LOG_INFO("Quitte le DAG ");
    LOG_INFO_6ADDR(&dag->dag_id);
    LOG_INFO_("\n");
}

void rpl_callback_metric_update(rpl_parent_t *parent, uint16_t old_metric, uint16_t new_metric) {
    if(metric_update_callback != NULL) {
        metric_update_callback(parent, old_metric, new_metric);
    }
    if(parent != NULL) {
        LOG_INFO("Mise à jour de la métrique pour le parent ");
        LOG_INFO_6ADDR(rpl_parent_get_ipaddr(parent));
        LOG_INFO_(" : %u -> %u\n", old_metric, new_metric);
    }
}

void rpl_callback_path_cost(rpl_parent_t *parent, uint16_t path_cost) {
    if(path_cost_callback != NULL) {
        path_cost_callback(parent, path_cost);
    }
    if(parent != NULL) {
        LOG_INFO("Nouveau coût de chemin vers le parent ");
        LOG_INFO_6ADDR(rpl_parent_get_ipaddr(parent));
        LOG_INFO_(" : %u\n", path_cost);
    }
}

void rpl_callback_dao_ack(uint8_t sequence) {
    if(dao_ack_callback != NULL) {
        dao_ack_callback(sequence);
    }
    LOG_INFO("ACK DAO reçu - Numéro de séquence: %u\n", sequence);
} 