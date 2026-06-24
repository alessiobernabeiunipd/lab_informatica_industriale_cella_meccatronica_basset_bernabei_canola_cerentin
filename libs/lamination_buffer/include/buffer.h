/*libreria di funzionalità del buffer pre laminazione, contiene funzioni 
 *generiche per la gestione di un buffer con logica FIFO.
 *Ogni volta che viene aggionto un nuovo elemento questo viene inserito
 *nel primo indice libero dell'array.
 *Ogni volta che viene prelevato un elemento lo si acquisisce dal primo
 *elemento dell'array.
 *Si tratta di un array di puntatori che indicano i nodi della lista
 *concatenata che risultano all'interno del buffer.
*/

#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 4

//pezzo che viene lavorato, una struct definisce un pezzo singolo. 
//linked list, probabilmente da eliminare.
typedef struct pezzo{
    int id_pezzo;
    int ID_ordine;              // per identificare pezzi che appartengono allo stesso ordine. Utile per le stats finali per fare cfr su totale pezzi ordine vs pezzi prodotti e calcolare scarto
    int priorità; 
    int deadline_ticks;         // entro quando devo completare il pezzo? sono nei tempi richiesti?
    //valori_nominali *tempi_nom; // pointer into the catalogue array 
    //piece_status stato;
    //timestamps ts;
    int tempo_laminazione_effettivo;
    int tempo_pressa_effettivo; 
    int lead_time; 
    float deviazione_gom;        //inizializzato a zero
    struct pezzo *next;
}Pezzo;

/*Inizializza un buffer vuoto nell'heap e ne restituisce un puntatore*/
Pezzo **initialize(int buffer_size);

/*Aggiunge un nuovo elemento al buffer, per precauzione esegue una funzione
 *di controllo is_full*/
void new_item(Pezzo *head, Pezzo **buffer, int buffer_size);

/*Rimuove un elemento dal buffer, per precauzione esegue una funzione
 *di controllo is_empty*/
Pezzo *take_item(Pezzo **buffer, int buffer_size);

/*Verifica se il buffer ha ancora spazio a disposizione*/
bool is_full(Pezzo **buffer, int buffer_size);

/*Verifica se il buffer è vuoto*/
bool is_empty(Pezzo **buffer);

/*Elimina la memoria occupata dal buffer*/
void terminate(Pezzo **buffer, int buffer_size);
#endif