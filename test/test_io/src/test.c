#include "logger.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {
    logger_close(); // chiude il file di log se aperto
    remove("test_log.log"); // pulizia
} 
// controlla che venga scritto un messaggio di info (controlla anche il formato della riga)
void test_logger_apre_e_scrive_info(void){
    int init_result = logger_init("test_log.log");
    TEST_ASSERT_EQUAL_INT(0, init_result); // controlla apertura file 
    log_info(1, "Test info message"); // tick e mess. arbitrario per test
    logger_close(); // chiude il file di log per non rischiare di leggere file con buffer non scaricato
    FILE *file = fopen("test_log.log", "r"); // riapro il file in lettura
    TEST_ASSERT_NOT_NULL(file); // controlla apertura file in lettura
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);
    TEST_ASSERT_NOT_NULL(buffer); 
    TEST_ASSERT_EQUAL_STRING("[INFO   ] Tick 0001: Test info message\n", buffer); // controlla contenuto
    fclose(file);
}
// controlla che venga scritto un messaggio di warning (controlla anche il formato)
void test_logger_scrive_warning(void){
    int init_result = logger_init("test_log.log");
    TEST_ASSERT_EQUAL_INT(0, init_result); // controlla apertura file 
    log_warning(2, "Test warning message"); // tick e mess. arbitrario per test
    logger_close(); // chiude il file di log per non rischiare di leggere file con buffer non scaricato
    FILE *file = fopen("test_log.log", "r"); // riapro il file in lettura
    TEST_ASSERT_NOT_NULL(file); // controlla apertura file in lettura
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);
    TEST_ASSERT_NOT_NULL(buffer); 
    TEST_ASSERT_EQUAL_STRING("[WARNING] Tick 0002: Test warning message\n", buffer); // controlla contenuto
    fclose(file);
}
// controlla che venga scritto un messaggio di error (controlla anche il formato della riga)
void test_logger_scrive_error(void){
    int init_result = logger_init("test_log.log");
    TEST_ASSERT_EQUAL_INT(0, init_result); // controlla apertura file 
    log_error(3, "Test error message"); // tick e mess. arbitrario per test
    logger_close(); // chiude il file di log per non rischiare di leggere file con buffer non scaricato
    FILE *file = fopen("test_log.log", "r"); // riapro il file in lettura
    TEST_ASSERT_NOT_NULL(file); // controlla apertura file in lettura
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);
    TEST_ASSERT_NOT_NULL(buffer); 
    TEST_ASSERT_EQUAL_STRING("[ERROR  ] Tick 0003: Test error message\n", buffer); // controlla contenuto
    fclose(file);
}
void test_logger_no_ansi_su_file(void){
int init_result = logger_init("test_log.log");
    TEST_ASSERT_EQUAL_INT(0, init_result);
    log_error(4, "Test error message"); // chiamo error per colore rosso
    logger_close();
    FILE *file = fopen("test_log.log", "r");
    TEST_ASSERT_NOT_NULL(file);
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);
    TEST_ASSERT_NOT_NULL(buffer);
    TEST_ASSERT_NULL(strstr(buffer, "\033[31m")); // controlla che non ci siano sequenze ANSI nel file
    fclose(file);
}
// controlla che l'inizializzazione fallisca (fornisco un path non valido)
void test_logger_init_fallisce_su_path_non_valido(void){
int init_result = logger_init("cartella_non_esistente/test_log.log");
    TEST_ASSERT_EQUAL_INT(-1, init_result); // controlla che l'inizializzazione fallisca
    // logger init non ha aperto niente quindi non serve logger_close
}
// controlla la parte di va_list+vsnprintf
void test_logger_variante_formattata(void){
int init_result = logger_init("test_log.log");
    TEST_ASSERT_EQUAL_INT(0, init_result); // controlla apertura file 
    log_info_f(5, "Test formatted message: %d", 42); // tick e mess. arbitrario per test
    logger_close(); // chiude il file di log per non rischiare di leggere file con buffer non scaricato
    FILE *file = fopen("test_log.log", "r"); // riapro il file in lettura
    TEST_ASSERT_NOT_NULL(file); // controlla apertura file in lettura
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);
    TEST_ASSERT_NOT_NULL(buffer); 
    TEST_ASSERT_EQUAL_STRING("[INFO   ] Tick 0005: Test formatted message: 42\n", buffer); // controlla contenuto
    fclose(file);
}


int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_logger_apre_e_scrive_info);
    RUN_TEST(test_logger_scrive_warning);
    RUN_TEST(test_logger_scrive_error);
    RUN_TEST(test_logger_no_ansi_su_file);
    RUN_TEST(test_logger_init_fallisce_su_path_non_valido);
    RUN_TEST(test_logger_variante_formattata);
    return UNITY_END();
}