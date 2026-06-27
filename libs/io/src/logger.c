#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#define ANSI_RESET "\033[0m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_RED "\033[31m"


static FILE *log_file = NULL;

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
// la stessa cosa su più funzioni

static void scrivi_log(int tick, const char *message, const char *level, const char *color){
// scrive sempre a terminale, su file solo se è aperto
    fprintf(stdout, "%s[%-7s] Tick %04d: %s%s\n", color, level, tick, message, ANSI_RESET);
    if (log_file != NULL){
        fprintf(log_file, "[%-7s] Tick %04d: %s\n", level, tick, message);   
        fflush(log_file); 
    }
}

void log_info(int tick, const char *message){
    scrivi_log(tick, message, "INFO", ANSI_RESET);
}

void log_warning(int tick, const char *message){
    scrivi_log(tick, message, "WARNING", ANSI_YELLOW);
}

void log_error(int tick, const char *message){
    scrivi_log(tick, message, "ERROR", ANSI_RED);
}

void logger_close(void){
    if (log_file == NULL){
        return;
    }
    fclose(log_file);
    log_file = NULL;
}