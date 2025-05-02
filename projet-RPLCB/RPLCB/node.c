/*
 * Implémentation d'un nœud RPLCB
 * RPLCB node implementation
 * 
 * Créé le: 01/03/2021
 * Modifié le: 12/09/2021
 * Auteur: M. Belacel
 */

#include "contiki.h"
#include "sys/log.h"
#include "sys/node-id.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "rpl-callback.h"

/* Module de log pour le debug */
#define LOG_MODULE "Node-RPLCB"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Paramètres du nœud / Node parameters */
#define PORT_UDP 8765  // Port pour communication UDP / UDP communication port
#define PERIODE_ENVOI (60 * CLOCK_SECOND)  // 1 min entre chaque envoi / 1 min between sends

/* Déclaration du process principal / Main process declaration */
PROCESS(process_noeud_rplcb, "Noeud RPLCB");
AUTOSTART_PROCESSES(&process_noeud_rplcb);

/* Socket UDP pour l'envoi/réception / UDP socket for sending/receiving */
static struct simple_udp_connection socket_udp;

/* Fonction de callback UDP / UDP callback function 
 * Appelée à chaque réception de paquet / Called for each received packet */
static void
callback_reception_udp(struct simple_udp_connection *c,
                const uip_ipaddr_t *addr_exp,
                uint16_t port_exp,
                const uip_ipaddr_t *addr_dest,
                uint16_t port_dest,
                const uint8_t *donnees,
                uint16_t taille)
{
    LOG_INFO("Message reçu: '%.*s' de ", taille, (char *)donnees);
    LOG_INFO_6ADDR(addr_exp);
    LOG_INFO_("\n");
}

/* Callback de changement de parent RPL / RPL parent change callback */
void rpl_callback_parent_switch(rpl_parent_t *ancien, rpl_parent_t *nouveau)
{
    if(ancien != NULL && nouveau != NULL) {
        LOG_INFO("Parent changé: ");
        LOG_INFO_6ADDR(rpl_parent_get_ipaddr(ancien));
        LOG_INFO_(" -> ");
        LOG_INFO_6ADDR(rpl_parent_get_ipaddr(nouveau));
        LOG_INFO_("\n");
    }
}

/* Process principal / Main process */
PROCESS_THREAD(process_noeud_rplcb, ev, data)
{
    static struct etimer timer;
    static char msg[32];
    uip_ipaddr_t addr_racine;
    
    PROCESS_BEGIN();
    
    /* Init réseau / Network init */
    NETSTACK_ROUTING.init();
    NETSTACK_MAC.init();
    NETSTACK_NETWORK.init();
    
    /* Config UDP */
    simple_udp_register(&socket_udp, PORT_UDP, NULL, PORT_UDP, callback_reception_udp);
    
    /* Enregistrement callback RPL / Register RPL callback */
    rpl_callback_register_parent_switch(rpl_callback_parent_switch);
    
    /* Nœud 1 = racine / Node 1 = root */
    if(node_id == 1) {
        LOG_INFO("Démarrage en tant que racine\n");
        NETSTACK_ROUTING.root_start();
    } else {
        LOG_INFO("Démarrage en tant que nœud normal (ID: %d)\n", node_id);
    }
    
    /* Config timer périodique / Setup periodic timer */
    etimer_set(&timer, PERIODE_ENVOI);
    
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        if(node_id != 1) {  // Si pas racine / If not root
            /* Prépare message / Prepare message */
            snprintf(msg, sizeof(msg), "msg_noeud_%d", node_id);
            
            /* Récupère addr racine / Get root address */
            if(NETSTACK_ROUTING.get_root_ipaddr(&addr_racine)) {
                /* Envoi / Send */
                simple_udp_sendto(&socket_udp, msg, strlen(msg), &addr_racine);
                LOG_INFO("Message envoyé: %s\n", msg);
            }
        }
        
        /* Reset timer */
        etimer_reset(&timer);
    }
    
    PROCESS_END();
} 