#ifndef report_H
#define report_H
#include "types.h"


int report_write(const char *report_file_path, const char *nome_politica, const metriche_t *m);
void report_print(const char *nome_politica, const metriche_t *m);
int csv_write(const char *csv_file_path, const char *nome_politica, const metriche_t *m);

#endif