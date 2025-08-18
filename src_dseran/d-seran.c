// d-seran.c : Distributed Self-Healing Energy-Aware Routing for Ad Hoc Networks (D-SERAN)
#include "contiki.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#include "project-conf.h"
#include "lib/random.h"
#include "net/linkaddr.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LOG_MODULE "D-SERAN"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MAX_NEIGHBORS 16
#define ENERGY_THRESHOLD 10 // mJ, seuil pour l'alerte faible énergie
#define TRUST_THRESHOLD 0.5 // seuil de confiance
#define INIT_TRUST 0.7
#define INIT_ENERGY 100
#define HARVEST_STEP 2
#define MAX_ENERGY 100
#define HELLO_INTERVAL (CLOCK_SECOND * 10)
#define ROUTE_TIMEOUT (CLOCK_SECOND * 30)

PROCESS(d_seran_process, "D-SERAN Routing Protocol");
AUTOSTART_PROCESSES(&d_seran_process);

// Structure pour un voisin
struct neighbor {
  linkaddr_t addr;
  float trust;
  uint16_t residual_energy;
  clock_time_t last_seen;
};
static struct neighbor neighbors[MAX_NEIGHBORS];
static uint8_t neighbor_count = 0;

// Energie du noeud
static uint16_t my_residual_energy = INIT_ENERGY;
static uint16_t my_harvested_energy = 0;

// UDP pour échanges "hello" (découverte/MAJ voisins)
static struct simple_udp_connection udp_conn;
#define UDP_PORT 1234

// Prototypes
static void send_hello(void);
static void process_hello(const linkaddr_t *src, uint16_t energy, float trust);
static void update_trust(const linkaddr_t *addr, float delta);
static void add_or_update_neighbor(const linkaddr_t *addr, uint16_t energy, float trust);
static linkaddr_t select_next_hop(void);
static void harvest_energy(void);
void notify_d_seran_of_movement(void);
static void udp_rx_callback(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr,
                           uint16_t sender_port, const uip_ipaddr_t *receiver_addr,
                           uint16_t receiver_port, const uint8_t *data, uint16_t datalen);

// Initialisation du protocole
static void d_seran_init(void) {
  neighbor_count = 0;
  my_residual_energy = INIT_ENERGY;
  my_harvested_energy = 0;
  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);
  LOG_INFO("D-SERAN initialisé\n");
}

// Envoi périodique de "hello" (découverte/MAJ voisins)
static void send_hello(void) {
  uint8_t buf[8];
  memcpy(&buf[0], &my_residual_energy, sizeof(uint16_t));
  float my_trust = 1.0; // On se fait confiance à soi-même
  memcpy(&buf[2], &my_trust, sizeof(float));
  // Ajout d'un champ random pour simuler la mobilité (adresse changeante)
  uint16_t rand_val = random_rand();
  memcpy(&buf[6], &rand_val, sizeof(uint16_t));
  simple_udp_sendto(&udp_conn, buf, sizeof(buf), NULL); // Broadcast
  LOG_INFO("SEND_UDP %u %lu\n", my_residual_energy, clock_time());
}

// Traitement d'un "hello" reçu
static void process_hello(const linkaddr_t *src, uint16_t energy, float trust) {
  add_or_update_neighbor(src, energy, trust);
  // Mise à jour de la confiance (exemple : bonus si le voisin répond régulièrement)
  update_trust(src, 0.01);
}

// Mise à jour de la confiance d'un voisin
static void update_trust(const linkaddr_t *addr, float delta) {
  for(uint8_t i=0; i<neighbor_count; i++) {
    if(linkaddr_cmp(&neighbors[i].addr, addr)) {
      neighbors[i].trust += delta;
      if(neighbors[i].trust > 1.0) neighbors[i].trust = 1.0;
      if(neighbors[i].trust < 0.0) neighbors[i].trust = 0.0;
      return;
    }
  }
}

// Ajout ou mise à jour d'un voisin
static void add_or_update_neighbor(const linkaddr_t *addr, uint16_t energy, float trust) {
  for(uint8_t i=0; i<neighbor_count; i++) {
    if(linkaddr_cmp(&neighbors[i].addr, addr)) {
      neighbors[i].residual_energy = energy;
      neighbors[i].trust = trust;
      neighbors[i].last_seen = clock_time();
      return;
    }
  }
  if(neighbor_count < MAX_NEIGHBORS) {
    linkaddr_copy(&neighbors[neighbor_count].addr, addr);
    neighbors[neighbor_count].residual_energy = energy;
    neighbors[neighbor_count].trust = trust;
    neighbors[neighbor_count].last_seen = clock_time();
    neighbor_count++;
  }
}

// Sélection du prochain saut (next hop) basé sur la confiance et l'énergie
static linkaddr_t select_next_hop(void) {
  float best_score = -1.0;
  int best_idx = -1;
  for(uint8_t i=0; i<neighbor_count; i++) {
    float score = neighbors[i].trust * (float)neighbors[i].residual_energy;
    if(score > best_score && neighbors[i].trust > TRUST_THRESHOLD && neighbors[i].residual_energy > ENERGY_THRESHOLD) {
      best_score = score;
      best_idx = i;
    }
  }
  if(best_idx >= 0) {
    LOG_INFO("HOP %u %lu\n", best_idx, clock_time());
    return neighbors[best_idx].addr;
  }
  return linkaddr_null;
}

// Simulation de récolte d'énergie (exemple)
static void harvest_energy(void) {
  my_harvested_energy += HARVEST_STEP;
  my_residual_energy += HARVEST_STEP;
  if(my_residual_energy > MAX_ENERGY) my_residual_energy = MAX_ENERGY;
  LOG_INFO("ENERGY %u %u\n", my_residual_energy, my_harvested_energy);
}

// Gestion de la mobilité (hook simplifié)
void notify_d_seran_of_movement(void) {
  LOG_INFO("MOVE %u %u %lu\n", linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1], (unsigned long)clock_time());
  LOG_INFO("Mouvement détecté, recalcul du routage\n");
  // Ici, on pourrait déclencher une redécouverte de voisins ou une mise à jour de la table
}

// Callback UDP pour réception de paquets (hello)
static void udp_rx_callback(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr,
                           uint16_t sender_port, const uip_ipaddr_t *receiver_addr,
                           uint16_t receiver_port, const uint8_t *data, uint16_t datalen) {
  if(datalen >= 6) {
    uint16_t energy;
    float trust;
    memcpy(&energy, &data[0], sizeof(uint16_t));
    memcpy(&trust, &data[2], sizeof(float));
    // On récupère l'adresse du voisin
    linkaddr_t src;
    memcpy(&src, &sender_addr->u8[14], 2); // IPv6 -> linkaddr
    process_hello(&src, energy, trust);
    LOG_INFO("RECV %u %lu\n", energy, clock_time());
  }
}

// Fonction principale du protocole
PROCESS_THREAD(d_seran_process, ev, data) {
  static struct etimer hello_timer, harvest_timer;
  PROCESS_BEGIN();
  d_seran_init();
  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);
  etimer_set(&hello_timer, HELLO_INTERVAL);
  etimer_set(&harvest_timer, CLOCK_SECOND * 5);
  while(1) {
    PROCESS_WAIT_EVENT();
    if(etimer_expired(&hello_timer)) {
      send_hello();
      etimer_reset(&hello_timer);
    }
    if(etimer_expired(&harvest_timer)) {
      harvest_energy();
      etimer_reset(&harvest_timer);
    }
    // Routage auto-réparateur : sélection dynamique du next hop
    linkaddr_t next_hop = select_next_hop();
    if(!linkaddr_cmp(&next_hop, &linkaddr_null)) {
      LOG_INFO("Next hop sélectionné : %u.%u\n", next_hop.u8[0], next_hop.u8[1]);
    } else {
      LOG_WARN("Aucun voisin fiable pour le routage\n");
    }
    if(my_residual_energy == 0) {
      LOG_INFO("LIFETIME %u %lu\n", linkaddr_node_addr.u8[0], clock_time());
      PROCESS_EXIT();
    }
  }
  PROCESS_END();
} 

const struct routing_driver d_seran_routing_driver = {
  "d-seran",
  NULL, NULL, NULL, NULL, NULL, NULL
}; 