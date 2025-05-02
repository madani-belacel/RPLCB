#ifndef UIP_DS6_ROUTE_DEFS_H
#define UIP_DS6_ROUTE_DEFS_H

#include <stdint.h>
#include "net/ipv6/uip-ds6.h"
#include "net/nbr-table.h"

/* Structures de base */
typedef struct rpl_route_entry {
  uip_ipaddr_t ipaddr;
  uint8_t length;
  uint8_t metric;
  uint8_t lifetime;
  struct rpl_route_entry *next;
} rpl_route_entry_t;

struct uip_ds6_route_neighbor_routes {
  struct uip_ds6_route_neighbor_route *route_list;
};

NBR_TABLE_DECLARE(nbr_routes);

struct uip_ds6_route {
  uip_ipaddr_t ipaddr;
  uint8_t length;
  uint8_t metric;
  uint8_t lifetime;
  struct uip_ds6_route *next;
};

struct uip_ds6_route_neighbor_route {
  struct uip_ds6_route *route;
  struct uip_ds6_route_neighbor_route *next;
};

struct uip_ds6_defrt {
  uip_ipaddr_t ipaddr;
  uint8_t is_infinite;
  uint8_t lifetime;
  struct uip_ds6_defrt *next;
};

typedef struct uip_ds6_route uip_ds6_route_t;
typedef struct uip_ds6_defrt uip_ds6_defrt_t;

#endif /* UIP_DS6_ROUTE_DEFS_H */ 