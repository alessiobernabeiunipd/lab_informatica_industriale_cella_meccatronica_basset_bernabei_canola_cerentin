#include "comparator.h"
#include <stdio.h>

// traduce l'esito del confronto nel nome della politica vincente (o "-" se pari).
// in stats decido che vince e registro, qui mostro
static const char *nome_vincitore(const confronto_t *c, vincitore_t v){
    switch (v){
        case VINCE_A: return c->nome_a;
        case VINCE_B: return c->nome_b;
        default:      return "-"; // PARI
    }
}

// scrive su file la tabella di confronto tra le due politiche.
// ritorna 0 in caso di successo, -1 se errore
int comparison_write(const char *path, const confronto_t *confronto){
    if (confronto == NULL){
        fprintf(stderr, "Errore: confronto non valido (NULL)\n");
        return -1;
    }
    FILE *file = fopen(path, "w");
    if (file == NULL){
        fprintf(stderr, "Errore nell'apertura del file di confronto: %s\n", path);
        return -1;
    }

    // intestazione
    fprintf(file, "================================================================\n");
    fprintf(file, "  CONFRONTO POLITICHE: %s vs %s\n", confronto->nome_a, confronto->nome_b);
    fprintf(file, "================================================================\n\n");

    // intestazione della tabella: le due colonne dei valori prendono il nome delle politiche
    fprintf(file, "%-24s%-11s%-11s%-11s%s\n",
            "Metrica", confronto->nome_a, confronto->nome_b, "Delta", "Migliore");
    fprintf(file, "------------------------------------------------------------------\n");

    // una riga per ogni metrica confrontata: io non sa quali siano, cicla e basta
    for (int i = 0; i < confronto->n_righe; i++){
        const riga_confronto_t *r = &confronto->righe[i];
        fprintf(file, "%-24s%-11.2f%-11.2f%+-11.2f%s\n",
                r->nome_metrica,
                r->valore_a,
                r->valore_b,
                r->delta,
                nome_vincitore(confronto, r->vincitore));
    }
    fprintf(file, "\n");

    fclose(file);
    return 0;
}
