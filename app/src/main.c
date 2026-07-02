#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "parsing.h"
#include "cella.h"
#include "controllore.h"

int main(void) {
    printf("=== AVVIO SIMULATORE CELLA MECCATRONICA ===\n\n");

    // 1. Parsing dei parametri di simulazione (prova sia .csv che .txt)
    parametri_simulazione params;
    int param_res = parse_parametri("input/simulation_parameters.csv", &params);
    if (param_res != 0) {
        param_res = parse_parametri("input/simulation_parameters.txt", &params);
    }
    
    if (param_res == 0) {
        printf("--- Parametri di Configurazione della Cella ---\n");
        printf("  - max_area_pressa: %.2f\n", params.max_area_pressa);
        printf("  - capacita_buffer_laminazione: %d\n", params.capacita_buffer_laminazione);
        printf("  - capacita_buffer_pressa: %d\n", params.capacita_buffer_pressa);
        printf("  - capacita_buffer_gom: %d\n", params.capacita_buffer_gom);
        printf("  - politica_controllo: %s\n", params.politica_controllo);
        printf("  - temperatura_ambiente_iniziale: %.2f\n", params.temperatura_ambiente_iniziale);
        printf("  - temperatura_incremento_minuto: %.4f\n", params.temperatura_incremento_minuto);
        printf("  - durata_simulazione_max: %d\n\n", params.durata_simulazione_max);
    } else {
        printf("Errore nel parsing di simulation_parameters!\n\n");
        return 1;
    }

    // 2. Parsing del catalogo
    catalogo_entry *catalogo = NULL;
    int num_cat = parse_catalogo("input/catalogo.csv", &catalogo);
    if (num_cat > 0) {
        printf("--- Catalogo Pezzi (%d elementi) ---\n", num_cat);
        for (int i = 0; i < num_cat; i++) {
            printf("  - Tipo %c: tempo_lam=%d, ciclo_termico=%d, dev_max=%.2f, area=%.2f\n", 
                   catalogo[i].tipo, catalogo[i].tempo_laminazione, 
                   catalogo[i].ciclo_termico, catalogo[i].deviazione_max, 
                   catalogo[i].area_stampo);
        }
        printf("\n");
    } else {
        printf("Errore nel parsing di catalogo.csv o catalogo vuoto!\n\n");
        return 1;
    }

    // 3. Parsing degli ordini
    ordine_entry *ordini = NULL;
    int num_ordini = parse_ordini("input/ordini.csv", &ordini);
    if (num_ordini > 0) {
        printf("--- Ordini di Produzione (%d ordini) ---\n", num_ordini);
        for (int i = 0; i < num_ordini; i++) {
            printf("  - Ordine ID %d: tipo=%c, quantita=%d, priorità=%d, tempo_completamento_max=%d\n", 
                   ordini[i].id, ordini[i].tipo, ordini[i].quantita, 
                   ordini[i].priorita, ordini[i].tempo_completamento_massimo);
        }
        printf("\n");
    } else {
        printf("Errore nel parsing di ordini.csv o nessun ordine presente!\n\n");
        free(catalogo);
        return 1;
    }

    // 4. Creazione dell'array valori_nominali (catalogo statico per la simulazione)
    valori_nominali *catalogo_nominale = malloc(num_cat * sizeof(valori_nominali));
    if (catalogo_nominale == NULL) {
        perror("Errore malloc catalogo_nominale");
        free(catalogo);
        free(ordini);
        return 1;
    }
    for (int i = 0; i < num_cat; i++) {
        catalogo_nominale[i].t_laminazione_nominale = catalogo[i].tempo_laminazione;
        catalogo_nominale[i].t_pressa_nominale = catalogo[i].ciclo_termico;
        catalogo_nominale[i].t_gom = 10; // Impostiamo un valore nominale di default per il controllo GOM
        catalogo_nominale[i].deviazione_max_gom = (int)(catalogo[i].deviazione_max * 100); // convertito per tipo int
    }

    // Inizializza la cella meccatronica
    cella_meccatronica cella = {0};
    cella.tick_corrente = 0;
    cella.tick_fine_simulazione = params.durata_simulazione_max;

    // 5. Creazione della lista dei pezzi partendo dagli ordini
    pezzo *head = NULL;
    pezzo *tail = NULL;
    int id_pezzo_counter = 1;

    for (int i = 0; i < num_ordini; i++) {
        // Cerca la corrispondenza del tipo nel catalogo nominale
        valori_nominali *val_nom = NULL;
        for (int j = 0; j < num_cat; j++) {
            if (catalogo[j].tipo == ordini[i].tipo) {
                val_nom = &catalogo_nominale[j];
                break;
            }
        }

        // Genera Q pezzi per questo ordine
        for (int q = 0; q < ordini[i].quantita; q++) {
            pezzo *p = malloc(sizeof(pezzo));
            if (p == NULL) {
                perror("Errore malloc pezzo");
                return 1;
            }
            p->id_pezzo = id_pezzo_counter++;
            p->ID_ordine = ordini[i].id;
            p->priorità = ordini[i].priorita;
            p->deadline_ticks = ordini[i].tempo_completamento_massimo;
            p->valori_nom = val_nom;
            p->stato = CREATED;

            p->ts.ingresso = 0;
            p->ts.inizio_laminazione = 0;
            p->ts.inizio_pressa = 0;
            p->ts.inizio_gom = 0;
            p->ts.uscita = 0;
            p->tempo_laminazione_effettivo = 0;
            p->tempo_pressa_effettivo = 0;
            p->lead_time = 0;
            p->deviazione_gom = 0.0f;
            p->next = NULL;

            if (head == NULL) {
                head = p;
                tail = p;
            } else {
                tail->next = p;
                tail = p;
            }
        }
    }
    cella.pezzi_head = head;

    printf("--- Pezzi Generati dagli Ordini (%d pezzi in totale) ---\n", id_pezzo_counter - 1);
    pezzo *tmp = head;
    while (tmp != NULL) {
        printf("  - Pezzo ID %d: da Ordine %d, priorità=%d, deadline=%d\n", 
               tmp->id_pezzo, tmp->ID_ordine, tmp->priorità, tmp->deadline_ticks);
        tmp = tmp->next;
    }
    printf("\n");

    // 6. Inizializziamo lo stato di test delle stazioni usando i primi 3 pezzi generati
    pezzo *curr = head;
    if (curr != NULL) {
        cella.laminazione.stato = BUSY;
        cella.laminazione.tick_lavorazione_rimasti = curr->valori_nom->t_laminazione_nominale;
        cella.laminazione.pezzo_in_lavorazione = curr;
        curr->stato = IN_LAMINATION;
        curr = curr->next;
    }
    
    if (curr != NULL) {
        cella.pressa.stato = BUSY;
        cella.pressa.tick_lavorazione_rimasti = curr->valori_nom->t_pressa_nominale;
        cella.pressa.pezzo_in_lavorazione = curr;
        curr->stato = IN_PRESS;
        curr = curr->next;
    }
    
    if (curr != NULL) {
        cella.gom.stato = BROKEN;
        cella.gom.tick_lavorazione_rimasti = 4; // lasciamo 4 tick e lo stato BROKEN per testare la pausa
        cella.gom.pezzo_in_lavorazione = curr;
        curr->stato = IN_GOM;
        curr = curr->next;
    }

    cella.agv.stato = IDLE;
    cella.agv.tick_lavorazione_rimasti = 0;
    cella.agv.pezzo_in_lavorazione = NULL;

    printf("=== AVVIO SIMULAZIONE DI PROVA DEI TICK ===\n");
    
    // Ciclo temporale discreto della simulazione
    while (cella.tick_corrente < cella.tick_fine_simulazione) {
        cella.tick_corrente++;
        controllore_tick(&cella);
    }

    printf("=== SIMULAZIONE COMPLETATA CON SUCCESSO ===\n");

    // Pulizia e rilascio di tutta la memoria dinamica
    pezzo *p_curr = cella.pezzi_head;
    while (p_curr != NULL) {
        pezzo *p_next = p_curr->next;
        free(p_curr);
        p_curr = p_next;
    }
    free(catalogo_nominale);
    free(catalogo);
    free(ordini);

    return 0;
}
