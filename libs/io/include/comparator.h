#ifndef comparator_h
#define comparator_h
#include "types.h"

// scrive su file la tabella di confronto tra le due politiche.
// ritorna 0 in caso di successo, -1 se errore (path non apribile o confronto NULL)
int comparison_write(const char *path, const confronto_t *confronto);

#endif