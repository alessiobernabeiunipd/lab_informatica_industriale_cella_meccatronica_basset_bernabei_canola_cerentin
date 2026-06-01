//#DEFINE

typedef struct { 
int ingresso; 	
int inizio_laminazione; 
int inizio_pressa; 
int inizio_gom; 
int uscita;
} Timestamps;


typedef struct pezzo{
    int id_pezzo;
    int priorita; 
    int deadline_ticks; 
    TempiNominali *tempi_nom; // ← pointer into the catalogue array 
    PieceStatus stato;
    Timestamps ts;
    int tempo_laminazione_effettivo;
    int tempo_pressa_effettivo; 
    int lead_time; 
    int n_rilavorazioni;
    float deviazione_gom; 
    struct Pezzo *next;
}pezzo;
