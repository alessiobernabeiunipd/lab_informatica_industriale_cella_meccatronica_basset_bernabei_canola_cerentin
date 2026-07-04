#include <stdio.h>
#include "parsing.h"
#include "pezzo.h"
#include "logger.h"

int genera_pezzi_da_ordini(cella_meccatronica *cella, 
                           const ordine_entry *ordini, int num_ordini,
                           const catalogo_entry *catalogo, int num_cat) {
    int id_pezzo_counter = 1;
    for (int i = 0; i < num_ordini; i++) {
        // Cerca la corrispondenza del tipo di pezzo nel catalogo
        const catalogo_entry *cat_ref = NULL;
        for (int j = 0; j < num_cat; j++) {
            if (catalogo[j].tipo == ordini[i].tipo) {
                cat_ref = &catalogo[j];
                break;
            }
        }

        if (cat_ref == NULL) {
            log_warning_f(0, "Warning: Tipo di pezzo '%c' per l'ordine %d non trovato nel catalogo. Salto.", 
                          ordini[i].tipo, ordini[i].id);
            continue;
        }

        // Genera Q pezzi per questo ordine
        for (int q = 0; q < ordini[i].quantita; q++) {
            pezzo *p = new_pezzo();
            if (p == NULL) {
                log_error(0, "Errore: allocazione di memoria per un pezzo fallita.");
                // Deallochiamo la lista parziale per evitare leak
                pezzo *curr = cella->list_head;
                while (curr != NULL) {
                    pezzo *next = curr->next;
                    free(curr);
                    curr = next;
                }
                cella->list_head = NULL;
                return -1;
            }

            p->id_pezzo = id_pezzo_counter++;
            p->ID_ordine = ordini[i].id;
            p->priorità = ordini[i].priorita;
            p->deadline_ticks = ordini[i].tempo_completamento_massimo;
            
            // Popoliamo i valori nominali del pezzo dal catalogo
            p->valori_nom.t_laminazione_nominale = cat_ref->tempo_laminazione;
            p->valori_nom.t_pressa_nominale = cat_ref->ciclo_termico;
            p->valori_nom.t_gom = 10; // Valore di default nominale per il GOM
            p->valori_nom.deviazione_max_gom = (int)(cat_ref->deviazione_max * 100);

            p->stato = CREATED;

            // Aggiungiamo il pezzo in coda alla lista della cella
            add_pezzo(&(cella->list_head), p);
        }
    }
    return id_pezzo_counter - 1;
}

void print_pezzi(const pezzo *list_head) {
    printf("=== ELENCO DEI PEZZI DA PRODURRE ===\n");
    const pezzo *curr_print = list_head;
    while (curr_print != NULL) {
        printf("  Pezzo ID: %02d | Ordine ID: %d | Priorita: %d | Deadline: %4d tick | Lam Nom: %2d tick | Pres Nom: %2d tick | GOM Nom: %2d tick | Max Dev GOM: %.2f\n", 
               curr_print->id_pezzo, 
               curr_print->ID_ordine, 
               curr_print->priorità, 
               curr_print->deadline_ticks, 
               curr_print->valori_nom.t_laminazione_nominale, 
               curr_print->valori_nom.t_pressa_nominale, 
               curr_print->valori_nom.t_gom, 
               curr_print->valori_nom.deviazione_max_gom / 100.0f);
        curr_print = curr_print->next;
    }
    printf("====================================\n\n");
}
