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