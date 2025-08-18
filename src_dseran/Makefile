# Makefile alternatif pour le projet D-SERAN
# Alternative Makefile for D-SERAN project
#
# Auteur / Author: Madani Belacel
# Date de création / Created: 15/04/2023
# Dernière mise à jour / Last updated: 20/05/2023
# 
# Version alternative de compilation pour D-SERAN
# Alternative compilation version for D-SERAN

CONTIKI_PROJECT = d-seran-alt
all: $(CONTIKI_PROJECT)

# Chemin vers Contiki-NG / Path to Contiki-NG
CONTIKI = ../../../

# Fichiers source alternatifs / Alternative source files
PROJECT_SOURCEFILES += d-seran.c mobility.c
PROJECT_CONF_PATH = ./

# Modules réseau requis / Required network modules
MODULES += core/net/ipv6 core/net/ipv6/uip-nd6 core/net/ipv6/uip-ds6 \
           core/net/ipv6/uip-icmp6 core/net/ipv6/uip-udp

# Bibliothèques système / System libraries
LDLIBS += -lm

# Inclusion du Makefile principal / Include main Makefile
include $(CONTIKI)/Makefile.include 