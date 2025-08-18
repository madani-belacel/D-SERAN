/*
 * dsr-demo.c : Protocole de référence DSR simplifié pour MANET sous Contiki-NG
 * Simplified DSR reference protocol for MANET under Contiki-NG
 * 
 * Auteur / Author: Madani Belacel
 * Date de création / Created: 22/03/2023
 * Dernière mise à jour / Last updated: 28/04/2023
 * 
 * Implémentation simplifiée de DSR pour comparaison avec D-SERAN
 * Simplified DSR implementation for comparison with D-SERAN
 */

#include "contiki.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/log.h"
#include "lib/random.h"
#include <stdio.h>
#include <stdlib.h>

#define LOG_MODULE "DSR-DEMO"
#define LOG_LEVEL LOG_LEVEL_INFO

// Configuration des paramètres / Parameter configuration
#define UDP_PORT 1236
#define SEND_INTERVAL (CLOCK_SECOND * 12)
#define MAX_HOPS 15

PROCESS(dsr_demo_process, "DSR Demo Protocol");
AUTOSTART_PROCESSES(&dsr_demo_process);

// Variables globales / Global variables
static struct uip_udp_conn *udp_conn;
static uint16_t seq_id = 0;

// Statistiques de performance / Performance statistics
static uint16_t packets_sent = 0;
static uint16_t route_discoveries = 0;

// Simulation de l'énergie / Energy simulation
static uint16_t my_energy = 100;

// Fonction de découverte de route / Route discovery function
static void discover_route(void) {
  route_discoveries++;
  
  // Simulation de la découverte de route / Route discovery simulation
  printf("DSR: Découverte de route #%u initiée\n", route_discoveries);
  
  // Consommation d'énergie pour la découverte / Energy consumption for discovery
  my_energy -= 2;
  
  LOG_INFO("ROUTE_DISC %u %lu\n", route_discoveries, clock_time());
}

// Fonction d'envoi de paquets / Packet sending function
static void send_packet(void) {
  char buf[32];
  
  // Préparation du message / Message preparation
  sprintf(buf, "DSR_DATA %u %lu", seq_id++, clock_time());
  
  // Envoi UDP / UDP sending
  uip_udp_packet_send(udp_conn, buf, strlen(buf));
  
  // Mise à jour des statistiques / Statistics update
  packets_sent++;
  my_energy--;
  
  // Logs de débogage / Debug logs
  LOG_INFO("SEND_UDP %u %lu\n", seq_id, clock_time());
  LOG_INFO("ENERGY %u %lu\n", my_energy, clock_time());
  
  // Traces de débogage occasionnelles / Occasional debug traces
  if (random_rand() % 12 == 0) {
    printf("DSR: Paquet envoyé #%u, énergie: %u mJ\n", seq_id-1, my_energy);
  }
}

// Processus principal DSR / Main DSR process
PROCESS_THREAD(dsr_demo_process, ev, data) {
  static struct etimer send_timer;
  static struct etimer route_timer;
  
  PROCESS_BEGIN();
  
  // Configuration de la connexion UDP / UDP connection setup
  udp_conn = udp_new(NULL, UIP_HTONS(UDP_PORT), NULL);
  udp_bind(udp_conn, UIP_HTONS(UDP_PORT));
  
  // Configuration des timers / Timer setup
  etimer_set(&send_timer, SEND_INTERVAL);
  etimer_set(&route_timer, CLOCK_SECOND * 30);  // Découverte de route périodique
  
  printf("DSR: Démonstration démarrée, port UDP: %d\n", UDP_PORT);
  
  while(1) {
    PROCESS_WAIT_EVENT();
    
    // Gestion du timer d'envoi / Send timer handling
    if(etimer_expired(&send_timer)) {
      send_packet();
      etimer_reset(&send_timer);
    }
    
    // Gestion du timer de découverte de route / Route discovery timer handling
    if(etimer_expired(&route_timer)) {
      discover_route();
      etimer_reset(&route_timer);
    }
    
    // Vérification de la fin de vie / Lifetime check
    if(my_energy == 0) {
      LOG_INFO("LIFETIME %u %lu\n", linkaddr_node_addr.u8[0], clock_time());
      printf("DSR: Énergie épuisée, arrêt du protocole\n");
      PROCESS_EXIT();
    }
  }
  
  PROCESS_END();
} 