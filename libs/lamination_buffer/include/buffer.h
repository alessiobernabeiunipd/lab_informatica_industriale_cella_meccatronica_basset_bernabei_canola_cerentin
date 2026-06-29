/*libreria di funzionalità del buffer, contiene funzioni 
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

//Struttura contenente un array di puntatori a Pezzo, che rappresenta il buffer.
//Permette di creare più buffer con dimensioni diverse.
typedef struct buffer{
    Pezzo **array;
    int array_size;
} buffer;

/*Inizializza un buffer vuoto nell'heap e ne restituisce una struttura buffer*/
buffer *initialize(int buffer_size);

/*Aggiunge un nuovo elemento al buffer, per precauzione esegue una funzione
 *di controllo is_full*/
void new_item(Pezzo *head, buffer *buf);

/*Rimuove un elemento dal buffer, per precauzione esegue una funzione
 *di controllo is_empty*/
Pezzo *take_item(buffer *buf);

/*Verifica se il buffer ha ancora spazio a disposizione*/
bool is_full(buffer *buf);

/*Verifica se il buffer è vuoto*/
bool is_empty(buffer *buf);

/*Elimina la memoria occupata dal buffer*/
void terminate(buffer *buf);
#endif