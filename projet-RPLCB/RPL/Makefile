CONTIKI = /home/madani-belacel/contiki-ng

# Configuration de la plateforme
TARGET = native

# Fichiers sources du projet
CONTIKI_PROJECT = node
PROJECT_SOURCEFILES += project-conf.h node.c

# Chemins d'inclusion
CFLAGS += -I$(CONTIKI)/os
CFLAGS += -I$(CONTIKI)/os/sys
CFLAGS += -I$(CONTIKI)/os/net
CFLAGS += -I$(CONTIKI)/os/net/ipv6
CFLAGS += -I$(CONTIKI)/os/net/routing
CFLAGS += -I$(CONTIKI)/os/net/routing/rpl-classic
CFLAGS += -I$(CONTIKI)/os/lib
CFLAGS += -I$(CONTIKI)/os/dev
CFLAGS += -I$(CONTIKI)/os/arch/platform/native

# Configuration RPL
CFLAGS += -DRPL_CONF_WITH_STORING=1
CFLAGS += -DRPL_CONF_WITH_MULTICAST=0
CFLAGS += -DUIP_DS6_NBR_CONF_INLINE=0
CFLAGS += -DUIP_DS6_ROUTE_CONF_INLINE=0
CFLAGS += -DUIP_CONF_IPV6=1
CFLAGS += -DRPL_CONF_WITH_IPV6=1
CFLAGS += -DRPL_CONF_WITH_RPL_CLASSIC=1
CFLAGS += -DRPL_CONF_WITH_RPL_LITE=0

# Use RPL Classic
MAKE_ROUTING = MAKE_ROUTING_RPL_CLASSIC
MODULES += os/net/routing/rpl-classic

all: $(CONTIKI_PROJECT)

clean:
	rm -rf build/* *.native

include $(CONTIKI)/Makefile.include 