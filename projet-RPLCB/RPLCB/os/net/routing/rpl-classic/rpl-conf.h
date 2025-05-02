#ifndef RPL_CONF_H
#define RPL_CONF_H

/* RPL Mode */
#define RPL_CONF_MODE RPL_MODE_CLASSIC

/* Enable RPL Callbacks */
#define RPL_CALLBACK_EXTENDED 1

/* DIO Configuration */
#define RPL_DIO_INTERVAL_MIN 12
#define RPL_DIO_INTERVAL_DOUBLINGS 8

/* DAO Configuration */
#define RPL_CONF_WITH_DAO_ACK 1
#define RPL_CONF_DAO_DELAY 4

/* Objective Function */
#define RPL_CONF_OF rpl_mrhof

/* Other Parameters */
#define RPL_CONF_WITH_PROBING 1
#define RPL_CONF_PROBING_INTERVAL 120
#define RPL_CONF_MAX_RANKINC 0
#define RPL_CONF_MIN_HOPRANKINC 256

#endif /* RPL_CONF_H */ 