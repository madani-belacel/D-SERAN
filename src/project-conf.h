/*
 * project-conf.h : Configuration du projet D-SERAN
 * D-SERAN Project Configuration
 * 
 * Auteur / Author: Madani Belacel
 * Date: Août 2025
 * 
 * Fichier de configuration pour le protocole D-SERAN
 * Configuration file for D-SERAN protocol
 */

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

// Configuration spécifique D-SERAN / D-SERAN specific configuration
#ifdef D_SERAN_CONF
#define NETSTACK_CONF_ROUTING d_seran_routing_driver
#endif

// Paramètres réseau généraux / General network parameters
#define UIP_CONF_ROUTER              1
#define ENERGEST_CONF_ON             1

// Niveaux de log pour différents composants / Log levels for different components
#define LOG_CONF_LEVEL_RPL           LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_TCPIP         LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_IPV6          LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_MAC           LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_FRAMER        LOG_LEVEL_WARN

// Configuration des buffers et tables / Buffer and table configuration
#define QUEUEBUF_CONF_NUM            16
#define UIP_CONF_MAX_ROUTES          16
#define NBR_TABLE_CONF_MAX_NEIGHBORS 16

#endif /* PROJECT_CONF_H_ */ 