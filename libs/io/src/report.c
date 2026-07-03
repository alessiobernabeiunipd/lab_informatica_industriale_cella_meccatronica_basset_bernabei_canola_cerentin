#include "report.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

// scrive il report formattato su uno stream generico (file o stdout)
static void report_dump(FILE *out, const char *nome_politica, const metriche_t *m){
    // intestazione
    fprintf(out, "================================================================\n");
    fprintf(out, "  REPORT STATISTICO FINALE (POLITICA: %s)\n", nome_politica);
    fprintf(out, "================================================================\n\n");

    // produzione
    fprintf(out, "--- PRODUZIONE ---\n");
    fprintf(out, "%-30s%d\n",    "Pezzi totali:",        m->pezzi_totali);
    fprintf(out, "%-30s%d\n",    "Pezzi completati:",    m->pezzi_completati);
    fprintf(out, "%-30s%d\n",    "Pezzi scartati:",      m->pezzi_scartati);
    fprintf(out, "%-30s%d\n",    "Pezzi in ritardo:",    m->pezzi_in_ritardo);
    fprintf(out, "%-30s%d\n",    "Rilavorazioni:",       m->rilavorazioni);
    fprintf(out, "%-30s%.2f%%\n","Tasso scarto:",        m->tasso_scarto * 100);
    fprintf(out, "\n");

    // tempi
    fprintf(out, "--- TEMPI (tick) ---\n");
    fprintf(out, "%-30s%.2f\n", "Lead time medio:",         m->lead_time_medio);
    fprintf(out, "%-30s%.2f\n", "Tempo laminazione medio:", m->tempo_laminazione_medio);
    fprintf(out, "%-30s%.2f\n", "Tempo pressa medio:",      m->tempo_pressa_medio);
    fprintf(out, "%-30s%.2f\n", "Tempo GOM medio:",         m->tempo_gom_medio);
    fprintf(out, "%-30s%.2f\n", "Deviazione GOM media:",    m->deviazione_gom_medio);
    fprintf(out, "\n");

    // eventi
    fprintf(out, "--- EVENTI ---\n");
    fprintf(out, "%-30s%d\n", "Blocchi buffer pieno:",  m->blocchi_buffer_pieno);
    fprintf(out, "%-30s%d\n", "Errori simulazione:",    m->errori_simulazione);
    fprintf(out, "\n");
}

int report_write(const char *report_file_path, const char *nome_politica, const metriche_t *m){
    FILE *report_file = fopen(report_file_path, "w");
    if (report_file == NULL){
        fprintf(stderr, "Errore nell'apertura del file di report: %s\n", report_file_path);
        return -1;
    }

    report_dump(report_file, nome_politica, m);

    fclose(report_file);
    return 0;
}

void report_print(const char *nome_politica, const metriche_t *m){
    report_dump(stdout, nome_politica, m);
}
