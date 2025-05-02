#include "contiki.h"
#include "lib/sensors.h"
#include "dev/energy.h"
#include <stdio.h>
#include <string.h>

#ifndef BATTERY_CAPACITY
#define BATTERY_CAPACITY 8800 // mJ
#endif

// Seuils d'alerte en pourcentage
#define ALERT_THRESHOLD_1 20
#define ALERT_THRESHOLD_2 10
#define ALERT_THRESHOLD_3 5

static unsigned long last_cpu, last_lpm, last_transmit, last_listen;
static double remaining_energy = BATTERY_CAPACITY;
static int lifetime_printed = 0;
static int alert_1_printed = 0;
static int alert_2_printed = 0;
static int alert_3_printed = 0;
static clock_time_t start_time;
static FILE *log_file;

void init_lifetime_logging() {
    char filename[50];
    snprintf(filename, sizeof(filename), "lifetime_log_%d.txt", node_id);
    log_file = fopen(filename, "w");
    if(log_file != NULL) {
        fprintf(log_file, "Time(s),Energy(mJ),Percentage(%%),CPU,LPM,TX,RX\n");
        start_time = clock_time();
    }
}

void update_lifetime() {
    unsigned long cpu, lpm, transmit, listen;
    unsigned long delta_cpu, delta_lpm, delta_transmit, delta_listen;
    double energy_used = 0;

    // Valeurs d'énergie depuis le boot
    cpu = energest_type_time(ENERGEST_TYPE_CPU);
    lpm = energest_type_time(ENERGEST_TYPE_LPM);
    transmit = energest_type_time(ENERGEST_TYPE_TRANSMIT);
    listen = energest_type_time(ENERGEST_TYPE_LISTEN);

    // Deltas
    delta_cpu = cpu - last_cpu;
    delta_lpm = lpm - last_lpm;
    delta_transmit = transmit - last_transmit;
    delta_listen = listen - last_listen;

    last_cpu = cpu;
    last_lpm = lpm;
    last_transmit = transmit;
    last_listen = listen;

    // Consommation énergétique en mJ
    energy_used += (delta_cpu * 0.5) / RTIMER_SECOND;     // CPU
    energy_used += (delta_lpm * 0.0005) / RTIMER_SECOND;  // LPM
    energy_used += (delta_transmit * 17.4) / RTIMER_SECOND; // TX
    energy_used += (delta_listen * 18.8) / RTIMER_SECOND;  // RX

    // Mise à jour de l'énergie restante
    remaining_energy -= energy_used;

    // Calcul du pourcentage
    double percentage = (remaining_energy / BATTERY_CAPACITY) * 100;

    // Enregistrement des données
    if(log_file != NULL) {
        clock_time_t current_time = clock_time();
        double elapsed_time = (double)(current_time - start_time) / CLOCK_SECOND;
        fprintf(log_file, "%.2f,%.2f,%.1f,%lu,%lu,%lu,%lu\n",
                elapsed_time, remaining_energy, percentage,
                delta_cpu, delta_lpm, delta_transmit, delta_listen);
        fflush(log_file);
    }

    if(remaining_energy <= 0 && !lifetime_printed) {
        printf("Batterie epuisee ! Node lifetime reached.\n");
        if(log_file != NULL) {
            fprintf(log_file, "Node lifetime: %.2f seconds\n", 
                    (double)(clock_time() - start_time) / CLOCK_SECOND);
            fclose(log_file);
        }
        lifetime_printed = 1;
    } else if (!lifetime_printed) {
        printf("Energie restante : %.2f mJ (%.1f%%)\n", remaining_energy, percentage);
        
        // Affichage des alertes aux seuils
        if(percentage <= ALERT_THRESHOLD_1 && !alert_1_printed) {
            printf("Alerte : Batterie a %.1f%% !\n", percentage);
            alert_1_printed = 1;
        }
        if(percentage <= ALERT_THRESHOLD_2 && !alert_2_printed) {
            printf("Alerte critique : Batterie a %.1f%% !\n", percentage);
            alert_2_printed = 1;
        }
        if(percentage <= ALERT_THRESHOLD_3 && !alert_3_printed) {
            printf("Alerte urgente : Batterie presque epuisee (%.1f%%) !\n", percentage);
            alert_3_printed = 1;
        }
    }
}
