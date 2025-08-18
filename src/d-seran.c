/*
 * d-seran.c : Distributed Self-Healing Energy-Aware Routing for Ad Hoc Networks
 * Protocole de routage distribué auto-réparateur et conscient de l'énergie
 * 
 * Auteur / Author: Madani Belacel
 * Date: Août 2025
 * 
 * Implémentation du protocole D-SERAN pour réseaux ad hoc
 * Implementation of D-SERAN protocol for ad hoc networks
 */

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

// Configuration des seuils et paramètres / Thresholds and parameters configuration
#define MAX_NEIGHBORS 16
#define ENERGY_THRESHOLD 10    // mJ, seuil pour l'alerte faible énergie / energy alert threshold
#define TRUST_THRESHOLD 0.5    // seuil de confiance / trust threshold
#define INIT_TRUST 0.7
#define INIT_ENERGY 100
#define HARVEST_STEP 2
#define MAX_ENERGY 100
#define HELLO_INTERVAL (CLOCK_SECOND * 10)
#define ROUTE_TIMEOUT (CLOCK_SECOND * 30)

PROCESS(d_seran_process, "D-SERAN Routing Protocol");
AUTOSTART_PROCESSES(&d_seran_process);

// Structure pour un voisin / Structure for a neighbor
struct neighbor {
  linkaddr_t addr;
  float trust;
  uint16_t residual_energy;
  clock_time_t last_seen;
};
static struct neighbor neighbors[MAX_NEIGHBORS];
static uint8_t neighbor_count = 0;

// Energie du noeud / Node energy
static uint16_t my_residual_energy = INIT_ENERGY;
static uint16_t my_harvested_energy = 0;

// UDP pour échanges "hello" (découverte/MAJ voisins) / UDP for hello exchanges
static struct simple_udp_connection udp_conn;
#define UDP_PORT 1234

// Prototypes des fonctions / Function prototypes
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

// Initialisation du protocole / Protocol initialization
static void d_seran_init(void) {
  neighbor_count = 0;
  my_residual_energy = INIT_ENERGY;
  my_harvested_energy = 0;
  
  // Configuration UDP pour communication / UDP setup for communication
  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);
  
  // Traces de débogage / Debug traces
  printf("D-SERAN: Initialisation terminée, énergie: %u mJ\n", my_residual_energy);
  LOG_INFO("D-SERAN initialisé\n");
}

// Envoi périodique de "hello" (découverte/MAJ voisins) / Periodic hello sending
static void send_hello(void) {
  uint8_t buf[8];
  
  // Préparation des données hello / Hello data preparation
  memcpy(&buf[0], &my_residual_energy, sizeof(uint16_t));
  float my_trust = 1.0;  // On se fait confiance à soi-même / We trust ourselves
  memcpy(&buf[2], &my_trust, sizeof(float));
  
  // Ajout d'un champ random pour simuler la mobilité / Random field for mobility simulation
  uint16_t rand_val = random_rand();
  memcpy(&buf[6], &rand_val, sizeof(uint16_t));
  
  // Broadcast du message hello / Hello message broadcast
  simple_udp_sendto(&udp_conn, buf, sizeof(buf), NULL);
  
  // Log avec timestamp / Log with timestamp
  LOG_INFO("SEND_UDP %u %lu\n", my_residual_energy, clock_time());
  
  // Trace de débogage occasionnelle / Occasional debug trace
  if (random_rand() % 10 == 0) {
    printf("D-SERAN: Hello envoyé, énergie résiduelle: %u mJ\n", my_residual_energy);
  }
}

// Traitement d'un "hello" reçu / Processing received hello
static void process_hello(const linkaddr_t *src, uint16_t energy, float trust) {
  add_or_update_neighbor(src, energy, trust);
  
  // Mise à jour de la confiance / Trust update
  update_trust(src, 0.01);
  
  // Vérification de l'énergie du voisin / Neighbor energy check
  if (energy < ENERGY_THRESHOLD) {
    printf("D-SERAN: Voisin %02x:%02x a une énergie faible: %u mJ\n", 
           src->u8[0], src->u8[1], energy);
  }
}

// Mise à jour de la confiance d'un voisin / Update neighbor trust
static void update_trust(const linkaddr_t *addr, float delta) {
  for(uint8_t i=0; i<neighbor_count; i++) {
    if(linkaddr_cmp(&neighbors[i].addr, addr)) {
      neighbors[i].trust += delta;
      
      // Limitation de la confiance entre 0 et 1 / Trust limitation between 0 and 1
      if(neighbors[i].trust > 1.0) neighbors[i].trust = 1.0;
      if(neighbors[i].trust < 0.0) neighbors[i].trust = 0.0;
      
      return;
    }
  }
}

// Ajout ou mise à jour d'un voisin / Add or update neighbor
static void add_or_update_neighbor(const linkaddr_t *addr, uint16_t energy, float trust) {
  for(uint8_t i=0; i<neighbor_count; i++) {
    if(linkaddr_cmp(&neighbors[i].addr, addr)) {
      // Mise à jour des informations existantes / Update existing information
      neighbors[i].residual_energy = energy;
      neighbors[i].trust = trust;
      neighbors[i].last_seen = clock_time();
      
      // Trace de débogage / Debug trace
      printf("D-SERAN: Voisin %02x:%02x mis à jour, énergie: %u, confiance: %.2f\n",
             addr->u8[0], addr->u8[1], energy, trust);
      return;
    }
  }
  
  // Ajout d'un nouveau voisin / Add new neighbor
  if(neighbor_count < MAX_NEIGHBORS) {
    linkaddr_copy(&neighbors[neighbor_count].addr, addr);
    neighbors[neighbor_count].residual_energy = energy;
    neighbors[neighbor_count].trust = trust;
    neighbors[neighbor_count].last_seen = clock_time();
    neighbor_count++;
    
    printf("D-SERAN: Nouveau voisin ajouté: %02x:%02x (total: %u)\n", 
           addr->u8[0], addr->u8[1], neighbor_count);
  } else {
    printf("D-SERAN: Impossible d'ajouter le voisin %02x:%02x, table pleine\n",
           addr->u8[0], addr->u8[1]);
  }
}

// Sélection du prochain saut basé sur la confiance et l'énergie / Next hop selection
static linkaddr_t select_next_hop(void) {
  float best_score = -1.0;
  int best_idx = -1;
  
  // Évaluation de tous les voisins / Evaluate all neighbors
  for(uint8_t i=0; i<neighbor_count; i++) {
    // Calcul du score combiné / Combined score calculation
    float score = neighbors[i].trust * (float)neighbors[i].residual_energy;
    
    if(score > best_score && neighbors[i].trust > TRUST_THRESHOLD && 
       neighbors[i].residual_energy > ENERGY_THRESHOLD) {
      best_score = score;
      best_idx = i;
    }
  }
  
  if(best_idx >= 0) {
    LOG_INFO("HOP %u %lu\n", best_idx, clock_time());
    
    // Log détaillé de la sélection / Detailed selection log
    printf("D-SERAN: Prochain saut sélectionné: %02x:%02x (score: %.2f)\n",
           neighbors[best_idx].addr.u8[0], neighbors[best_idx].addr.u8[1], best_score);
    
    return neighbors[best_idx].addr;
  }
  
  printf("D-SERAN: Aucun voisin valide trouvé pour le routage\n");
  return linkaddr_null;
}

// Simulation de récolte d'énergie / Energy harvesting simulation
static void harvest_energy(void) {
  my_harvested_energy += HARVEST_STEP;
  my_residual_energy += HARVEST_STEP;
  
  // Limitation de l'énergie maximale / Maximum energy limitation
  if(my_residual_energy > MAX_ENERGY) {
    my_residual_energy = MAX_ENERGY;
  }
  
  LOG_INFO("ENERGY %u %u\n", my_residual_energy, my_harvested_energy);
  
  // Affichage périodique de l'état énergétique / Periodic energy status display
  static uint8_t harvest_count = 0;
  if(++harvest_count % 5 == 0) {
    printf("D-SERAN: Énergie récoltée: %u mJ, résiduelle: %u mJ\n", 
           my_harvested_energy, my_residual_energy);
  }
}

// Gestion de la mobilité / Mobility management
void notify_d_seran_of_movement(void) {
  LOG_INFO("MOVE %u %u %lu\n", linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1], (unsigned long)clock_time());
  
  printf("D-SERAN: Mouvement détecté, recalcul du routage en cours...\n");
  
  // Ici, on pourrait déclencher une redécouverte de voisins / Here we could trigger neighbor rediscovery
  // ou une mise à jour de la table de routage / or routing table update
}

// Callback UDP pour réception de paquets hello / UDP callback for hello packet reception
static void udp_rx_callback(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr,
                           uint16_t sender_port, const uip_ipaddr_t *receiver_addr,
                           uint16_t receiver_port, const uint8_t *data, uint16_t datalen) {
  if(datalen >= 6) {
    uint16_t energy;
    float trust;
    
    // Extraction des données reçues / Extract received data
    memcpy(&energy, &data[0], sizeof(uint16_t));
    memcpy(&trust, &data[2], sizeof(float));
    
    // Récupération de l'adresse du voisin / Get neighbor address
    linkaddr_t src;
    memcpy(&src, &sender_addr->u8[14], 2);  // IPv6 -> linkaddr conversion
    
    // Traitement du message hello / Process hello message
    process_hello(&src, energy, trust);
    LOG_INFO("RECV %u %lu\n", energy, clock_time());
    
    // Traces de débogage occasionnelles / Occasional debug traces
    if (random_rand() % 20 == 0) {
      printf("D-SERAN: Hello reçu de %02x:%02x, énergie: %u, confiance: %.2f\n",
             src.u8[0], src.u8[1], energy, trust);
    }
  }
}

// Fonction principale du protocole / Main protocol function
PROCESS_THREAD(d_seran_process, ev, data) {
  static struct etimer hello_timer, harvest_timer;
  
  PROCESS_BEGIN();
  
  // Initialisation et configuration / Initialization and setup
  d_seran_init();
  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);
  
  // Configuration des timers / Timer setup
  etimer_set(&hello_timer, HELLO_INTERVAL);
  etimer_set(&harvest_timer, CLOCK_SECOND * 5);
  
  printf("D-SERAN: Processus principal démarré, timers configurés\n");
  
  while(1) {
    PROCESS_WAIT_EVENT();
    
    // Gestion du timer hello / Hello timer management
    if(etimer_expired(&hello_timer)) {
      send_hello();
      etimer_reset(&hello_timer);
    }
    
    // Gestion du timer de récolte d'énergie / Energy harvesting timer management
    if(etimer_expired(&harvest_timer)) {
      harvest_energy();
      etimer_reset(&harvest_timer);
    }
    
    // Routage auto-réparateur : sélection dynamique du next hop / Self-healing routing
    linkaddr_t next_hop = select_next_hop();
    if(!linkaddr_cmp(&next_hop, &linkaddr_null)) {
      LOG_INFO("Next hop sélectionné : %u.%u\n", next_hop.u8[0], next_hop.u8[1]);
      
      // Affichage occasionnel du routage / Occasional routing display
      static uint8_t route_count = 0;
      if(++route_count % 15 == 0) {
        printf("D-SERAN: Routage actif vers %02x:%02x\n", next_hop.u8[0], next_hop.u8[1]);
      }
    } else {
      LOG_WARN("Aucun voisin fiable pour le routage\n");
      printf("D-SERAN: Aucune route disponible, attente de nouveaux voisins...\n");
    }
    
    // Vérification de la fin de vie / Lifetime check
    if(my_residual_energy == 0) {
      LOG_INFO("LIFETIME %u %lu\n", linkaddr_node_addr.u8[0], clock_time());
      printf("D-SERAN: Énergie épuisée, arrêt du protocole\n");
      PROCESS_EXIT();
    }
  }
  
  PROCESS_END();
} 

// Structure du pilote de routage / Routing driver structure
const struct routing_driver d_seran_routing_driver = {
  "d-seran",
  NULL, NULL, NULL, NULL, NULL, NULL
}; 