#ifndef logger_h
#define logger_h

// apre il file di log al percorso indicato, pronto a ricevere eventi di log. Restituisce 0 se l'apertura è andata a buon fine, -1 altrimenti.
int logger_init(const char *log_file_path); 
// scrive un evento informativo con il tick corrente come timestamp
void log_info(int tick, const char *message);
//scrive un warning con il tick corrente
void log_warning(int tick, const char *message);
//scrive un errore con il tick corrente
void log_error(int tick, const char *message);
//chiude il file di log
void logger_close();

#endif