# Makefile principal pour le projet D-SERAN
# Main Makefile for D-SERAN project
#
# Auteur / Author: Madani Belacel
# Date de création / Created: 12/02/2023
# Dernière mise à jour / Last updated: 15/03/2023
# 
# Configuration de compilation pour le protocole D-SERAN
# Compilation configuration for D-SERAN protocol

CONTIKI_PROJECT = d-seran
all: $(CONTIKI_PROJECT)

# Chemin vers Contiki-NG / Path to Contiki-NG
CONTIKI = ../../../

# Fichiers source du projet / Project source files
PROJECT_SOURCEFILES += d-seran.c mobility.c
PROJECT_CONF_PATH = ./

# Modules Contiki-NG requis / Required Contiki-NG modules
MODULES += core/net/ipv6 core/net/ipv6/uip-nd6 core/net/ipv6/uip-ds6 \
           core/net/ipv6/uip-icmp6 core/net/ipv6/uip-udp

# Bibliothèques système / System libraries
LDLIBS += -lm

# Inclusion du Makefile principal Contiki-NG / Include main Contiki-NG Makefile
include $(CONTIKI)/Makefile.include 