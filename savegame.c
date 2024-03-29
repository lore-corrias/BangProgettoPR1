#include "savegame.h"

/**
 * Legge dal file "nomeSalvataggio"."SAVEGAME_EXT" il salvataggio in cui è contenuta una partita,
 * e restituisce una struttura avente per campi tutti i parametri di quel salvataggio.
 *
 * @param nomeSalvataggio Il nome del file in cui è contenuto il salvataggio.
 * @return Struttura contenente il salvataggio caricato.
 */
Salvataggio caricaSalvataggio(char nomeSalvataggio[SAVEGAME_NAME_LEN + 1]) {
    // numero di byte letti per ogni operazione
    size_t read, readPlayer, readMano, readGioco;
    // struct salvataggio e file da aprire per leggere i suoi campi
    Salvataggio salvataggio;
    // nome completo del file di salvataggio
    char nomeSalvataggioCompleto[SAVEGAME_NAME_LEN + SAVEGAME_EXT_LEN + 1] = {};
    // puntatore al file di salvataggio
    FILE* salvataggioFile = NULL;

    // ottengo il nome completo del file di salvataggio
    aggiungiEstensioneSalvataggio(nomeSalvataggio, nomeSalvataggioCompleto);

    // apertura del file in lettura binaria
    salvataggioFile = fopen(nomeSalvataggioCompleto, "rb");
    assertPuntatoreNonNull(salvataggioFile, "\nImpossibile aprire il file di salvataggio.");

    // lettura del file di salvataggio
    // numero di giocatori
    read = fread(&salvataggio.nGiocatori, sizeof(int), 1, salvataggioFile);
    if(read != 1) {
        printf("\nErrore durante la lettura del file di salvataggio '%s': numero di giocatori non determinato.", nomeSalvataggio);
        exit(-1);
    }

    // alloco l'array dei giocatori nel salvataggio
    salvataggio.giocatori = (Giocatore*) calloc(salvataggio.nGiocatori, sizeof(Giocatore));
    assertPuntatoreNonNull(salvataggio.giocatori, "\nErrore: impossibile allocare dinamicamente l'array di giocatori.");

    // lettura dei blocchi di giocatori
    for(int i = 0; i < salvataggio.nGiocatori; i++) {
        readPlayer = fread(&salvataggio.giocatori[i], sizeof(Giocatore), 1, salvataggioFile);
        if(readPlayer != 1) {
            printf("\nImpossibile leggere le informazioni del giocatore");
            exit(-1);
        }

        // alloco dinamicamente le altre strutture
        // lettura carte della mano del giocatore
        if(salvataggio.giocatori[i].carteMano.numeroCarte > 0) {
            // allocazione dinamica
            salvataggio.giocatori[i].carteMano.carte = (Carta *) calloc(salvataggio.giocatori[i].carteMano.numeroCarte, sizeof(Carta));
            assertPuntatoreNonNull(salvataggio.giocatori[i].carteMano.carte, "\nImpossibile caricare dinamicamente le carte del giocatore.");
            // lettura da file
            readMano = fread(salvataggio.giocatori[i].carteMano.carte, sizeof(Carta), salvataggio.giocatori[i].carteMano.numeroCarte, salvataggioFile);
            if(readMano != salvataggio.giocatori[i].carteMano.numeroCarte) {
                printf("\nImpossibile leggere tutte le carte della mano del giocatore.");
                exit(-1);
            }
        }
        // lettura carte in gioco del giocatore
        if(salvataggio.giocatori[i].carteGioco.numeroCarte > 0) {
            // allocazione dinamica
            salvataggio.giocatori[i].carteGioco.carte = (Carta *) calloc(salvataggio.giocatori[i].carteGioco.numeroCarte, sizeof(Carta));
            assertPuntatoreNonNull(salvataggio.giocatori[i].carteGioco.carte, "\nImpossibile caricare dinamicamente le carte del giocatore.");
            // lettura da file
            readGioco = fread(salvataggio.giocatori[i].carteGioco.carte, sizeof(Carta), salvataggio.giocatori[i].carteGioco.numeroCarte, salvataggioFile);
            if(readGioco != salvataggio.giocatori[i].carteGioco.numeroCarte) {
                printf("\nErrore: impossibile leggere tutte le carte in gioco del giocatore.");
                exit(-1);
            }
        }
    }

    // lettura del numero del prossimo giocatore che deve giocare
    read = fread(&salvataggio.prossimoGiocatore, sizeof(int), 1, salvataggioFile);
    if(read != 1) {
        printf("\nErrore: impossibile leggere il numero del prossimo giocatore.");
        exit(-1);
    }

    // lettura del mazzo di pesca
    read = fread(&salvataggio.mazzoPesca, sizeof(Mazzo), 1, salvataggioFile);
    if(read != 1) {
        printf("\nErrore: impossibile leggere il mazzo di pesca.");
        exit(-1);
    }
    // lettura carte del mazzo di pesca
    if(salvataggio.mazzoPesca.numeroCarte > 0) {
        // allocazione dinamica
        salvataggio.mazzoPesca.carte = (Carta *) calloc(salvataggio.mazzoPesca.numeroCarte, sizeof(Carta));
        assertPuntatoreNonNull(salvataggio.mazzoPesca.carte, "\nErrore: impossibile caricare le carte del mazzo di pesca.");
        // lettura da file
        read = fread(salvataggio.mazzoPesca.carte, sizeof(Carta), salvataggio.mazzoPesca.numeroCarte, salvataggioFile);
        if(read != salvataggio.mazzoPesca.numeroCarte) {
            printf("\nImpossibile leggere tutte le carte del mazzo di pesca del giocatore.");
            exit(-1);
        }
    }

    // lettura del mazzo di scarti
    read = fread(&salvataggio.mazzoScarti, sizeof(Mazzo), 1, salvataggioFile);
    if(read != 1) {
        printf("\nErrore: impossibile leggere il mazzo di scarti.");
        exit(-1);
    }
    // lettura carte del mazzo di scarti
    if(salvataggio.mazzoScarti.numeroCarte > 0) {
        // allocazione dinamica
        salvataggio.mazzoScarti.carte = (Carta *) calloc(salvataggio.mazzoScarti.numeroCarte, sizeof(Carta));
        assertPuntatoreNonNull(salvataggio.mazzoScarti.carte, "\nErrore: impossibile caricare le carte del mazzo di scarti.");
        // lettura da file
        read = fread(salvataggio.mazzoScarti.carte, sizeof(Carta), salvataggio.mazzoScarti.numeroCarte, salvataggioFile);
        if(read != salvataggio.mazzoScarti.numeroCarte) {
            printf("\nImpossibile leggere tutte le carte del mazzo di scarti del giocatore.");
            exit(-1);
        }
    }

    // aggiunta del nome del salvataggio alla struttura
    strcpy(salvataggio.nomeSalvataggio, nomeSalvataggio);

    // chiusura del file
    fclose(salvataggioFile);

    // restituisci il puntatore al salvataggio letto
    return salvataggio;
}

/**
 * Subroutine che scrive una struct "Salvataggio" sul file con il nome fornito.
 *
 * @param salvataggio Il salvataggio da scrivere.
 * @param nomeSalvataggio Il nome del salvataggio da scrivere.
 */
void scriviSalvataggio(Salvataggio salvataggio, char nomeSalvataggio[]) {
    // numero di byte scritti per ogni operazione
    size_t write, writePlayer, writeMano, writeGioco;
    // file da aprire per la scrittura
    FILE* salvataggioFile = NULL;
    // nome del salvataggio completo
    char nomeSalvataggioCompleto[SAVEGAME_NAME_LEN + SAVEGAME_EXT_LEN + 1] = {};

    // ottengo il nome completo del file
    aggiungiEstensioneSalvataggio(nomeSalvataggio, nomeSalvataggioCompleto);

    // scrivo il nome sulla lista dei salvataggi, se questo non è presente
    if(!isSalvataggioInLista(nomeSalvataggio))
        aggiungiSalvataggioLista(nomeSalvataggio);

    // apertura del file e verifica
    salvataggioFile = fopen(nomeSalvataggioCompleto, "wb");
    assertPuntatoreNonNull(salvataggioFile, "\nErrore durante l'apertura del file di salvataggio.");

    // scrittura del numero di giocatori
    write = fwrite(&salvataggio.nGiocatori, sizeof(int), 1, salvataggioFile);
    if(write != 1) {
        printf("\nErrore durante la scrittura del file di salvataggio: impossibile scrivere il numero di player.");
        exit(-1);
    }
    // scrittura degli nGiocatori
    for(int i = 0; i < salvataggio.nGiocatori; i++) {
        writePlayer = fwrite(&salvataggio.giocatori[i], sizeof(Giocatore), 1, salvataggioFile);
        if(writePlayer != 1) {
            printf("\nErrore durante la scrittura del giocatore sul file di salvataggio.");
            exit(-1);
        }
        if(salvataggio.giocatori[i].carteMano.numeroCarte > 0) {
            writeMano = fwrite(salvataggio.giocatori[i].carteMano.carte, sizeof(Carta), salvataggio.giocatori[i].carteMano.numeroCarte, salvataggioFile);
            if(writeMano != salvataggio.giocatori[i].carteMano.numeroCarte) {
                printf("\nErrore: impossibile scrivere le carte della mano del giocatore su file.");
                exit(-1);
            }
        }
        if(salvataggio.giocatori[i].carteGioco.numeroCarte > 0) {
            writeGioco = fwrite(salvataggio.giocatori[i].carteGioco.carte, sizeof(Carta), salvataggio.giocatori[i].carteGioco.numeroCarte, salvataggioFile);
            if(writeGioco != salvataggio.giocatori[i].carteGioco.numeroCarte) {
                printf("\nErrore: impossibile scrivere le carte in gioco del giocatore su file.");
                exit(-1);
            }
        }
    }
    // scrittura del prossimo giocatore
    write = fwrite(&salvataggio.prossimoGiocatore, sizeof(int), 1, salvataggioFile);
    if(write != 1) {
        printf("\nErrore durante la scrittura del file di salvataggio: impossibile scrivere la posizione del prossimo giocatore.");
        exit(-1);
    }
    // scrittura del mazzo di pesca
    write = fwrite(&salvataggio.mazzoPesca, sizeof(Mazzo), 1, salvataggioFile);
    if(write != 1) {
        printf("\nErrore durante la scrittura del file di salvataggio: impossibile scrivere il mazzo di pesca.");
        exit(-1);
    }
    if(salvataggio.mazzoPesca.numeroCarte > 0) {
        write = fwrite(salvataggio.mazzoPesca.carte, sizeof(Carta), salvataggio.mazzoPesca.numeroCarte, salvataggioFile);
        if(write != salvataggio.mazzoPesca.numeroCarte) {
            printf("\nErrore: impossibile scrivere le carte del mazzo di pesca sul file di salvataggio.");
            exit(-1);
        }
    }
    // scrittura del mazzo di scarti
    write = fwrite(&salvataggio.mazzoScarti, sizeof(Mazzo), 1, salvataggioFile);
    if(write != 1) {
        printf("\nErrore durante la scrittura del file di salvataggio: impossibile scrivere il mazzo di scarti.");
        exit(-1);
    }
    if(salvataggio.mazzoScarti.numeroCarte > 0) {
        write = fwrite(salvataggio.mazzoScarti.carte, sizeof(Carta), salvataggio.mazzoScarti.numeroCarte, salvataggioFile);
        if(write != salvataggio.mazzoScarti.numeroCarte) {
            printf("\nErrore: impossibile scrivere le carte del mazzo di scarti.");
            exit(-1);
        }
    }

    // chiusura del file
    fclose(salvataggioFile);
}

/**
 * Funzione booleana che verifica che un file di salvataggio, con un nome dato (".sav" escluso) esista.
 *
 * @param nomeSalvataggio Il nome del salvataggio da verificare.
 * @return True se il salvataggio esiste, False altrimenti
 */
bool salvataggioEsistente(char nomeSalvataggio[SAVEGAME_NAME_LEN + 1]) {
    // nome completo del file di salvataggio
    char nomeSalvataggioCompleto[SAVEGAME_NAME_LEN + SAVEGAME_EXT_LEN + 1] = {};

    // ottengo il nome completo del file
    aggiungiEstensioneSalvataggio(nomeSalvataggio, nomeSalvataggioCompleto);

    return fileEsistente(nomeSalvataggioCompleto);
}

/**
 * Subroutine che stampa a schermo tutti i nomi dei salvataggi attualmente presenti nella lista,
 * contenuta nel file SAVEGAME_LIST_FILE
 */
void stampaSalvataggiInLista() {
    // puntatore contenente il file con la lista dei salvataggi
    FILE* fileLista = NULL;
    // stringa contenente il nome del file di salvataggio che sta venendo letto al momento
    char nomeSalvataggioCorrente[SAVEGAME_NAME_LEN + 1] = "";
    // variabile contenente il numero di byte letti
    int read;

    printf("\n%s LISTA SALVATAGGI %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
    // creo il file con la lista se non esiste, e stampo a schermo che è vuota
    if(!fileEsistente(SAVEGAME_LIST_FILE)) {
        fileLista = fopen(SAVEGAME_LIST_FILE, "w");
        assertPuntatoreNonNull(fileLista, "Errore: impossibile aprire il file di lista dei salvataggi in scrittura");
        printf("\nLa lista dei salvataggi è vuota!");
        fclose(fileLista);
    } else {
        fileLista = fopen(SAVEGAME_LIST_FILE, "r"); // apro il file in lettura
        assertPuntatoreNonNull(fileLista, "Errore: impossibile aprire il file di lista dei salvataggi in lettura");

        do {
            read = fscanf(fileLista, "%s", nomeSalvataggioCorrente);
            // verifico che il nome del salvataggio letto sia quello che sto cercando
            if(read == 1)
                printf("\n*) %s", nomeSalvataggioCorrente);
        } while(read == 1);
    }
    printf("\n%s LISTA SALVATAGGI %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
    fclose(fileLista);
}

/**
 * Subroutine che scrive il nome di un nuovo salvataggio sulla lista.
 *
 * @param nomeSalvataggio Il nome del salvataggio da scrivere.
 */
void aggiungiSalvataggioLista(char nomeSalvataggio[SAVEGAME_NAME_LEN + 1]) {
    // puntatore contenente il file con la lista dei salvataggi
    FILE* fileLista = NULL;

    // apro il file di salvataggi in scrittura
    fileLista = fopen(SAVEGAME_LIST_FILE, "a");
    assertPuntatoreNonNull(fileLista, "Errore: impossibile aprire il file di lista dei salvataggi in scrittura");

    // scrivo il salvataggio su una nuova riga
    fprintf(fileLista, "%s\n", nomeSalvataggio);
    fclose(fileLista);
}

/**
 * Funzione che verifica che un salvataggio sia presente nella lista dei salvataggi.
 *
 * @param nomeSalvataggio Il nome del salvataggio da verificare.
 * @return True se il salvataggio è in lista, false altrimenti.
 */
bool isSalvataggioInLista(char nomeSalvataggio[SAVEGAME_NAME_LEN + 1]) {
    // puntatore contenente il file con la lista dei salvataggi
    FILE* fileLista = NULL;
    // stringa contenente il nome del file di salvataggio che sta venendo letto al momento
    char nomeSalvataggioCorrente[SAVEGAME_NAME_LEN + 1] = "";
    // variabile contenente il numero di byte letti
    int read = 0;

    // creo il file con la lista se non esiste, e restituisco "false" dato che è vuoto
    if(!fileEsistente(SAVEGAME_LIST_FILE)) {
        fileLista = fopen(SAVEGAME_LIST_FILE, "w");
        assertPuntatoreNonNull(fileLista, "Errore: impossibile aprire il file di lista dei salvataggi in scrittura");
        fclose(fileLista);
        return false;
    }
    fileLista = fopen(SAVEGAME_LIST_FILE, "r"); // apro il file in lettura
    assertPuntatoreNonNull(fileLista, "Errore: impossibile aprire il file di lista dei salvataggi in lettura");

    do {
        read = fscanf(fileLista, "%s\n", nomeSalvataggioCorrente);
        // verifico che il nome del salvataggio letto sia quello che sto cercando
        if(strcmp(nomeSalvataggio, nomeSalvataggioCorrente) == 0) {
            fclose(fileLista);
            return true;
        }
    } while(read == 1);
    fclose(fileLista);
    return false; // il file non è stato trovato
}

/**
 * Funzione che verifica che un file esista.
 *
 * @param nomeFile Il nome del file da verificare.
 * @return True se il file esiste, False altrimenti.
 */
bool fileEsistente(char* nomeFile) {
    FILE* fp = fopen(nomeFile, "r");
    // se fp == NULL, allora il file non esiste. la modalità in lettura non crea il salvataggio
    if(fp) {
        fclose(fp); // è necessario chiudere il file
        return true;
    }
    return false;
}

/**
 * Subroutine che aggiunge al nome di un file di salvataggio la sua estensione.
 *
 * @param nomeSalvataggio Il nome del file di salvataggio incompleto.
 * @param nomeCompleto Puntatore a una stringa che deve contenere il nome del file completo.
 */
void aggiungiEstensioneSalvataggio(char nomeSalvataggio[SAVEGAME_NAME_LEN + 1], char nomeCompleto[SAVEGAME_NAME_LEN + SAVEGAME_EXT_LEN + 1]) {
    // copio il nome del salvataggio
    strcat(nomeCompleto, nomeSalvataggio);
    // e poi l'estensione
    strcat(nomeCompleto, SAVEGAME_EXT);
}