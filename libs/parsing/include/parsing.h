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
//spostato in types.h

// Ritorna il numero di elementi letti, alloca l'array di output in *entries. Ritorna -1 in caso di errore.
int parse_catalogo(const char *filepath, catalogo_entry **entries);

// Ritorna il numero di elementi letti, alloca l'array di output in *entries. Ritorna -1 in caso di errore.
int parse_ordini(const char *filepath, ordine_entry **entries);

// Ritorna 0 in caso di successo, -1 in caso di errore. Popola dest.
int parse_parametri(const char *filepath, parametri_simulazione *dest);

// Genera i pezzi dagli ordini attingendo ai valori nominali del catalogo.
// Ritorna il numero totale di pezzi generati in caso di successo, o -1 in caso di errore di allocazione.
int genera_pezzi_da_ordini(cella_meccatronica *cella, 
                           const ordine_entry *ordini, int num_ordini,
                           const catalogo_entry *catalogo, int num_cat);

// Stampa l'elenco dei pezzi da produrre senza prefissi di log
void print_pezzi(const pezzo *list_head);

#endif // PARSING_H