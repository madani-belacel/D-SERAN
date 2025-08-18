# D-SERAN: Distributed Self-Healing Energy-Aware Routing for Ad Hoc Networks

## Description
Ce projet contient l'implémentation complète du protocole D-SERAN (Distributed Self-Healing Energy-Aware Routing for Ad Hoc Networks) développé dans l'article scientifique correspondant.

This project contains the complete implementation of the D-SERAN protocol (Distributed Self-Healing Energy-Aware Routing for Ad Hoc Networks) developed in the corresponding scientific article.

## Structure du Projet / Project Structure

```
D-SERAN_source_code/
├── src/                          # Code source principal / Main source code
│   ├── d-seran.c                # Implémentation du protocole D-SERAN / D-SERAN protocol implementation
│   ├── mobility.c                # Gestion de la mobilité des nœuds / Node mobility management
│   ├── aodv-demo.c              # Démonstration AODV / AODV demonstration
│   ├── dsr-demo.c               # Démonstration DSR / DSR demonstration
│   ├── olsr-demo.c              # Démonstration OLSR / OLSR demonstration
│   ├── project-conf.h           # Configuration du projet / Project configuration
│   ├── Makefile                 # Fichier de compilation principal / Main compilation file
│   └── Makefile.*               # Fichiers de compilation spécifiques / Specific compilation files
├── src_dseran/                   # Version alternative du code source / Alternative source code version
│   ├── d-seran.c                # Code source D-SERAN / D-SERAN source code
│   ├── mobility.c                # Gestion de la mobilité / Mobility management
│   ├── project-conf.h           # Configuration / Configuration
│   └── Makefile                 # Compilation / Compilation
├── figure-fin-2/                 # Figures utilisées dans l'article / Figures used in the article
│   ├── fig_1_energy_en.png      # Consommation énergétique / Energy consumption
│   ├── fig_2_pdr_mobility_en.png # PDR vs mobilité / PDR vs mobility
│   ├── fig_3_attack_detection_en.png # Détection d'attaques / Attack detection
│   ├── fig_4_latency_en.png     # Latence / Latency
│   ├── fig_5_throughput_en.png  # Débit / Throughput
│   ├── fig_6_pdr_real_en.png    # PDR données simulées / Simulated data PDR
│   ├── fig_7_energy_real_en.png # Énergie données simulées / Simulated data energy
│   ├── fig_7_latency_en.png     # Latence données simulées / Simulated data latency
│   ├── fig_7_throughput_en.png  # Débit données simulées / Simulated data throughput
│   ├── fig_8_detection_real_en.png # Détection données simulées / Simulated data detection
│   └── fig_9_stability_en.png   # Stabilité / Stability
├── scripts/                      # Scripts de génération et analyse / Generation and analysis scripts
│   ├── generate_dseran_data.py  # Génération des données D-SERAN / D-SERAN data generation
│   ├── plot_comparisons.py      # Génération des graphiques / Plot generation
│   ├── plot_comparisons_complete.py # Graphiques complets / Complete plots
│   ├── parse_logs.py            # Analyse des logs / Log analysis
│   ├── run_simulations.sh       # Exécution des simulations / Simulation execution
│   ├── simple_build.py          # Compilation simplifiée / Simplified build
│   └── build_manuscript.py      # Construction du manuscrit / Manuscript building
├── data_real/                    # Données issues de la simulation / Simulation data
│   ├── dseran_*.csv             # Données D-SERAN / D-SERAN data
│   ├── aodv_*.csv               # Données AODV / AODV data
│   ├── dsr_*.csv                # Données DSR / DSR data
│   └── olsr_*.csv               # Données OLSR / OLSR data
├── data/                        # Données issues de la simulation pour les figures / Simulation data for figures
│   ├── dseran_*.csv             # Données de simulation D-SERAN / D-SERAN simulation data
│   └── ...                      # Autres données issues de la simulation / Other simulation data
├── simulations/                  # Fichiers de configuration Cooja / Cooja configuration files
│   ├── d-seran.csc              # Configuration D-SERAN / D-SERAN configuration
│   ├── aodv.csc                 # Configuration AODV / AODV configuration
│   ├── dsr.csc                  # Configuration DSR / DSR configuration
│   └── olsr.csc                 # Configuration OLSR / OLSR configuration
├── results/                      # Logs de simulation et résultats / Simulation logs and results
│   ├── d-seran.log              # Log principal D-SERAN / Main D-SERAN log
│   ├── aodv.log                 # Log principal AODV / Main AODV log
│   ├── d-seran_console.log      # Log console D-SERAN / D-SERAN console log
│   └── aodv_console.log         # Log console AODV / AODV console log
└── article/                      # Article scientifique / Scientific article
    ├── manuscript_Grok_4.tex    # Source LaTeX / LaTeX source
    └── manuscript_Grok_4.pdf    # PDF compilé / Compiled PDF
```

## Installation et Utilisation / Installation and Usage

### Prérequis / Prerequisites
- Contiki-NG installé et configuré / Contiki-NG installed and configured
- Python 3.7+ avec les packages requis / Python 3.7+ with required packages
- Cooja (simulateur Contiki-NG) / Cooja (Contiki-NG simulator)

### Compilation / Compilation
```bash
cd src
make TARGET=cooja
```

### Exécution des simulations / Running simulations
```bash
cd scripts
./run_simulations.sh
```

### Génération des figures / Generating figures
```bash
cd scripts
python3 plot_comparisons.py
```

## Données et Résultats / Data and Results

### Données issues de la simulation / Simulation data
Les fichiers CSV dans `data_real/` contiennent les résultats bruts issus de la simulation :
- **PDR** : Packet Delivery Ratio
- **Énergie** : Consommation énergétique / Energy consumption
- **Latence** : Délai de transmission / Transmission delay
- **Débit** : Throughput
- **Détection** : Précision de détection d'attaques / Attack detection accuracy

The CSV files in `data_real/` contain the raw results from the simulation:
- **PDR**: Packet Delivery Ratio
- **Energy**: Energy consumption
- **Latency**: Transmission delay
- **Throughput**: Data transfer rate
- **Detection**: Attack detection accuracy

### Figures / Figures
Les figures dans `figure-fin-2/` sont générées automatiquement à partir des données CSV et sont utilisées dans l'article scientifique.

The figures in `figure-fin-2/` are automatically generated from CSV data and are used in the scientific article.

## Reproducibilité / Reproducibility

Ce projet est conçu pour être entièrement reproductible. Toutes les données, scripts et configurations nécessaires sont inclus pour :
1. Compiler le protocole D-SERAN / Compile the D-SERAN protocol
2. Exécuter les simulations / Run simulations
3. Générer les figures / Generate figures
4. Reproduire les résultats de l'article / Reproduce the article results

This project is designed to be fully reproducible. All necessary data, scripts, and configurations are included to:
1. Compile the D-SERAN protocol
2. Run simulations
3. Generate figures
4. Reproduce the article results

## Référence / Reference

Pour plus de détails sur le protocole D-SERAN, consultez l'article scientifique dans le dossier `article/`.

For more details about the D-SERAN protocol, consult the scientific article in the `article/` folder.

## Support / Support

En cas de problème ou question, consultez d'abord les logs dans le dossier `results/` et les commentaires dans le code source.

In case of problems or questions, first consult the logs in the `results/` folder and the comments in the source code.
