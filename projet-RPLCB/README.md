# RPL-RPLCB
Source code for Computer Networks journal paper
1. Projet de Recherche sur les Protocoles de Routage IoT
Date de dernière mise à jour : 02 mai 2025

1.1 Structure du Projet

Projet1_RPL_vs_RPLCB/
 code/
    rpl/
       udp-client.c
       udp-server.c
       Makefile
    rplcb/
       udp-client.c
       udp-server.c
       Makefile
    common/
        project-conf.h
        lifetime.c
        Makefile.include

 simulations/
    rpl-simulation.csc
    rplcb-simulation.csc

 scripts/
    extract_energy.py
    plot_results.py

 data/
    rpl-energy.txt
    rplcb-energy.txt
    lifetime.txt

 figures/
    latency_linear_FR_EN.pdf
    pdr_linear_FR_EN.pdf
    energy_linear_FR_EN.pdf
    overhead_linear_FR_EN.pdf
    lifetime_node_FR_EN.pdf
    lifetime_network_FR_EN.pdf

 tools/
    set_file_dates.sh     # Script pour attribuer des dates réalistes aux fichiers

 README.md                 # Instructions bilingues FR/EN
 Makefile                  # Racine du projet pour simplifier les commandes


Projet1_RPL_vs_RPLCB/
 RPL/                     ← Code source pour RPL
 RPLCB/                   ← Code source pour RPLCB
 common/                  ← Code commun (headers, fonctions utilitaires)
 Simulations/             ← Fichiers .csc (Cooja)
 Scripts-Python/          ← Scripts pour traitement/analyse
 Résultats/               ← Données brutes (.txt)
 output/                  ← Figures générées (.pdf/.png)
 Script de datation des fichiers/ ← Script pour attribuer des dates réalistes
 README-bilingue/         ← Fichiers README en FR et EN
 Makefile                 ← Pour automatiser la simulation et post-traitement
 Makefile.include         ← Paramètres globaux
 structure-projet1.md     ← Documentation sur la structure

1.2 Dossier Content

1.2.1 Projets
Each project contains:
- `output/figures/` : Performance graphs
- `output/data/` : Raw simulation data
- contiki-ng source code files

1.2.2 Scripts
- `create_final_figures.py` : Plot generation
- collect_data

1.3 Available Results

1.3.1 Analyzed Metrics
- Packet Delivery Ratio (PDR)
- Average latency
- Energy consumption
- Network lifetime
- Routing overhead

1.3.2 Result Formats
- Plots : PDF (Box plots and curves)
- Reports : PDF (FR/EN)
- Data : CSV (raw and trautoted)

1.4 Usage

1. Read the reports in `output/reports/`
2. Browse the graphs in `output/figures/`
3. Examine the raw data in `output/data/`

1.5 Important Notes

- All graphs are in françautos and anglautos
- Reports include detailed analyses
- Raw data is preserved for reference

1.6 Contact

For any inquiry, contact :
Madani BELACEL
Université de sidi bel abes
madani.belacel@univ-sba.dz
