#ifndef metrics_h
#define metrics_h
#include "types.h"

// azzera tutti i contatori e i campi della struct metriche_t prima di inizio simulazione; ritona 0 se tutto ok, -1 se errore (es. malloc fallita)
int metrics_init(int n_pezzi_csv);
// libera eventuali risorse allocate per la struct metriche_t, al momento non serve, lascio commentata per simmetria
void metrics_destroy(void);
const metriche_t *metrics_get(void);
// aggiorna i contatori quando un pezzo viene completato con esito OK; usa lead time per aggiornare somma cumulativa
void record_piece_done(const pezzo *p);
// aggiorna i contatori quando un pezzo viene scartato
void record_piece_scrap (const pezzo *p);
// aggiorna i contatori quando un pezzo viene completato in ritardo
void record_piece_late (const pezzo *p);
//incrementa il contatore dei blocchi causati da buffer pieno
void record_block (void);
// incrementa il contatore degli errori di simulazione 
void record_error (void);
#endif 