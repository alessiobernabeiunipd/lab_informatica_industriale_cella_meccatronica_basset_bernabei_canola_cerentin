#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

//struttura che contiene i tempi come istanti temporali in cui il pezzo compie un'azione determinata
// uscita - ingresso = lead time; i tre inizio_x vengono impiegati per il tempo macchina effettivo
typedef struct { 
int ingresso; 	
int inizio_laminazione; 
int inizio_pressa; 
int inizio_gom; 
int uscita;
}timestamps;

//Struttura contenente un array di puntatori a Pezzo, che rappresenta il buffer.
//Permette di creare più buffer con dimensioni diverse.
typedef struct buffer{
    pezzo **array;
    int array_size;
} buffer;

//struttura per contenere le caratteristiche della struttura che vengono passate col pezzo (caratteritìstiche variano in base al pezzo in lavorazione)
//implementato come un array con un n° di struct = numero di pezzi diversi
typedef struct{
    int t_laminazione_nominale;
    int t_pressa_nominale;
    int t_gom;
    int deviazione_max_gom;
}valori_nominali;

//struttura per contenere gli status che può assumere il pezzo
typedef enum{                                                                   
    CREATED, // letto dal CSV ma non ancora in lavorazione
    TRAVELING,// in viaggio sull'AGV
    WAITING_INPUT, //in attesa nel buffer iniziale.
    IN_LAMINATION, // il pezzo sta venendo laminato
    WAITING_PRESS, // nel buffer pre-pressa.
    IN_PRESS, // in lavorazione in pressa.
    ON_CONVEYOR, // trasportato verso il controllo finale.
    IN_GOM, // in controllo/ispezione.
    OK, // pezzo valido.
    SCRAP, // scarto, triggera  creazione di nuova entry nella lista ordini per rifarlo.
    DONE, // completato e uscito dall’impianto
}piece_status;

//pezzo che viene lavorato, una struct definisce un pezzo singolo. 
//linked list
typedef struct pezzo{
    int id_pezzo;
    int ID_ordine;              // per identificare pezzi che appartengono allo stesso ordine per le stats finali per fare cfr su totale pezzi ordine vs pezzi prodotti e calcolare scarto
    int priorità; 
    int deadline_ticks;         // entro quando devo completare il pezzo? sono nei tempi richiesti?
    valori_nominali valori_nom; // copia catalogo valori_nominali                            (sottostruttura statica)
    piece_status stato;             //monitora avanzamento del pezzo                                        (sottostruttura)
    timestamps ts;                  //contiene valori che verranno usati per le tre statistuche sottostanti (sottostruttura)
    int tempo_laminazione_effettivo;
    int tempo_pressa_effettivo; 
    int tempo_gom_effettivo;

    int lead_time; 
    float deviazione_gom;        //inizializzato a zero
    struct pezzo *next;
}pezzo;

typedef enum{
    IDLE,
    BUSY,
    COOLING,
}station_status;

typedef struct { 
    station_status stato;
    int tick_lavorazione_rimasti; //tick rimanenti per finire il lavoro corrente, se broken metto in pausa. inizializzare a 0
    pezzo *pezzo_in_lavorazione; //inizializzare a NULL
}AGV;

typedef struct { 
    station_status stato;
    int tick_lavorazione_rimasti; //tick rimanenti per finire il lavoro corrente, se broken metto in pausa. inizializzare a 0
    pezzo *pezzo_in_lavorazione; //inizializzare a NULL
}stazione_laminazione;

typedef struct { 
    station_status stato;
    int tick_lavorazione_rimasti; //tick rimanenti per finire il lavoro corrente, se cooling metto in pausa. inizializzare a 0
    pezzo *pezzo_in_lavorazione; //inizializzare a NULL
}stazione_pressa;

typedef struct { 
    station_status stato;
    int tick_lavorazione_rimasti; //tick rimanenti per finire il lavoro corrente, se broken metto in pausa. inizializzare a 0
    int tick_cooling_rimasti;
    pezzo *pezzo_in_lavorazione; //inizializzare a NULL
    float t_GOM;
}stazione_GOM;

typedef struct {
    float max_area_pressa;
    int capacita_buffer_laminazione;
    int capacita_buffer_pressa;
    int capacita_buffer_gom;
    char politica_controllo[32];
    //può assumere i valori fcfs o priorità
    float temperatura_ambiente_iniziale;
    float temperatura_incremento_minuto;
    int durata_simulazione_max;
    int tempo_agv;
} parametri_simulazione;

typedef struct {
    stazione_laminazione  *laminazione;
    stazione_pressa       *pressa;
    stazione_GOM          *gom;
    AGV                   *agv;

    buffer *buf_lam;
    buffer *buf_pressa;
    buffer *buf_gom;

    int tick_corrente;               //parametri della simulazione, sono nella struct per avere 
   
    pezzo *list_head;               //punta al primo elemento della linked list
    int pezzi_completati;            //due contatori per le statistiche
    int pezzi_scartati;

    parametri_simulazione param;
} cella_meccatronica;

#endif