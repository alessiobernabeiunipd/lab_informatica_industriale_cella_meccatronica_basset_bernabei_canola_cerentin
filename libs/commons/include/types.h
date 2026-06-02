#ifndef TYPES_H
#define TYPES_H

//struttura che contiene i tempi come istanti temporali in cui il pezzo compie un'azione determinata
typedef struct { 
int ingresso; 	
int inizio_laminazione; 
int inizio_pressa; 
int inizio_gom; 
int uscita;
}timestamps;

//struttura per contenere le caratteristiche della struttura che vengono passate col pezzo (caratteritìstiche varaino in base al pezzo in lavorazione)
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
    int ID_ordine;              // per identificare pezzi che appartengono allo stesso ordine. Utile per le stats finali per fare cfr su totale pezzi ordine vs pezzi prodotti e calcolare scarto
    int priorità; 
    int deadline_ticks;         // entro quando devo completare il pezzo? sono nei tempi richiesti?
    valori_nominali *valori_nom; // pointer into the catalogue array 
    piece_status stato;
    timestamps ts;
    int tempo_laminazione_effettivo;
    int tempo_pressa_effettivo; 
    int lead_time; 
    float deviazione_gom;        //inizializzato a zero
    struct Pezzo *next;
}pezzo;

#endif