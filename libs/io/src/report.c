#include "report.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

int report_write(const char *report_file_path, const char *nome_politica, const metriche_t *m){
    FILE *report_file = fopen(report_file_path, "w");
    if (report_file == NULL){
        fprintf(stderr, "Errore nell'apertura del file di report: %s\n", report_file_path);
        return -1;
    }

    // intestazione
    fprintf(report_file, "================================================================\n");
    fprintf(report_file, "  REPORT STATISTICO FINALE (POLITICA: %s)\n", nome_politica);
    fprintf(report_file, "================================================================\n\n");

    // produzione
    fprintf(report_file, "--- PRODUZIONE ---\n");
    fprintf(report_file, "%-30s%d\n",    "Pezzi totali:",        m->pezzi_totali);
    fprintf(report_file, "%-30s%d\n",    "Pezzi completati:",    m->pezzi_completati);
    fprintf(report_file, "%-30s%d\n",    "Pezzi scartati:",      m->pezzi_scartati);
    fprintf(report_file, "%-30s%d\n",    "Pezzi in ritardo:",    m->pezzi_in_ritardo);
    fprintf(report_file, "%-30s%d\n",    "Rilavorazioni:",       m->rilavorazioni);
    fprintf(report_file, "%-30s%.2f%%\n","Tasso scarto:",        m->tasso_scarto * 100);
    fprintf(report_file, "\n");

    // tempi
    fprintf(report_file, "--- TEMPI (tick) ---\n");
    fprintf(report_file, "%-30s%.2f\n", "Lead time medio:",         m->lead_time_medio);
    fprintf(report_file, "%-30s%.2f\n", "Tempo laminazione medio:", m->tempo_laminazione_medio);
    fprintf(report_file, "%-30s%.2f\n", "Tempo pressa medio:",      m->tempo_pressa_medio);
    fprintf(report_file, "%-30s%.2f\n", "Tempo GOM medio:",         m->tempo_gom_medio);
    fprintf(report_file, "%-30s%.2f\n", "Deviazione GOM media:",    m->deviazione_gom_medio);
    fprintf(report_file, "\n");

    // eventi
    fprintf(report_file, "--- EVENTI ---\n");
    fprintf(report_file, "%-30s%d\n", "Blocchi buffer pieno:",  m->blocchi_buffer_pieno);
    fprintf(report_file, "%-30s%d\n", "Errori simulazione:",    m->errori_simulazione);
    fprintf(report_file, "\n");

    fclose(report_file);
    return 0;
}