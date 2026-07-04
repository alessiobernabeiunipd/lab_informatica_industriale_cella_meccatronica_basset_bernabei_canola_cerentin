#include "parsing.h"
#include "unity.h"
#include <stdio.h>
#include <stdlib.h>

// Scrive 'contenuto' nel file 'path': fixture temporaneo per i test del parser.
// Nota: passare ai parser un nome che NON inizia con "input/" fa fare a
// open_file_probed una fopen diretta dalla CWD, senza risalire le cartelle.
// A fine test ricordarsi di remove(path) per non lasciare file sporchi in build/.
static void scrivi_file(const char *path, const char *contenuto){
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        TEST_FAIL_MESSAGE("Impossibile creare il file");
        return;
    }
    fputs(contenuto, f);
    fclose(f);
}

void setUp(void) {}
void tearDown(void) {}

// 1. Catalogo valido: header + N righe -> parse_catalogo ritorna N e popola entries.
//    Ricorda: free(entries) e remove() del file a fine test.
void test_catalogo_valido(void){
    scrivi_file("tmp_catalogo.csv", "tipo,tlam,ciclo,dev,area\nA,5,10,0.5,100.0\nB,3,8,1.2,80.0\n");
    catalogo_entry *entries = NULL;
    int n = parse_catalogo("tmp_catalogo.csv", &entries);
    TEST_ASSERT_EQUAL_INT(2, n);
    TEST_ASSERT_EQUAL_INT('A', entries[0].tipo);
    TEST_ASSERT_EQUAL_INT(5, entries[0].tempo_laminazione);
    TEST_ASSERT_EQUAL_INT('B', entries[1].tipo);
    free(entries);
    remove("tmp_catalogo.csv");
}

// 2. Ordini validi: come sopra ma per parse_ordini (verifica id/quantita/priorita).
void test_ordini_valido(void){
    scrivi_file("tmp_ordini.csv", "id,tipo,quantita,priorita,tempo_max\n1,A,3,10,360\n2,B,5,2,480\n");
    ordine_entry *entries = NULL;
    int n = parse_ordini("tmp_ordini.csv", &entries);
    TEST_ASSERT_EQUAL_INT(2, n);
    TEST_ASSERT_EQUAL_INT(1, entries[0].id);
    TEST_ASSERT_EQUAL_INT('A', entries[0].tipo);
    TEST_ASSERT_EQUAL_INT(3, entries[0].quantita);
    TEST_ASSERT_EQUAL_INT(10, entries[0].priorita);
    TEST_ASSERT_EQUAL_INT(360, entries[0].tempo_completamento_massimo);
    free(entries);
    remove("tmp_ordini.csv");
}

// 3. Parametri validi: righe chiave=valore -> parse_parametri ritorna 0 e popola dest.
void test_parametri_valido(void){
    scrivi_file("tmp_par.txt",
                "durata_simulazione_max=100\n"
                "politica_controllo=fcfs\n"
                "capacita_buffer_gom=5\n");
    parametri_simulazione dest;
    int r = parse_parametri("tmp_par.txt", &dest);
    TEST_ASSERT_EQUAL_INT(0, r);
    TEST_ASSERT_EQUAL_INT(100, dest.durata_simulazione_max);
    TEST_ASSERT_EQUAL_INT(5, dest.capacita_buffer_gom);
    TEST_ASSERT_EQUAL_STRING("fcfs", dest.politica_controllo);
    remove("tmp_par.txt");
}

// 4. File mancante (path di errore): parse_catalogo -> 0 con entries == NULL;
//    parse_parametri -> -1. Conferma che sull'errore non si alloca nulla.
void test_file_mancante(void){
    catalogo_entry *entries = NULL;
    int n = parse_catalogo("non_esiste_catalogo.csv", &entries);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NULL(entries);

    parametri_simulazione dest;
    int r = parse_parametri("non_esiste_par.txt", &dest);
    TEST_ASSERT_EQUAL_INT(-1, r);
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_catalogo_valido);
    RUN_TEST(test_ordini_valido);
    RUN_TEST(test_parametri_valido);
    RUN_TEST(test_file_mancante);
    return UNITY_END();
}
