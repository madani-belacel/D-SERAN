# D-SERAN (Distributed Self-Healing Energy-Aware Routing for Ad Hoc Networks)

Ce dossier contient le code source complet et prêt à l'emploi du protocole D-SERAN pour réseaux ad hoc mobiles (MANETs), incluant :
- Routage auto-réparateur
- Gestion de la confiance
- Prise en compte de l'énergie résiduelle et de la récolte d'énergie
- Support de la mobilité (Random Waypoint)

## Structure du code
- `d-seran.c` : Protocole principal (Contiki-NG)
- `project-conf.h` : Configuration du projet
- `mobility.c` : Gestion de la mobilité
- `lstm_adhoc.py` : Prédiction énergétique (optionnel)
- `Makefile` : Compilation sous Contiki-NG

## Compilation et simulation
1. Placez tous les fichiers dans `examples/AER_BELACEL/src/`.
2. Compilez avec :
   ```
   make -C src d-seran.cooja
   ```
3. Ouvrez Cooja, chargez le binaire généré, et utilisez le script d'exemple ci-dessous.

## Exemple de script de simulation Cooja (10 nœuds mobiles)
```cooja
// D-SERAN MANET demo (10 nœuds mobiles)
log.log("Simulation D-SERAN démarrée\n");
for(var i=0; i<10; i++) {
  var mote = sim.getMote(i);
  // Position initiale aléatoire
  mote.setPosition(20+Math.random()*100, 20+Math.random()*100, 0);
}
log.log("Topologie initialisée\n");
// Exemple de mobilité : déplacer les nœuds toutes les 30s
TIMEOUT(30000, function moveMotes() {
  for(var i=0; i<10; i++) {
    var mote = sim.getMote(i);
    mote.setPosition(20+Math.random()*100, 20+Math.random()*100, 0);
  }
  log.log("Mouvement des nœuds\n");
  TIMEOUT(30000, moveMotes);
});
```

## Références
Voir l'article associé pour la description complète du protocole et des expériences. 