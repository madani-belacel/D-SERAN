/*
 * aodv-demo.c : Protocole de référence simple (UDP Flooding) pour MANET sous Contiki-NG
 * Simple reference protocol (UDP Flooding) for MANET under Contiki-NG
 * 
 * Auteur / Author: Madani Belacel
 * Date de création / Created: 18/03/2023
 * Dernière mise à jour / Last updated: 25/04/2023
 * 
 * Implémentation simplifiée d'AODV pour comparaison avec D-SERAN
 * Simplified AODV implementation for comparison with D-SERAN
 */

#include "contiki.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/log.h"
#include "lib/random.h"
#include <stdio.h>
#include <stdlib.h>

#define LOG_MODULE "AODV-DEMO"
#define LOG_LEVEL LOG_LEVEL_INFO

// Configuration des paramètres / Parameter configuration
#define UDP_PORT 1235
#define SEND_INTERVAL (CLOCK_SECOND * 10)
#define MAX_HOPS 10

PROCESS(aodv_demo_process, "AODV/UDP Flooding Demo");
AUTOSTART_PROCESSES(&aodv_demo_process);

// Variables globales / Global variables
static struct uip_udp_conn *udp_conn;
static uint16_t seq_id = 0;

// Statistiques de performance / Performance statistics
static uint16_t packets_sent = 0;

// Simulation de l'énergie / Energy simulation
static uint16_t my_energy = 100;

// Fonction d'envoi de paquets / Packet sending function
static void send_packet(void) {
  char buf[32];
  
  // Préparation du message / Message preparation
  sprintf(buf, "DATA %u %lu", seq_id++, clock_time());
  
  // Envoi UDP / UDP sending
  uip_udp_packet_send(udp_conn, buf, strlen(buf));
  
  // Mise à jour des statistiques / Statistics update
  packets_sent++;
  my_energy--;
  
  // Logs de débogage / Debug logs
  LOG_INFO("SEND_UDP %u %lu\n", seq_id, clock_time());
  LOG_INFO("ENERGY %u %lu\n", my_energy, clock_time());
  
  // Traces de débogage occasionnelles / Occasional debug traces
  if (random_rand() % 15 == 0) {
    printf("AODV: Paquet envoyé #%u, énergie: %u mJ\n", seq_id-1, my_energy);
  }
}

// Processus principal AODV / Main AODV process
PROCESS_THREAD(aodv_demo_process, ev, data) {
  static struct etimer send_timer;
  
  PROCESS_BEGIN();
  
  // Configuration de la connexion UDP / UDP connection setup
  udp_conn = udp_new(NULL, UIP_HTONS(UDP_PORT), NULL);
  udp_bind(udp_conn, UIP_HTONS(UDP_PORT));
  
  // Configuration du timer d'envoi / Send timer setup
  etimer_set(&send_timer, SEND_INTERVAL);
  
  printf("AODV: Démonstration démarrée, port UDP: %d\n", UDP_PORT);
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));
    
    // Envoi périodique / Periodic sending
    send_packet();
    etimer_reset(&send_timer);
    
    // Vérification de la fin de vie / Lifetime check
    if(my_energy == 0) {
      LOG_INFO("LIFETIME %u %lu\n", linkaddr_node_addr.u8[0], clock_time());
      printf("AODV: Énergie épuisée, arrêt du protocole\n");
      PROCESS_EXIT();
    }
  }
  
  PROCESS_END();
} 