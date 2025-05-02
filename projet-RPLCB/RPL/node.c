#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/routing/routing.h"
#include "net/routing/rpl-classic/rpl.h"
#include "net/routing/rpl-classic/rpl-private.h"
#include "rpl-types.h"

#include <stdio.h>
#include <string.h>

/* Configuration des logs */
#define LOG_MODULE "RPL"
#define LOG_LEVEL LOG_LEVEL_INFO
#include "sys/log.h"

/* Variables globales */
static struct uip_ds6_nbr *nbr;
static struct rpl_route_entry *route;
static struct uip_ds6_route *ds6_route;

/* Fonction principale */
PROCESS(node_process, "RPL Node");
AUTOSTART_PROCESSES(&node_process);

PROCESS_THREAD(node_process, ev, data)
{
  PROCESS_BEGIN();

  /* Initialisation */
  LOG_INFO("Starting RPL node\n");
  
  /* Configuration RPL */
  rpl_dag_t *dag;
  uip_ipaddr_t prefix;
  
  /* Création du préfixe */
  uip_ip6addr(&prefix, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  
  /* Création du DAG */
  dag = rpl_set_root(RPL_DEFAULT_INSTANCE, &prefix);
  if(dag != NULL) {
    rpl_set_prefix(dag, &prefix, 64);
    LOG_INFO("Created DAG with ID: ");
    LOG_INFO_6ADDR(&dag->dag_id);
    LOG_INFO_("\n");
  }

  /* Boucle principale */
  while(1) {
    PROCESS_WAIT_EVENT();
    
    /* Traitement des événements RPL */
    if(ev == PROCESS_EVENT_TIMER) {
      /* Mise à jour périodique */
      LOG_INFO("Periodic update\n");
    }
  }

  PROCESS_END();
} 