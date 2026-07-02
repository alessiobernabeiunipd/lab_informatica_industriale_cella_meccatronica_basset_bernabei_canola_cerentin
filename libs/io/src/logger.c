#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#define ANSI_RESET "\033[0m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_RED "\033[31m"


static FILE *log_file = NULL;

// apre il file in modalità "W", ritorna -1 se fallisce o già inizializzato
int logger_init(const char *log_file_path){
    if (log_file != NULL){
        return -1; //file già aperto
    }
    log_file = fopen(log_file_path, "w");
    if (log_file == NULL){
        return -1; //errore apertura file    
    }
    return 0; //success
}

// funzione di supporto per scrittura evento con tick corrente => non scrivo
// la stessa cosa su più funzioni; output duplicato su terminale e file se aperto
//fflush serve a non perdere righe in caso di crash
static void scrivi_log(int tick, const char *message, const char *level, const char *color){
    fprintf(stdout, "%s[%-7s] Tick %04d: %s%s\n", color, level, tick, message, ANSI_RESET);
    if (log_file != NULL){
        fprintf(log_file, "[%-7s] Tick %04d: %s\n", level, tick, message);   
        fflush(log_file); 
    }
}
static void scrivi_log_f(int tick, const char *format, va_list args, const char *level, const char *color){
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), format, args); // scrive il messaggio su una stringa rispettando il limite di dimensione
        // restituisce un int che indica il numero di caratteri scritti (non usato)
        scrivi_log(tick, buffer, level, color);
}
// scrivi_log e log_f hanno lo stesso output, ma log_f permette di passare un numero variabile di argomenti come printf

void log_info(int tick, const char *message){
    scrivi_log(tick, message, "INFO", ANSI_RESET);
}
void log_info_f(int tick, const char *format, ...){
    va_list args;
    va_start(args, format); // inizializza args all'ultimo parametro fisso prima dei ...
    scrivi_log_f(tick, format, args, "INFO", ANSI_RESET);
    va_end(args); // chiude la lista (libera lo stato interno)
}

void log_warning(int tick, const char *message){
    scrivi_log(tick, message, "WARNING", ANSI_YELLOW);
}
void log_warning_f(int tick, const char *format, ...){
    va_list args;
    va_start(args, format);
    scrivi_log_f(tick, format, args, "WARNING", ANSI_YELLOW);
    va_end(args);
}

void log_error(int tick, const char *message){
    scrivi_log(tick, message, "ERROR", ANSI_RED);
}
void log_error_f(int tick, const char *format, ...){
    va_list args;
    va_start(args, format);
    scrivi_log_f(tick, format, args, "ERROR", ANSI_RED);
    va_end(args);
}
// chiude il file e azzera il puntatore
void logger_close(void){
    if (log_file == NULL){
        return; // serve a evitare crash se viene chiamata più volte senza init
    }
    fclose(log_file);
    log_file = NULL;
}