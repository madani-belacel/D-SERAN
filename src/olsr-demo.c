/*
 * olsr-demo.c : Protocole de référence OLSR simplifié pour MANET sous Contiki-NG
 * Simplified OLSR reference protocol for MANET under Contiki-NG
 * 
 * Auteur / Author: Madani Belacel
 * Date de création / Created: 25/03/2023
 * Dernière mise à jour / Last updated: 02/05/2023
 * 
 * Implémentation simplifiée d'OLSR pour comparaison avec D-SERAN
 * Simplified OLSR implementation for comparison with D-SERAN
 */

#include "contiki.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/log.h"
#include "lib/random.h"
#include <stdio.h>
#include <stdlib.h>

#define LOG_MODULE "OLSR-DEMO"
#define LOG_LEVEL LOG_LEVEL_INFO

// Configuration des paramètres / Parameter configuration
#define UDP_PORT 1237
#define SEND_INTERVAL (CLOCK_SECOND * 8)
#define HELLO_INTERVAL (CLOCK_SECOND * 5)

PROCESS(olsr_demo_process, "OLSR Demo Protocol");
AUTOSTART_PROCESSES(&olsr_demo_process);

// Variables globales / Global variables
static struct uip_udp_conn *udp_conn;
static uint16_t seq_id = 0;

// Statistiques de performance / Performance statistics
static uint16_t packets_sent = 0;
static uint16_t hello_messages = 0;

// Simulation de l'énergie / Energy simulation
static uint16_t my_energy = 100;

// Fonction d'envoi de messages HELLO / HELLO message sending function
static void send_hello(void) {
  hello_messages++;
  
  // Simulation de l'envoi de messages HELLO / HELLO message sending simulation
  printf("OLSR: Message HELLO #%u envoyé\n", hello_messages);
  
  // Consommation d'énergie pour HELLO / Energy consumption for HELLO
  my_energy -= 1;
  
  LOG_INFO("HELLO_MSG %u %lu\n", hello_messages, clock_time());
}

// Fonction d'envoi de paquets / Packet sending function
static void send_packet(void) {
  char buf[32];
  
  // Préparation du message / Message preparation
  sprintf(buf, "OLSR_DATA %u %lu", seq_id++, clock_time());
  
  // Envoi UDP / UDP sending
  uip_udp_packet_send(udp_conn, buf, strlen(buf));
  
  // Mise à jour des statistiques / Statistics update
  packets_sent++;
  my_energy--;
  
  // Logs de débogage / Debug logs
  LOG_INFO("SEND_UDP %u %lu\n", seq_id, clock_time());
  LOG_INFO("ENERGY %u %lu\n", my_energy, clock_time());
  
  // Traces de débogage occasionnelles / Occasional debug traces
  if (random_rand() % 10 == 0) {
    printf("OLSR: Paquet envoyé #%u, énergie: %u mJ\n", seq_id-1, my_energy);
  }
}

// Processus principal OLSR / Main OLSR process
PROCESS_THREAD(olsr_demo_process, ev, data) {
  static struct etimer send_timer;
  static struct etimer hello_timer;
  
  PROCESS_BEGIN();
  
  // Configuration de la connexion UDP / UDP connection setup
  udp_conn = udp_new(NULL, UIP_HTONS(UDP_PORT), NULL);
  udp_bind(udp_conn, UIP_HTONS(UDP_PORT));
  
  // Configuration des timers / Timer setup
  etimer_set(&send_timer, SEND_INTERVAL);
  etimer_set(&hello_timer, HELLO_INTERVAL);
  
  printf("OLSR: Démonstration démarrée, port UDP: %d\n", UDP_PORT);
  
  while(1) {
    PROCESS_WAIT_EVENT();
    
    // Gestion du timer d'envoi / Send timer handling
    if(etimer_expired(&send_timer)) {
      send_packet();
      etimer_reset(&send_timer);
    }
    
    // Gestion du timer HELLO / HELLO timer handling
    if(etimer_expired(&hello_timer)) {
      send_hello();
      etimer_reset(&hello_timer);
    }
    
    // Vérification de la fin de vie / Lifetime check
    if(my_energy == 0) {
      LOG_INFO("LIFETIME %u %lu\n", linkaddr_node_addr.u8[0], clock_time());
      printf("OLSR: Énergie épuisée, arrêt du protocole\n");
      PROCESS_EXIT();
    }
  }
  
  PROCESS_END();
} 