#ifndef metrics_h
#define metrics_h
#include "types.h"

// azzera tutti i contatori e i campi della struct metriche_t prima di inizio simulazione
void metrics_init(metriche_t *metrics);
// libera eventuali risorse allocate per la struct metriche_t, al momento non serve, lascio commentata per simmetria
//void metrics_destroy(metriche_t *metrics);
// aggiorna i contatori quando un pezzo viene completato con esito OK; usa lead time per aggiornare somma cumulativa
void record_piece_done(metriche_t *metrics, const pezzo *p);
// aggiorna i contatori quando un pezzo viene scartato
void record_piece_scrap (metriche_t *metrics, const pezzo *p);
// aggiorna i contatori quando un pezzo viene completato in ritardo
void record_piece_late (metriche_t *metrics, const pezzo *p);
//incrementa il contatore dei blocchi causati da buffer pieno
void record_block (metriche_t *metrics);
// incrementa il contatore degli errori di simulazione 
void record_error (metriche_t *metrics);
#endif 