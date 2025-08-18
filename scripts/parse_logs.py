#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
 * parse_logs.py : Script d'analyse des logs de simulation
 * Simulation log analysis script
 * 
 * Auteur / Author: Madani Belacel
 * Date de création / Created: 10/05/2023
 * Dernière mise à jour / Last updated: 18/06/2023
 * 
 * Script pour extraire et analyser les métriques des logs de simulation
 * Script to extract and analyze simulation log metrics
 * 
 * Utilise des expressions régulières pour parser les logs Cooja
 * Uses regular expressions to parse Cooja logs
"""

import re
import pandas as pd
import os
import sys
import time

# Vérification des arguments de ligne de commande / Command line argument checking
if len(sys.argv) >= 3:
    INPUT_DIR = sys.argv[1]
    OUTPUT_DIR = sys.argv[2]
else:
    INPUT_DIR = 'results'
    OUTPUT_DIR = 'results'

print(f"Parse Logs: Répertoire d'entrée: {INPUT_DIR}")
print(f"Parse Logs: Input directory: {INPUT_DIR}")
print(f"Parse Logs: Répertoire de sortie: {OUTPUT_DIR}")
print(f"Parse Logs: Output directory: {OUTPUT_DIR}")

# Configuration des fichiers de logs / Log files configuration
LOGS = {
    'dseran': os.path.join(INPUT_DIR, 'd-seran.log'),
    'aodv': os.path.join(INPUT_DIR, 'aodv.log')
}

# Expressions régulières pour extraire les métriques / Regular expressions for metric extraction
# Format: [INFO: MODULE] MSG
PATTERNS = {
    'energy': re.compile(r'ENERGY\s+(\d+)\s+(\d+)'),
    'send': re.compile(r'SEND_UDP\s+(\d+)\s+(\d+)'),
    'recv': re.compile(r'RECV\s+(\d+)\s+(\d+)'),
    'hop': re.compile(r'HOP\s+(\d+)\s+(\d+)'),
    'pdr': re.compile(r'PDR\s+(\d+)\s+(\d+)'),
    'loss': re.compile(r'LOSS\s+(\d+)\s+(\d+)'),
    'lifetime': re.compile(r'LIFETIME\s+(\d+)\s+(\d+)'),
    'throughput': re.compile(r'THROUGHPUT\s+(\d+)\s+(\d+)'),
    'mobility': re.compile(r'MOVE\s+(\d+)\s+(\d+)\s+(\d+)')
}

# Création du répertoire de sortie / Create output directory
os.makedirs(OUTPUT_DIR, exist_ok=True)

# Traitement de chaque protocole / Process each protocol
for proto, logfile in LOGS.items():
    print(f"Traitement du protocole: {proto.upper()}")
    print(f"Processing protocol: {proto.upper()}")
    
    # Initialisation des données / Data initialization
    data = {k: [] for k in PATTERNS}
    
    if not os.path.exists(logfile):
        print(f"  Fichier de log non trouvé: {logfile}")
        print(f"  Log file not found: {logfile}")
        continue
    
    # Lecture et analyse du fichier de log / Read and analyze log file
    with open(logfile, 'r', encoding='utf-8', errors='ignore') as f:
        line_count = 0
        for line in f:
            line_count += 1
            
            # Extraction de la partie message après le crochet / Extract message part after bracket
            if ']' in line:
                msg = line.split(']')[-1]
            else:
                msg = line
            
            # Recherche des motifs dans chaque ligne / Pattern search in each line
            for key, pat in PATTERNS.items():
                m = pat.search(msg)
                if m:
                    data[key].append(m.groups())
                    
                    # Traces de débogage occasionnelles / Occasional debug traces
                    if line_count % 1000 == 0:
                        print(f"    Ligne {line_count}: {key} trouvé - {m.groups()}")
                        print(f"    Line {line_count}: {key} found - {m.groups()}")
    
    print(f"  Total de lignes traitées: {line_count}")
    print(f"  Total simulation lines: {line_count}")
    
    # Sauvegarde des données extraites / Save extracted data
    for key, values in data.items():
        if values:
            df = pd.DataFrame(values)
            output_file = os.path.join(OUTPUT_DIR, f'{proto}_{key}.csv')
            df.to_csv(output_file, index=False, header=False)
            
            print(f"    {key}: {len(values)} valeurs sauvegardées dans {output_file}")
            print(f"    {key}: {len(values)} values saved in {output_file}")
        else:
            print(f"    {key}: Aucune donnée trouvée")
            print(f"    {key}: No data found")

print("\nAnalyse des logs terminée / Log analysis completed")
print(f"Résultats sauvegardés dans: {OUTPUT_DIR}")
print(f"Results saved in: {OUTPUT_DIR}") 