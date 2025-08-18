// project-conf.h : Configuration du projet D-SERAN
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

// Configuration spécifique D-SERAN
#ifdef D_SERAN_CONF
#define NETSTACK_CONF_ROUTING d_seran_routing_driver
#endif

#define UIP_CONF_ROUTER              1
#define ENERGEST_CONF_ON             1
#define LOG_CONF_LEVEL_RPL           LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_TCPIP         LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_IPV6          LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_MAC           LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_FRAMER        LOG_LEVEL_WARN
#define QUEUEBUF_CONF_NUM            16
#define UIP_CONF_MAX_ROUTES          16
#define NBR_TABLE_CONF_MAX_NEIGHBORS 16

#endif /* PROJECT_CONF_H_ */ 