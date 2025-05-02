CONTIKI = /home/madani-belacel/contiki-ng

# Target platform
TARGET = native

# Source files
CONTIKI_PROJECT = node
PROJECT_SOURCEFILES += project-conf.h node.c rpl-callback.c

# Add include paths
CFLAGS += -I$(CONTIKI)/os
CFLAGS += -I$(CONTIKI)/arch/platform/$(TARGET)
CFLAGS += -I$(CONTIKI)/os/net/routing
CFLAGS += -I$(CONTIKI)/os/net/ipv6
CFLAGS += -DUIP_DS6_NBR_CONF_INLINE=0

# Use RPL Classic with Callbacks
MAKE_ROUTING = MAKE_ROUTING_RPL_CLASSIC
MODULES += os/net/routing/rpl-classic
MODULES += os/net/routing/rpl-callback

# Configuration
CFLAGS += -DUIP_CONF_IPV6=1
CFLAGS += -DUIP_CONF_IPV6_RPL=1
CFLAGS += -DRPL_CONF_WITH_RPL_CLASSIC=1
CFLAGS += -DRPL_CONF_WITH_RPL_LITE=0
CFLAGS += -DRPL_CALLBACK_EXTENDED=1

all: $(CONTIKI_PROJECT)

clean:
	rm -rf build/* *.native

include $(CONTIKI)/Makefile.include 