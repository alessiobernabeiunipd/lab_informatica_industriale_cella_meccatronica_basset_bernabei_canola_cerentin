#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsing.h"

// Rimuove spazi bianchi all'inizio e alla fine di una stringa
static void trim(char *str) {
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r' || str[start] == '\n') {
        start++;
    }
    if (start > 0) {
        int i = 0;
        while (str[start + i] != '\0') {
            str[i] = str[start + i];
            i++;
        }
        str[i] = '\0';
    }
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[len - 1] = '\0';
        len--;
    }
}

// Tenta di aprire il file. Se fallisce e il percorso inizia con "input/", 
// tenta di risalire le cartelle padre (es. "../input/..." o "../../input/...")
static FILE* open_file_probed(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (f != NULL) {
        return f; 
    }
    
    if (strncmp(filepath, "input/", 6) == 0) {
        char path[512];
        
        // Prova a cercare in "../input/..."
        snprintf(path, sizeof(path), "../%s", filepath);
        f = fopen(path, "r");
        if (f != NULL) {
            return f;
        }
        
        // Prova a cercare in "../../input/..."
        snprintf(path, sizeof(path), "../../%s", filepath);
        f = fopen(path, "r");
        if (f != NULL) {
            return f;
        }
    }
    
    return NULL;
}

// Conta quante righe di dati (non vuote e non commentate) ci sono nel file CSV dopo l'header
static int count_csv_entries(const char *filepath) {
    FILE *file = open_file_probed(filepath);
    if (file == NULL) {
        return 0;
    }
    
    char line[256];
    int count = 0;
    int is_header = 1;
    while (fgets(line, sizeof(line), file)) {
        trim(line);
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }
        if (is_header) { //salta la prima linea
            is_header = 0;
            continue;
        }
        count++;
    }
    fclose(file);
    return count;
}

int parse_catalogo(const char *filepath, catalogo_entry **entries) {
    int num_entries = count_csv_entries(filepath);
    if (num_entries <= 0) {
        *entries = NULL;
        return 0;
    }

    *entries = malloc(num_entries * sizeof(catalogo_entry));
    if (*entries == NULL) {
        perror("Errore malloc catalogo");
        return -1;
    }

    FILE *file = open_file_probed(filepath);
    if (file == NULL) {
        free(*entries);
        *entries = NULL;
        return -1;
    }

    char line[256];
    int index = 0;
    int is_header = 1;
    while (fgets(line, sizeof(line), file)) {
        trim(line);
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }
        if (is_header) {
            is_header = 0;
            continue;
        }

        char tipo;
        int tempo_lam;
        int ciclo_term;
        float dev_max;
        float area_stampo;

        if (sscanf(line, " %c , %d , %d , %f , %f", &tipo, &tempo_lam, &ciclo_term, &dev_max, &area_stampo) == 5) {
            (*entries)[index].tipo = tipo;
            (*entries)[index].tempo_laminazione = tempo_lam;
            (*entries)[index].ciclo_termico = ciclo_term;
            (*entries)[index].deviazione_max = dev_max;
            (*entries)[index].area_stampo = area_stampo;
            index++;
        }
    }

    fclose(file);
    return index;
}

int parse_ordini(const char *filepath, ordine_entry **entries) {
    int num_entries = count_csv_entries(filepath);
    if (num_entries <= 0) {
        *entries = NULL;
        return 0;
    }

    *entries = malloc(num_entries * sizeof(ordine_entry));
    if (*entries == NULL) {
        perror("Errore malloc ordini");
        return -1;
    }

    FILE *file = open_file_probed(filepath);
    if (file == NULL) {
        free(*entries);
        *entries = NULL;
        return -1;
    }

    char line[256];
    int index = 0;
    int is_header = 1;
    while (fgets(line, sizeof(line), file)) {
        trim(line);
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }
        if (is_header) {
            is_header = 0;
            continue;
        }

        int id;
        char tipo;
        int quantita;
        int priorita;
        int tempo_max;

        if (sscanf(line, "%d , %c , %d , %d , %d", &id, &tipo, &quantita, &priorita, &tempo_max) == 5) {
            (*entries)[index].id = id;
            (*entries)[index].tipo = tipo;
            (*entries)[index].quantita = quantita;
            (*entries)[index].priorita = priorita;
            (*entries)[index].tempo_completamento_massimo = tempo_max;
            index++;
        }
    }

    fclose(file);
    return index;
}

int parse_parametri(const char *filepath, parametri_simulazione *dest) {
    FILE *file = open_file_probed(filepath);
    if (file == NULL) {
        return -1;
    }

    char line[256];
    dest->max_area_pressa = 0.0f;
    dest->capacita_buffer_laminazione = 0;
    dest->capacita_buffer_pressa = 0;
    dest->capacita_buffer_gom = 0;
    dest->politica_controllo[0] = '\0';
    dest->temperatura_ambiente_iniziale = 0.0f;
    dest->temperatura_incremento_minuto = 0.0f;
    dest->durata_simulazione_max = 0;

    while (fgets(line, sizeof(line), file)) {
        char *comment = strchr(line, '#');
        if (comment) {
            *comment = '\0';
        }
        trim(line);
        if (strlen(line) == 0) {
            continue;
        }

        char *eq = strchr(line, '=');
        if (eq == NULL) {
            continue;
        }

        *eq = '\0';
        char *key = line;
        char *val = eq + 1;
        trim(key);
        trim(val);

        if (strcmp(key, "max_area_pressa") == 0) {
            dest->max_area_pressa = atof(val);
        } else if (strcmp(key, "capacita_buffer_laminazione") == 0) {
            dest->capacita_buffer_laminazione = atoi(val);
        } else if (strcmp(key, "capacita_buffer_pressa") == 0) {
            dest->capacita_buffer_pressa = atoi(val);
        } else if (strcmp(key, "capacita_buffer_gom") == 0) {
            dest->capacita_buffer_gom = atoi(val);
        } else if (strcmp(key, "politica_controllo") == 0) {
            strncpy(dest->politica_controllo, val, sizeof(dest->politica_controllo) - 1);
            dest->politica_controllo[sizeof(dest->politica_controllo) - 1] = '\0';
        } else if (strcmp(key, "temperatura_ambiente_iniziale") == 0) {
            dest->temperatura_ambiente_iniziale = atof(val);
        } else if (strcmp(key, "temperatura_incremento_minuto") == 0) {
            dest->temperatura_incremento_minuto = atof(val);
        } else if (strcmp(key, "durata_simulazione_max") == 0) {
            dest->durata_simulazione_max = atoi(val);
        }
    }

    fclose(file);
    return 0;
}
