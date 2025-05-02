#include "contiki.h"
#include "contiki-net.h"
#include "net/routing/rpl-lite/rpl.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"

#include "lifetime.c" // Calcul de durée de vie

#define LOG_MODULE "UDP client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

#define SEND_INTERVAL (60 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static uip_ipaddr_t dest_ipaddr;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  static uint32_t count = 0;

  PROCESS_BEGIN();

  LOG_INFO("UDP client started\n");

  // Initialisation du logging de durée de vie
  init_lifetime_logging();

  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, NULL);

  etimer_set(&timer, SEND_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    etimer_reset(&timer);

    // Adresse IP du routeur
    uip_ip6addr(&dest_ipaddr, 0xfd00, 0, 0, 0, 0, 0, 0, 1);

    char buf[50];
    snprintf(buf, sizeof(buf), "Message %lu from client", (unsigned long)count++);
    simple_udp_sendto(&udp_conn, buf, strlen(buf) + 1, &dest_ipaddr);
    LOG_INFO("Message sent: %s\n", buf);

    // Mettre à jour et afficher la durée de vie
    update_lifetime();
  }

  PROCESS_END();
}
