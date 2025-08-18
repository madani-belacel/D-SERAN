/*
 * mobility.c : Gestion de la mobilité pour MANET (RWP, Gauss-Markov)
 * Mobility management for MANET (RWP, Gauss-Markov)
 * 
 * Auteur / Author: Madani Belacel
 * Date: Août 2025
 * 
 * Implémentation des modèles de mobilité pour simulations MANET
 * Implementation of mobility models for MANET simulations
 */

#include "contiki.h"
#include "lib/random.h"
#include <stdio.h>
#include <math.h>

// Structures pour la position, la vitesse, etc. / Structures for position, speed, etc.
typedef struct {
  float x, y;           // Coordonnées 2D / 2D coordinates
  float speed;          // Vitesse en unités/tour / Speed in units/turn
  float direction;      // Direction en degrés / Direction in degrees
} node_mobility_t;

static node_mobility_t my_mobility;

// Initialisation de la mobilité / Mobility initialization
void mobility_init(void) {
  // Initialiser la position et la mobilité selon le modèle choisi / Initialize position and mobility
  my_mobility.x = random_rand() % 100;
  my_mobility.y = random_rand() % 100;
  my_mobility.speed = 1.0 + (random_rand() % 5);
  my_mobility.direction = random_rand() % 360;
  
  // Affichage de l'état initial / Display initial state
  printf("Mobility: Position initiale: (%.1f, %.1f), vitesse: %.1f, direction: %.1f°\n",
         my_mobility.x, my_mobility.y, my_mobility.speed, my_mobility.direction);
}

// Mise à jour de la mobilité / Mobility update
void mobility_update(void) {
  // Sauvegarde de l'ancienne position / Save old position
  float old_x = my_mobility.x;
  float old_y = my_mobility.y;
  
  // Mettre à jour la position selon RWP (simplifié) / Update position according to RWP (simplified)
  my_mobility.x += my_mobility.speed * cos(my_mobility.direction * M_PI / 180.0);
  my_mobility.y += my_mobility.speed * sin(my_mobility.direction * M_PI / 180.0);
  
  // Gestion des limites de la zone / Boundary handling
  if(my_mobility.x < 0) {
    my_mobility.x = 0;
    printf("Mobility: Limite gauche atteinte, position ajustée\n");
  }
  if(my_mobility.y < 0) {
    my_mobility.y = 0;
    printf("Mobility: Limite basse atteinte, position ajustée\n");
  }
  if(my_mobility.x > 100) {
    my_mobility.x = 100;
    printf("Mobility: Limite droite atteinte, position ajustée\n");
  }
  if(my_mobility.y > 100) {
    my_mobility.y = 100;
    printf("Mobility: Limite haute atteinte, position ajustée\n");
  }
  
  // Calcul de la distance parcourue / Distance calculation
  float distance = sqrt((my_mobility.x - old_x) * (my_mobility.x - old_x) + 
                        (my_mobility.y - old_y) * (my_mobility.y - old_y));
  
  // Affichage occasionnel de la mobilité / Occasional mobility display
  static uint8_t update_count = 0;
  if(++update_count % 8 == 0) {
    printf("Mobility: Position: (%.1f, %.1f), déplacement: %.2f unités\n",
           my_mobility.x, my_mobility.y, distance);
  }
  
  // Notifier D-SERAN du mouvement / Notify D-SERAN of movement
  extern void notify_d_seran_of_movement(void);
  notify_d_seran_of_movement();
}

// Fonction pour obtenir la position actuelle / Get current position
void mobility_get_position(float *x, float *y) {
  *x = my_mobility.x;
  *y = my_mobility.y;
}

// Fonction pour définir manuellement la position / Manually set position
void mobility_set_position(float x, float y) {
  my_mobility.x = x;
  my_mobility.y = y;
  
  printf("Mobility: Position manuellement définie à (%.1f, %.1f)\n", x, y);
}

// Hooks pour interaction avec D-SERAN / Hooks for D-SERAN interaction
// extern void notify_d_seran_of_movement(void); 