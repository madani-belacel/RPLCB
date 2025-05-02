#ifndef RPL_TYPES_H
#define RPL_TYPES_H

#include "contiki.h"
#include "net/ipv6/uip.h"

/* Définitions des types personnalisés pour éviter les conflits */
typedef struct custom_nbr {
  struct custom_nbr *next;
  uip_ipaddr_t ipaddr;
  uint8_t state;
  uint8_t used;
  uint8_t isused;
  uint8_t isrouter;
  uint8_t link_metric;
  uint8_t link_metric_confidence;
  uint8_t link_metric_etx;
  uint8_t link_metric_confidence_etx;
} custom_nbr_t;

typedef struct custom_route {
  struct custom_route *next;
  uip_ipaddr_t ipaddr;
  uint8_t length;
  uint8_t used;
  uint8_t isused;
  uint8_t isrouter;
  uint8_t route_metric;
  uint8_t route_metric_confidence;
  uint8_t route_metric_etx;
  uint8_t route_metric_confidence_etx;
} custom_route_t;

/* Redéfinition des types système */
#define uip_ds6_nbr_t custom_nbr_t
#define uip_ds6_route_t custom_route_t
#define rpl_route_entry_t custom_route_t

#endif /* RPL_TYPES_H */ 