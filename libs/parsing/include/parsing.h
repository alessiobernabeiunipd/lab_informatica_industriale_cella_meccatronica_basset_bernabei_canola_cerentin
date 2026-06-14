#ifndef PARSING_H
#define PARSING_H

#include "types.h"

// Struttura per memorizzare una riga del catalogo
typedef struct {
    char tipo;
    int tempo_laminazione;
    int ciclo_termico;
    float deviazione_max;
    float area_stampo;
} catalogo_entry;

// Struttura per memorizzare una riga degli ordini
typedef struct {
    int id;
    char tipo;
    int quantita;
    int priorita;
    int tempo_completamento_massimo;
} ordine_entry;

// Struttura per memorizzare i parametri generali di simulazione
typedef struct {
    float max_area_pressa;
    int capacita_buffer_laminazione;
    int capacita_buffer_pressa;
    int capacita_buffer_gom;
    char politica_controllo[32];
    float temperatura_ambiente_iniziale;
    float temperatura_incremento_minuto;
    int durata_simulazione_max;
} parametri_simulazione;

// Ritorna il numero di elementi letti, alloca l'array di output in *entries. Ritorna -1 in caso di errore.
int parse_catalogo(const char *filepath, catalogo_entry **entries);

// Ritorna il numero di elementi letti, alloca l'array di output in *entries. Ritorna -1 in caso di errore.
int parse_ordini(const char *filepath, ordine_entry **entries);

// Ritorna 0 in caso di successo, -1 in caso di errore. Popola dest.
int parse_parametri(const char *filepath, parametri_simulazione *dest);

#endif // PARSING_H