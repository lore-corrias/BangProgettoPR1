#include <stdbool.h>
#include "giocatore.h"

// -------- MACRO GENERALI --------
/**
 * Macro contenente la directory in cui inserire i file di salvataggio
 * (lo "/" alla fine del nome è necessario)
 */
#define SAVEGAME_DIR "savegames/"
/**
 * Se definita, questa macro indica il nome del file di testo in cui sono contenuti, uno per riga,
 * i nomi dei file di salvataggio precedentemente creati. La path completa è "SAVEGAME_DIR/SAVEGAME_LIST_FILE",
 */
#define SAVEGAME_LIST_FILE "saves.txt"
/**
 * Lunghezza massima del nome di un file di salvataggio ('\0' escluso).
 */
#define SAVEGAME_NAME_LEN 16

/**
 * Struttura contenente le informazioni fondamentali relative a una partita, memorizzate in un file di salvataggio.
 * I campi di questa struttura vengono utilizzati per creare i file di salvataggio.
 */
typedef struct {
    Giocatore* giocatori;
    int nGiocatori;
    Mazzo mazzoPesca;
    Mazzo mazzoScarti;
    int prossimoGiocatore;
    char nomeSalvataggio[SAVEGAME_NAME_LEN + 1]; // TODO: il nome del salvataggio deve essere aggiunto anche nel file binario?
} Salvataggio;

Salvataggio caricaSalvataggio(char nomeSalvataggio[SAVEGAME_NAME_LEN + 1]);
void scriviSalvataggio(Salvataggio salvataggio, char nomeSalvataggio[SAVEGAME_NAME_LEN]);