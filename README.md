# D-SERAN: Distributed Self-Healing Energy-Aware Routing for Ad Hoc Networks

## Description
This project provides the complete implementation of the **D-SERAN** protocol (*Distributed Self-Healing Energy-Aware Routing for Ad Hoc Networks*), developed and evaluated in the corresponding scientific article.

---

## Project Structure

```
D-SERAN_source_code/
├── src/                          # Main source code
│   ├── d-seran.c                # D-SERAN protocol implementation
│   ├── mobility.c               # Node mobility management
│   ├── aodv-demo.c              # AODV demonstration
│   ├── dsr-demo.c               # DSR demonstration
│   ├── olsr-demo.c              # OLSR demonstration
│   ├── project-conf.h           # Project configuration
│   ├── Makefile                 # Main Makefile
│   └── Makefile.*               # Specific Makefiles
├── src_dseran/                   # Alternative source code version
├── figure-fin-2/                 # Figures used in the article
├── scripts/                      # Data generation and analysis scripts
├── data_real/                    # Raw simulation data
├── data/                         # Processed data for figures
├── simulations/                  # Cooja simulation files
├── results/                      # Simulation logs and results
└── article/                      # Scientific article
```

---

## Installation and Usage

### Prerequisites
- **Contiki-NG** installed and configured  
- **Python 3.7+** with required packages  
- **Cooja** (Contiki-NG simulator)  

### Compilation
```bash
cd src
make TARGET=cooja
```

### Running simulations
```bash
cd scripts
./run_simulations.sh
```

### Generating figures
```bash
cd scripts
python3 plot_comparisons.py
```

---

## Data and Results

### Data
The CSV files in `data_real/` contain the raw results:  
- **PDR**: Packet Delivery Ratio  
- **Energy**: Energy consumption  
- **Latency**: Transmission delay  
- **Throughput**: Data transfer rate  
- **Detection**: Attack detection accuracy  

### Figures
The figures in `figure-fin-2/` are automatically generated from CSV data and used in the scientific article.  

---

## Reproducibility

This project is designed to be fully reproducible. All required data, scripts, and configurations are included to:  
1. Compile the D-SERAN protocol  
2. Run the simulations  
3. Generate the figures  
4. Reproduce the article results  

---

## Reference

For more details about the D-SERAN protocol, please refer to the scientific article in the `article/` folder.  

---

## Support

For questions or issues, please check the logs in the `results/` folder and the comments in the source code.  
