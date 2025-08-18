// mobility.c : Gestion de la mobilité pour MANET (RWP, Gauss-Markov)
#include "contiki.h"
#include "lib/random.h"
#include <stdio.h>
#include <math.h>

// Structures pour la position, la vitesse, etc.
typedef struct {
  float x, y;
  float speed;
  float direction;
} node_mobility_t;

static node_mobility_t my_mobility;

void mobility_init(void) {
  // Initialiser la position et la mobilité selon le modèle choisi
  my_mobility.x = random_rand() % 100;
  my_mobility.y = random_rand() % 100;
  my_mobility.speed = 1.0 + (random_rand() % 5);
  my_mobility.direction = random_rand() % 360;
}

void mobility_update(void) {
  // Mettre à jour la position selon RWP (simplifié)
  my_mobility.x += my_mobility.speed * cos(my_mobility.direction);
  my_mobility.y += my_mobility.speed * sin(my_mobility.direction);
  if(my_mobility.x < 0) my_mobility.x = 0;
  if(my_mobility.y < 0) my_mobility.y = 0;
  if(my_mobility.x > 100) my_mobility.x = 100;
  if(my_mobility.y > 100) my_mobility.y = 100;
  // Notifier D-SERAN
  extern void notify_d_seran_of_movement(void);
  notify_d_seran_of_movement();
}

// Hooks pour interaction avec D-SERAN
// extern void notify_d_seran_of_movement(void); 