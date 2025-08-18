#!/bin/bash
# Script pour lancer les simulations D-SERAN et AODV en mode batch (nogui)
# Script to run D-SERAN and AODV simulations in batch mode (nogui)
#
# Auteur / Author: Madani Belacel
# Date de création / Created: 12/06/2023
# Dernière mise à jour / Last updated: 25/07/2023
# 
# Lancement automatique des simulations de comparaison des protocoles
# Automatic launch of protocol comparison simulations
# 
# Utilise Cooja en mode sans interface graphique pour l'exécution en lot
# Uses Cooja in headless mode for batch execution

set -e

# Configuration des chemins / Path configuration
COOJA_JAR=/home/belacel/contiki-ng1/tools/cooja/dist/cooja.jar
SIMDIR=$(dirname "$0")/..

# Vérification de l'existence de Cooja / Check Cooja existence
if [ ! -f "$COOJA_JAR" ]; then
    echo "ERREUR: Fichier Cooja non trouvé: $COOJA_JAR"
    echo "ERROR: Cooja file not found: $COOJA_JAR"
    exit 1
fi

echo "=========================================="
echo "DÉMARRAGE DES SIMULATIONS / SIMULATION START"
echo "=========================================="
echo "Répertoire de simulation: $SIMDIR"
echo "Simulation directory: $SIMDIR"
echo "JAR Cooja: $COOJA_JAR"
echo "Cooja JAR: $COOJA_JAR"
echo ""

# Création du répertoire de résultats / Create results directory
mkdir -p "$SIMDIR/results"

# Lancement de la simulation D-SERAN / Launch D-SERAN simulation
echo "[INFO] Lancement de la simulation D-SERAN..."
echo "[INFO] Launching D-SERAN simulation..."
start_time=$(date +%s)

java -jar "$COOJA_JAR" -nogui "$SIMDIR/simulations/d-seran.csc" > "$SIMDIR/results/d-seran.log" 2>&1

end_time=$(date +%s)
duration=$((end_time - start_time))
echo "[INFO] Simulation D-SERAN terminée en ${duration} secondes"
echo "[INFO] D-SERAN simulation completed in ${duration} seconds"

# Lancement de la simulation AODV / Launch AODV simulation
echo "[INFO] Lancement de la simulation AODV..."
echo "[INFO] Launching AODV simulation..."
start_time=$(date +%s)

java -jar "$COOJA_JAR" -nogui "$SIMDIR/simulations/aodv.csc" > "$SIMDIR/results/aodv.log" 2>&1

end_time=$(date +%s)
duration=$((end_time - start_time))
echo "[INFO] Simulation AODV terminée en ${duration} secondes"
echo "[INFO] AODV simulation completed in ${duration} seconds"

# Affichage des résultats / Display results
echo ""
echo "=========================================="
echo "SIMULATIONS TERMINÉES / SIMULATIONS COMPLETED"
echo "=========================================="
echo "Logs sauvegardés dans: $SIMDIR/results/"
echo "Logs saved in: $SIMDIR/results/"
echo "  - d-seran.log (simulation D-SERAN)"
echo "  - aodv.log (simulation AODV)"
echo ""

# Vérification de la taille des logs / Check log file sizes
for logfile in "$SIMDIR/results"/*.log; do
    if [ -f "$logfile" ]; then
        size=$(wc -l < "$logfile")
        echo "  $(basename "$logfile"): $size lignes / lines"
    fi
done

echo ""
echo "Analyse des logs disponible avec: python3 parse_logs.py"
echo "Log analysis available with: python3 parse_logs.py" 