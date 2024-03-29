#include "partita.h"


/**
 * Subroutine che avvia il gioco, non appena il programma viene lanciato.
 */
void avvioGioco() {
    // il salvataggio da avviare
    Salvataggio partita;

    // ----------------- LOGICA DI AVVIO DEL GIOCO -------------------
    printf(" ____          _   _  _____ _   _____  _____  __   ______ _____ _____ _______ _____ ____  _   _ \n"
           "|  _ \\   /\\   | \\ | |/ ____| | |  __ \\|  __ \\/_ | |  ____|  __ \\_   _|__   __|_   _/ __ \\| \\ | |\n"
           "| |_) | /  \\  |  \\| | |  __| | | |__) | |__) || | | |__  | |  | || |    | |    | || |  | |  \\| |\n"
           "|  _ < / /\\ \\ | . ` | | |_ | | |  ___/|  _  / | | |  __| | |  | || |    | |    | || |  | | . ` |\n"
           "| |_) / ____ \\| |\\  | |__| |_| | |    | | \\ \\ | | | |____| |__| || |_   | |   _| || |__| | |\\  |\n"
           "|____/_/    \\_\\_| \\_|\\_____(_) |_|    |_|  \\_\\|_| |______|_____/_____|  |_|  |_____\\____/|_| \\_|\n\n");
    printf("Benvenuto, sfidante, in Bang! Il gioco da tavolo organizzato per il progetto finale"
           "del corso di Programmazione 1 dell'Università di Cagliari!\n");
    printf("Per iniziare, dimmi, preferisci creare una partita da zero, oppure caricare un file di salvataggio?\n");

    char choice;
    do {
        printf("%c) Carica Partita\n"
               "%c) Crea Nuova Partita\n"
               "%c) Esci dal gioco\n", PROMPT_CARICA_PARTITA, PROMPT_NUOVA_PARTITA, PROMPT_EXIT);
        printf("?) ");
        scanf(" %c", &choice);
        if(choice != PROMPT_CARICA_PARTITA && choice != PROMPT_NUOVA_PARTITA && choice != PROMPT_EXIT)
            printf("\nInserisci una delle opzioni possibili!\n");
    } while (choice != PROMPT_CARICA_PARTITA && choice != PROMPT_NUOVA_PARTITA && choice != PROMPT_EXIT);

    // da qui, il controllo del flusso di esecuzione è affidato a una delle due funzioni che si occupano di creare/caricare
    // una partita, oppure il programma termina sotto scelta dell'utente.
    switch (choice) {
        case PROMPT_CARICA_PARTITA:
            partita = caricaPartita();
            break;
        case PROMPT_NUOVA_PARTITA:
            partita = creaPartita();
            break;
        case PROMPT_EXIT:
        default:
            printf("\nA presto!");
            exit(0);
    }

    // avvia la partita scelta
    avviaPartita(partita);
}

/**
 * Funzione che genera una partita a partire dalle informazioni fornite dal giocatore.
 * Le informazioni della partita sono restituite in una struttura "Salvataggio" apposita.
 *
 * @return La partita generata dalle informazioni fornite.
 */
Salvataggio creaPartita() {
    int i, j;
    // salvataggio da restituire
    Salvataggio partita;
    // boolean di appoggio per verificare che un nickname sia valido
    bool verificaNome = true;
    // carattere di appoggio per confermare il salvataggio
    char confermaSalvataggio;

    printf("\nBene, iniziamo allora! Bang! è un gioco multiplayer, a cui è possibile "
           "giocare in compagnia da un minimo di %d a un massimo di %d persone.\n"
           "Per cominciare, perché non mi dici quanti giocatori sarete?\n", MIN_PLAYERS, MAX_PLAYERS);

    // richiesta all'utente del numero dei giocatori che intendono partecipare
    do {
        printf("?) ");
        scanf("%d", &partita.nGiocatori);
        if(partita.nGiocatori < MIN_PLAYERS || partita.nGiocatori > MAX_PLAYERS)
            printf("Devi inserire un numero compreso tra %d e %d!\n", MIN_PLAYERS, MAX_PLAYERS);
    } while (partita.nGiocatori < MIN_PLAYERS || partita.nGiocatori > MAX_PLAYERS); // validazione dell'input

    // allocazione dinamica di un array contenente le strutture dei giocatori partecipanti
    partita.giocatori = (Giocatore*) calloc(partita.nGiocatori, sizeof(Giocatore));
    // verifica dell'allocazione dinamica
    assertPuntatoreNonNull(partita.giocatori, "\nErrore: allocazione dinamica dell'array dei giocatori fallita.");

    // richiesta all'utente dei nickname dei giocatori
    printf("Perfetto, dimmi un po' di più sui partecipanti!\n"
           "Cominciamo dai loro nomi: inserisci il nome di ogni giocatore, tenendo a mente che\n"
           "non possono contenere spazi ed hanno una lunghezza massima di %d caratteri.", NOME_UTENTE_LEN);
    for(i = 0; i < partita.nGiocatori; i++) {
        do {
            verificaNome = true;
            printf("\nGiocatore %d: ", i+1);
            scanf(" %49[^\n]s", partita.giocatori[i].nomeUtente);

            // verifico che il nome non sia vuoto
            if(strcmp(partita.giocatori[i].nomeUtente, "") != 0) {
                // verifico che il nome non contenga spazi
                for(j = 0; j < strlen(partita.giocatori[i].nomeUtente) && verificaNome; j++) {
                    if(partita.giocatori[i].nomeUtente[j] == ' ')
                        verificaNome = false;
                }
                if(verificaNome) {
                    // verifica che il nome non sia già stato inserito
                    for(j = i-1; j >= 0; j--) {
                        // il nome è già stato inserito!
                        if(strcmp(partita.giocatori[j].nomeUtente, partita.giocatori[i].nomeUtente) == 0) {
                            verificaNome = false;
                            printf("\nQuesto nome è già stato inserito! Riprova.");
                        }
                    }
                } else {
                    printf("\nIl tuo nome non può contenere spazi! Riprova.");
                }
            } else {
                printf("\nInserisci un nickname!");
            }
        } while (!verificaNome);

        // creo i mazzi dei giocatori
        partita.giocatori[i].carteMano = (Mazzo) {GIOC_MANO, 0, NULL};
        partita.giocatori[i].carteGioco = (Mazzo) {GIOC_GIOCO, 0, NULL};
    }

    // assegnazione dei ruoli dei giocatori
    printf("Piacere di conoscervi! Adesso genererò randomicamente i ruoli di ognuno di voi.\n");
    assegnaRuoli(partita.giocatori, partita.nGiocatori);

    // caricamento del mazzoPesca dal file "mazzo_bang.txt"
    printf("\nI ruoli sono stati assegnati! Carico il mazzo di pesca di carte dal file di testo...\n");
    Mazzo mazzoPesca = caricaMazzo();

    printf("\nMazzo caricato! Adesso distribuirò le carte dalla cima del mazzoPesca di pesca.\n"
           "Vi ricordo che ad ogni giocatore spetta un numero di carte pari al numero dei suoi punti vita di partenza.\n");

    // creazione di un mazzo vuoto che contiene le carte scartate
    Mazzo mazzoScarti = {MAZZO_SCARTO, 0, NULL};

    // distribuzione delle carte dal mazzoPesca ai giocatori
    distribuisciCartePartenza(&mazzoPesca, &mazzoScarti, partita.giocatori, partita.nGiocatori);

    // inserimento dei mazzi nelle info della partita
    partita.mazzoPesca = mazzoPesca;
    partita.mazzoScarti = mazzoScarti;

    // determinazione del primo giocatore che inizia, che è sempre lo sceriffo
    partita.prossimoGiocatore = 0;
    while(partita.giocatori[partita.prossimoGiocatore].ruoloAssegnato != SCERIFFO && partita.prossimoGiocatore < partita.nGiocatori)
        partita.prossimoGiocatore++;
    // verifico che lo sceriffo sia stato individuato, altrimenti c'è un problema nell'assegnazione
    if(partita.giocatori[partita.prossimoGiocatore].ruoloAssegnato != SCERIFFO) {
        printf("\nErrore di avvio: impossibile determinare lo sceriffo.");
        exit(-1);
    }

    // richiesta all'utente di un nome che identifichi il file di salvataggio
    do {
        printf("\nCi siamo quasi! Come ultima cosa, vorrei che mi dicessi un nome per il file di salvataggio\n"
               "in cui sarà memorizzata questa partita (max. %d caratteri)\n", SAVEGAME_NAME_LEN);
        printf("?) ");
        scanf(" %16s", partita.nomeSalvataggio);
        if(salvataggioEsistente(partita.nomeSalvataggio)) {
            // chiedo all'utente se vuole sovrascrivere il salvataggio
            do {
                printf("\nIl salvataggio esiste già! Vuoi sovrascriverlo?\n"
                       "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                scanf(" %c", &confermaSalvataggio);
                if(confermaSalvataggio == PROMPT_RIFIUTA) {
                    printf("\nInserisci un altro nome!");
                    strcpy(partita.nomeSalvataggio, ""); // annullo il salvataggio scelto
                } else if(confermaSalvataggio != PROMPT_RIFIUTA && confermaSalvataggio != PROMPT_CONFERMA) {
                    printf("\nInserisci un'azione valida!");
                }
            } while (confermaSalvataggio != PROMPT_RIFIUTA && confermaSalvataggio != PROMPT_CONFERMA);
        }
    } while (strcmp(partita.nomeSalvataggio, "") == 0);

    // scrittura del nuovo salvataggio
    scriviSalvataggio(partita, partita.nomeSalvataggio);

    printf("\nBene, è tutto pronto! Che la partita abbia inizio, e buona fortuna ai partecipanti!");

    return partita;
}

/**
 * Funzione che avvia una partita a partire dal salvataggio fornito.
 * Contiene tutta la logica del turno e della vittoria.
 *
 * @param partita Il salvataggio da cui avviare la partita.
 */
void avviaPartita(Salvataggio partita) {
    int i;
    // variabili booleane di appoggio per i cicli sottostanti
    bool ripetizioneCiclo, ripetiTurno = true;
    // variabili booleane che mantengono informazioni sulle carte giocate nei turni precedenti
    bool bangGiocato, cartaGiocata = false;
    // variabili di tipo int di appoggio per la selezioni delle carte da giocare e delle opzioni dei prompt durante il turno
    int promptTurnoScelta, posizioneCartaSelezionata;
    // variabile che contiene il numero di carte da scartare a fine turno
    int carteDaScartare;
    // variabili di tipo char di appoggio per la selezione delle opzioni nei prompt durante il turno
    char ruoloGiocatore[NOME_RUOLO_LEN_MAX + 1], tmpChoice;
    // variabile "Carta" contenente la carta da giocare in questo turno
    Carta cartaSelezionata;
    // carta contenente l'arma del giocatore corrente
    Carta armaGiocatore;
    // variabile contenente il ruolo dei vincitori al termine della partita
    Ruoli ruoloVincitore;
    // puntatore al giocatore del turno corrente
    Giocatore* giocatore = NULL;

    svuotaSchermo();
    svuotaBuffer();
    // inizio della logica della partita
    while(!partitaTerminata(partita, &ruoloVincitore)) { // verifico che la partita non sia terminata
        // scrittura del file di salvataggio
        scriviSalvataggio(partita, partita.nomeSalvataggio);

        // salvo il puntatore del giocatore corrente
        giocatore = &partita.giocatori[partita.prossimoGiocatore];

        printf("\n\n%s TURNO n° %d %s", MEZZO_SEPARATORE, partita.prossimoGiocatore + 1, MEZZO_SEPARATORE);
        scriviInizioTurnoSuLog(partita.prossimoGiocatore + 1);

        // salvo il nome del ruolo del giocatore.
        prendiNomeRuolo(giocatore->ruoloAssegnato, ruoloGiocatore);
        printf("\n%s, tocca a te giocare! Il tuo ruolo è '%s'.\n", giocatore->nomeUtente, ruoloGiocatore);

        // calcolo della gittata del giocatore
        giocatore->gittata = calcolaGittata(*giocatore);

        // all'inizio del turno, il giocatore pesca due carte
        pescaCarte(&partita.mazzoPesca, &partita.mazzoScarti, giocatore, 2);

        printf("\n%s CARTE IN GIOCO %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
        printf("Per prima cosa, verifico l'effetto delle tue carte in gioco...");
        // se le carte in gioco permettono al giocatore di continuare, allora inizia il turno
        if(verificaCarteInGioco(&partita.mazzoPesca, &partita.mazzoScarti, partita.prossimoGiocatore, partita.giocatori, partita.nGiocatori)) {
            printf("\n%s CARTE IN GIOCO %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            printf("\nPremi invio per continuare.");
            getchar();

            // imposto le variabili booleane ausiliarie
            bangGiocato = false;
            ripetiTurno = true;
            do {
                // il giocatore sceglie cosa fare all'inizio del turno
                do {
                    printf("\n%s MENU %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                    printf("\nScegli una delle seguenti azioni:\n"
                           "%d) Gioca una delle tue carte\n"
                           "%d) Vedi le tue carte in mano\n"
                           "%d) Vedi le tue carte in gioco\n"
                           "%d) Controlla la tua distanza dagli altri giocatori\n"
                           "%d) Vedi le carte in gioco degli altri giocatori\n"
                           "%d) Vedi la vita di tutti i giocatori\n"
                           "%d) Passa il turno\n"
                           "%d) Chiudi il gioco\n"
                           "?) ",
                           PROMPT_TURNO_GIOCA_CARTA, PROMPT_TURNO_VEDI_CARTE_MANO, PROMPT_TURNO_VEDI_CARTE_GIOCO, PROMPT_TURNO_VEDI_DISTANZE,
                           PROMPT_TURNO_VEDI_CARTE_GIOCO_ALTRI, PROMPT_VEDI_VITA_ALTRI, PROMPT_TURNO_PASSA_TURNO, PROMPT_TURNO_ESCI);
                    scanf("%d", &promptTurnoScelta);
                    ripetizioneCiclo = promptTurnoScelta != PROMPT_TURNO_GIOCA_CARTA &&
                                       promptTurnoScelta != PROMPT_TURNO_VEDI_CARTE_MANO &&
                                       promptTurnoScelta != PROMPT_TURNO_VEDI_CARTE_GIOCO &&
                                       promptTurnoScelta != PROMPT_TURNO_VEDI_DISTANZE &&
                                       promptTurnoScelta != PROMPT_TURNO_VEDI_CARTE_GIOCO_ALTRI &&
                                       promptTurnoScelta != PROMPT_VEDI_VITA_ALTRI &&
                                       promptTurnoScelta != PROMPT_TURNO_PASSA_TURNO &&
                                       promptTurnoScelta != PROMPT_TURNO_ESCI;
                    if (ripetizioneCiclo)
                        printf("\nInserisci una delle scelte mostrate!\n"
                               "?) ");
                } while (ripetizioneCiclo);

                // a seconda di quello che ha scelto il giocatore, effettuo l'azione corrispondente
                switch (promptTurnoScelta) {
                    // il giocatore ha deciso di giocare una carta
                    case PROMPT_TURNO_GIOCA_CARTA:
                        // verifico che il mazzo di mano non sia vuoto
                        if(giocatore->carteMano.numeroCarte > 0) {
                            // chiedo all'utente di scegliere una carta
                            do {
                                printf("\nBene, queste sono le carte nella tua mano!");
                                // mostro le carte nella mano del giocatore
                                mostraCarteMazzo(giocatore->carteMano);
                                printf("\nInserisci il numero della carta da selezionare: ");
                                scanf("%d", &posizioneCartaSelezionata);

                                // verifico che la scelta sia valida
                                if (posizioneCartaSelezionata <= 0 || posizioneCartaSelezionata > giocatore->carteMano.numeroCarte)
                                    printf("\nInserisci una posizione valida!");
                            } while (posizioneCartaSelezionata <= 0 || posizioneCartaSelezionata > giocatore->carteMano.numeroCarte);

                            cartaSelezionata = giocatore->carteMano.carte[posizioneCartaSelezionata - 1];

                            // 'Bang!' può essere giocata solo una volta (ammenoché non si possegga una 'Volcanic' come arma)
                            if (strcmp(cartaSelezionata.nomeCarta, "Bang!") == 0) {
                                armaGiocatore = prendiArmaGiocatore(*giocatore);
                                // il giocatore deve selezionare un'altra carta perché non può giocare un altro bang
                                if (bangGiocato && strcmp(armaGiocatore.nomeCarta, CARTA_VOLCANIC) != 0) {
                                    printf("\nHai già giocato un '%s' in questo turno!", CARTA_BANG);
                                    break; // scelta invalida, non possono essere giocati più bang
                                }
                            }

                            svuotaSchermo();

                            // la funzione 'giocaCarta' restituisce "False" se la carta non è stata giocata per qualsiasi motivo
                            // salvo in una variabile booleana un valore che indica se la carta è stata effettivamente giocata o meno
                            cartaGiocata = giocaCarta(
                                    &partita.mazzoPesca, &partita.mazzoScarti, partita.nGiocatori,
                                    partita.giocatori,
                                    partita.prossimoGiocatore, posizioneCartaSelezionata - 1);

                            if(cartaGiocata) {
                                // se la carta giocata è un 'Bang!', allora impedisco al giocatore di giocarla ancora
                                if (strcmp(cartaSelezionata.nomeCarta, CARTA_BANG) == 0)
                                    bangGiocato = true;
                                // se la carta non è della tipologia "in gioco" o "arma", scarto la carta dalla mano del giocatore
                                // potrebbe succedere che, nell'uccisione di uno sceriffo, un giocatore sia già obbligato a scartare le sue carte
                                if(cartaSelezionata.tipologiaCarta != ARMA &&
                                   cartaSelezionata.tipologiaCarta != EFFETTO &&
                                   giocatore->carteMano.numeroCarte > 0) {
                                    spostaCartaMazzo(&giocatore->carteMano, &partita.mazzoScarti, posizioneCartaSelezionata - 1);
                                }
                            }
                        } else {
                            printf("\nNon hai carte nella tua mano!");
                        }
                        break;
                        // seconda scelta: mostro le carte nella mano del giocatore
                    case PROMPT_TURNO_VEDI_CARTE_MANO:
                        mostraCarteMazzo(giocatore->carteMano);
                        svuotaBuffer();
                        break;
                        // terza scelta: mostro le carte in gioco del giocatore
                    case PROMPT_TURNO_VEDI_CARTE_GIOCO:
                        mostraCarteMazzo(giocatore->carteGioco);
                        svuotaBuffer();
                        break;
                        // quarta scelta: mostro la distanza tra il giocatore e gli altri
                    case PROMPT_TURNO_VEDI_DISTANZE:
                        mostraDistanze(partita.nGiocatori, partita.giocatori, partita.prossimoGiocatore);
                        svuotaBuffer();
                        break;
                        // quinta scelta: mostro le carte in gioco degli altri giocatori
                    case PROMPT_TURNO_VEDI_CARTE_GIOCO_ALTRI:
                        mostraCarteInGiocoAltri(partita.nGiocatori, partita.giocatori,partita.prossimoGiocatore);
                        svuotaBuffer();
                        break;
                    case PROMPT_VEDI_VITA_ALTRI:
                        mostraVitaGiocatori(partita.nGiocatori, partita.giocatori);
                        svuotaBuffer();
                        break;
                        // sesta scelta: passo il turno
                    case PROMPT_TURNO_PASSA_TURNO:
                        do {
                            printf("\nHai deciso di passare il turno! Sei sicuro?\n"
                                   "Ricorda che prima di passare, devi scartare le carte in eccesso.\n"
                                   "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                            scanf(" %c", &tmpChoice);
                            if(tmpChoice != PROMPT_CONFERMA && tmpChoice != PROMPT_RIFIUTA)
                                printf("\nInserisci un'azione valida!");
                        } while (tmpChoice != PROMPT_CONFERMA && tmpChoice != PROMPT_RIFIUTA);

                        if (tmpChoice == PROMPT_CONFERMA) {
                            if(giocatore->carteMano.numeroCarte > giocatore->puntiVita) {
                                carteDaScartare = giocatore->carteMano.numeroCarte - giocatore->puntiVita;
                                printf("\nPrima di passare il turno, devi scartare %d carte!", carteDaScartare);
                                // chiedo al giocatore di scartare le carte in eccesso
                                for(i = 0; i < carteDaScartare; i++) {
                                    scartaCarta(giocatore, &giocatore->carteMano, &partita.mazzoScarti);
                                }
                            }
                            printf("\nBene, passiamo il turno al prossimo giocatore!");
                            svuotaBuffer();
                            ripetiTurno = false;
                        }
                        break;
                        // ultima scelta: chiudo il gioco
                    case PROMPT_TURNO_ESCI:
                        do {
                            printf("\nSei sicuro di voler uscire?\n"
                                   "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                            scanf(" %c", &tmpChoice);
                            if(tmpChoice != PROMPT_CONFERMA && tmpChoice != PROMPT_RIFIUTA)
                                printf("\nInserisci un'azione valida!");
                        } while (tmpChoice != PROMPT_CONFERMA && tmpChoice != PROMPT_RIFIUTA);

                        // chiudo la partita salvando
                        if (tmpChoice == PROMPT_CONFERMA) {
                            chiudiGioco(&partita);
                        }
                }

                // aspetto che il giocatore prema invio per tornare al menu principale
                printf("\n\nPremi 'Invio' per tornare al menu principale.");
                getchar();
                svuotaSchermo();
            } while (ripetiTurno && !partitaTerminata(partita, &ruoloVincitore)); // se in una delle sue azioni il giocatore ha scelto "annulla", allora ripeti il ciclo di scelta
        }

        if(!partitaTerminata(partita, &ruoloVincitore)) {
            // turno terminato
            printf("\n\n%s TURNO TERMINATO %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            // seleziono il prossimo giocatore in vita per il turno successivo
            do {
                partita.prossimoGiocatore = (partita.prossimoGiocatore + 1) % partita.nGiocatori;
            } while(partita.giocatori[partita.prossimoGiocatore].puntiVita < 1);
            printf("\nIl prossimo giocatore sarà '%s'! Premi 'Invio' quando sei pronto a continuare.", partita.giocatori[partita.prossimoGiocatore].nomeUtente);
            // azioni per pulire lo schermo in vista del prossimo turno
            getchar();
            svuotaSchermo();
        }
    }
    // fine logica del turno

    // fine partita
    scriviSalvataggio(partita, partita.nomeSalvataggio);
    chiudiPartita(ruoloVincitore);
}

/**
 * Funzione che verifica l'azione delle carte in gioco di un giocatore.
 * Questa funzione viene chiamata ad ogni inizio turno, ed effettua le azioni dettate
 * dalle carte i gioco "dinamite" e "prigione". La funzione ha due valori di return:
 * VERO -> il giocatore può giocare il turno (non è morto per la dinamite o è evaso dalla prigione)
 * FALSO -> il giocatore non può giocare il turno (è morto per la dinamite o rimane in prigione)
 *
 * @param mazzoPesca Puntatore al mazzo di pesca.
 * @param mazzoScarti Puntatore al mazzo di scarti.
 * @param posizioneGiocatore Intero rappresentante la posizione del giocatore di cui verificare le carte.
 * @param giocatori Puntatore all'array dei giocatori.
 * @param nGiocatori Intero rappresentante il numero totale di giocatori.
 * @return True, se l'utente può giocare il turno, False altrimenti.
 */
bool verificaCarteInGioco(Mazzo* mazzoPesca, Mazzo* mazzoScarti, int posizioneGiocatore, Giocatore* giocatori, int nGiocatori) {
    int i;
    // variabile che contiene la posizione del giocatore a cui passare la dinamite
    int prossimoGiocatore;
    // carta estratta nella verifica delle carte in gioco
    Carta cartaEstratta;
    // puntatore al giocatore da verificare
    Giocatore* giocatore = &giocatori[posizioneGiocatore];

    // verifico in primis che il giocatore abbia delle carte in gioco
    if(giocatore->carteGioco.numeroCarte > 0) {
        int posizioneCartaDinamite = cercaCartaMazzoPerNome(giocatore->carteGioco, CARTA_DINAMITE);
        int posizioneCartaPrigione = cercaCartaMazzoPerNome(giocatore->carteGioco, CARTA_PRIGIONE);
        // ricerco la carta in gioco "dinamite"
        if(posizioneCartaDinamite != -1) {
            printf("\nHai in gioco una carta 'Dinamite'! Ora sarà estratta una carta che potrebbe farla esplodere o meno.");
            cartaEstratta = estraiCarta(mazzoPesca, mazzoScarti);
            scriviCartaEstrattaSuLog(giocatore->nomeUtente, cartaEstratta); // loggo la carta estratta

            // aspetto che l'utente prema invio
            printf("\nPremi invio per estrarre una carta.");
            getchar();

            // la carta estratta fa esplodere la dinamite
            if(cartaEstratta.numeroCarta >= 2 && cartaEstratta.numeroCarta <= 9 && cartaEstratta.semeCarta == PICCHE) {
                printf("\nBOOM! La dinamite è esplosa, quindi perdi 3 punti vita!");
                rimuoviPuntiVita(giocatore, NULL, 3, mazzoPesca, mazzoScarti);

                // scarto la carta dinamite
                spostaCartaMazzo(&giocatore->carteGioco, mazzoScarti, posizioneCartaDinamite);

                // verifico che il giocatore non sia morto, altrimenti è necessario finire il turno
                if(giocatore->puntiVita < 1) {
                    scriviDinamiteSuLog(giocatore->nomeUtente, cartaEstratta);
                    return false;
                }
            } else {
                // cerco il giocatore in vita immediatamente successivo al giocatore corrente
                // (non è necessario verificare che il successivo non abbia dinamiti dato che nel mazzo ce n'è solo una)
                i = posizioneGiocatore;
                do {
                    i = (i + 1) % nGiocatori;
                    prossimoGiocatore = i;
                } while (giocatori[prossimoGiocatore].puntiVita < 1);
                printf("\nScampato pericolo! La dinamite viene passata al prossimo giocatore possibile: %s.", giocatori[prossimoGiocatore].nomeUtente);
                // sposto la carta dinamite al mazzo del giocatore adiacente
                spostaCartaMazzo(&giocatore->carteGioco, &giocatori[prossimoGiocatore].carteGioco, posizioneCartaDinamite);
            }
            scriviDinamiteSuLog(giocatore->nomeUtente, cartaEstratta);
        }

        // ricerco la carta in gioco "prigione"
        if(posizioneCartaPrigione != -1) {
            printf("\nSei in prigione! Ora sarà estratta una carta: se il suo seme sarà di Cuori, potrai evadere, altrimenti salterai il turno!");
            cartaEstratta = estraiCarta(mazzoPesca, mazzoScarti);

            // aspetto che l'utente prema invio
            printf("\nPremi invio per estrarre una carta.");
            getchar();

            // scarto comunque la carta prigione
            spostaCartaMazzo(&giocatore->carteGioco, mazzoScarti, posizioneCartaDinamite);

            // verifico che il giocatore possa evadere
            if(cartaEstratta.semeCarta == CUORI) {
                printf("\nLa carta estratta è di cuori, puoi evadere di prigione!");
            } else {
                printf("\nOps, la carta estratta non è di cuori, quindi salterai il turno!");
                scriviPrigioneSuLog(giocatore->nomeUtente, cartaEstratta); // il giocatore non è evaso, quindi scrivo su log ed esco
                return false;
            }
            scriviPrigioneSuLog(giocatore->nomeUtente, cartaEstratta);
        }
    } else {
        printf("\nNon hai nessuna carta in gioco!");
    }

    return true; // il giocatore non è morto e non deve saltare il turno
}

/**
 * Funzione che verifica che la partita sia terminata (con la vittoria dei giocatori/fuorilegge/sceriffo)
 * verificandone lo stato, restituendo true o false di conseguenza.
 * Il secondo parametro rappresenta un puntatore a una variabile di tipo "Ruoli", che, nel caso di vittoria,
 * conterrà il ruolo che ha vinto la partita.
 * Se la funzione restituisce "False", allora ruoloVincitore assumerà il valore "-1".
 *
 * @param partita Il salvataggio della partita da verificare.
 * @param ruoloVincitore Puntatore alla variabile contenente il ruolo dei vincitori, o "-1" se la partita non è finita.
 * @return True se la partita è terminata, False altrimenti.
 */
bool partitaTerminata(Salvataggio partita, Ruoli* ruoloVincitore) {
    // variabile contatore
    int i;
    // variabili contatori per tutte le tipologie di giocatori ancora in vita
    int nSceriffi = 0, nFuorilegge = 0, nVice = 0, nPlayer = 0, nRinnegati = 0;

    // iterazione sull'array di giocatori
    for(i = 0; i < partita.nGiocatori; i++) {
        if(partita.giocatori[i].puntiVita > 0) {
            // comincio la conta dei giocatori per categoria
            switch (partita.giocatori[i].ruoloAssegnato) {
                case SCERIFFO:
                    nSceriffi++;
                    break;
                case FUORILEGGE:
                    nFuorilegge++;
                    break;
                case VICESCERIFFO:
                    nVice++;
                    break;
                case RINNEGATO:
                    nRinnegati++;
                    break;
                default:
                    nPlayer++;
                    break;
            }
        }
    }

    // se una delle condizioni seguenti è vera, allora la partita è terminata, altrimenti è possibile continuare
    // prima verifica: vittoria degli sceriffi
    if(nSceriffi > 0 && nFuorilegge == 0) {
        *ruoloVincitore = SCERIFFO;
    // seconda verifica: vittoria dei fuorilegge
    } else if(nSceriffi == 0 && nFuorilegge > 0) {
        *ruoloVincitore = FUORILEGGE;
    // terza verifica: vittoria dei rinnegati
    } else if(nSceriffi == 0 && nFuorilegge == 0 && nVice == 0 && nPlayer == 0 && nRinnegati > 0) {
        *ruoloVincitore = RINNEGATO;
    } else {
        *ruoloVincitore = -1; // dato che la partita non è terminata, il ruolo vincitore viene settato all'intero "-1"
    }

    return *ruoloVincitore != -1; // restituisco vero o falso a seconda che sia stato determinato o meno un vincitore
}

/**
 * Funzione che richiede all'utente le informazioni relative a un salvataggio da caricare,
 * restituendo poi una struttura "Salvataggio" con le informazioni lette dal file "savegame.bin" relativo.
 * Se il salvataggio non viene trovato, all'utente viene chiesto nuovamente di indicare un nome.
 *
 * @return Il salvataggio caricato dal file.
 */
Salvataggio caricaPartita() {
    // nome del salvataggio da caricare
    char nomeSalvataggio[SAVEGAME_NAME_LEN + 1];
    // booleano di supporto per verificare che un salvataggio da caricare esista
    bool savegameEsistente = true;
    // char di supporto per confermare il caricamento di un salvataggio
    char conferma;

    // verifico l'esistenza del file con la lista dei salvataggi
    if(fileEsistente(SAVEGAME_LIST_FILE)) {
        // stampa a schermo del file contenente la lista dei salvataggi
        stampaSalvataggiInLista();
    }

    do {
        printf("\nInserisci il nome del file in cui è stato scritto il salvataggio.\n"
               "?) ");
        scanf(" %16s", nomeSalvataggio);
        if(strcmp(nomeSalvataggio, "") == 0) {
            printf("\nInserisci un nome valido");
        } else {
            savegameEsistente = salvataggioEsistente(nomeSalvataggio);
            if(!savegameEsistente) {
                printf("\nImpossibile caricare il salvataggio scelto.");
            }
        }
    } while(strcmp(nomeSalvataggio, "") == 0 || !savegameEsistente);

    // conferma del salvataggio scelto
    do {
        printf("\nHai selezionato il salvataggio '%s'. Desideri caricarlo?\n"
               "%c/%c) ", nomeSalvataggio, PROMPT_CONFERMA, PROMPT_RIFIUTA);
        scanf(" %c", &conferma);
        if(conferma != PROMPT_CONFERMA && conferma != PROMPT_RIFIUTA)
            printf("\nInserisci un'azione valida!");
    } while(conferma != PROMPT_CONFERMA && conferma != PROMPT_RIFIUTA);

    // se l'utente ha confermato, carico il salvataggio, altrimenti chiudo il gioco
    if(conferma == PROMPT_CONFERMA) {
        printf("\nCaricamento del salvataggio '%s'...", nomeSalvataggio);
        return caricaSalvataggio(nomeSalvataggio);
    } else {
        printf("\nUscita dal gioco.");
        exit(0);
    }
}

/**
 * Subroutine che assegna randomicamente i ruoli di ogni giocatore.
 * La distribuzione dei ruoli segue il numero dei partecipanti:
 * * 4 giocatori -> 1 sceriffo, 1 rinnegato, 2 fuorilegge, 0 vicesceriffi
 * * 5 giocatori -> 1 sceriffo, 1 rinnegato, 2 fuorilegge, 1 vicesceriffo
 * * 6 giocatori -> 1 sceriffo, 1 rinnegato, 3 fuorilegge, 1 vicesceriffo
 * * 7 giocatori -> 1 sceriffo, 1 rinnegato, 3 fuorilegge, 2 vicesceriffi
 *
 * @param giocatori L'array contenente i giocatori.
 * @param nGiocatori Il numero totale di giocatori.
 */
void assegnaRuoli(Giocatore giocatori[], int nGiocatori) {
    int i;
    // intero generato randomicamente, che indica un ruolo (da 0 a 3)
    int ruoloGenerato;
    // definizione dell'array contenente i ruoli dei giocatori
    int ruoliGiocatori[ROLES_NUMBER];
    // variabile ausiliaria per verificare che la generazione è terminata
    bool generazioneTerminata = false;

    // il numero di sceriffi e di rinnegati è sempre uguale
    ruoliGiocatori[SCERIFFO] = 1;
    ruoliGiocatori[RINNEGATO] = 1;

    // calcolo del numero di guorilegge e vicesceriffi in base al numero di giocatori
    switch (nGiocatori) {
        case 4:
            ruoliGiocatori[FUORILEGGE] = 2;
            ruoliGiocatori[VICESCERIFFO] = 0;
            break;
        case 5:
            ruoliGiocatori[FUORILEGGE] = 2;
            ruoliGiocatori[VICESCERIFFO] = 1;
            break;
        case 6:
            ruoliGiocatori[FUORILEGGE] = 3;
            ruoliGiocatori[VICESCERIFFO] = 1;
            break;
        case 7:
            ruoliGiocatori[FUORILEGGE] = 3;
            ruoliGiocatori[VICESCERIFFO] = 2;
            break;
        default:
            printf("\nErrore: numero di giocatori non riconosciuto, impossibile assegnare i ruoli. Arresto.");
            exit(-1);
    }

    // stampa a schermo della distribuzione dei ruoli
    printf("Essendo il numero dei partecipanti %d, i ruoli saranno divisi come segue:\n"
           "Sceriffi: %d\n"
           "Rinnegati: %d\n"
           "Fuorilegge: %d\n"
           "Vicesceriffi: %d\n",
           nGiocatori, ruoliGiocatori[SCERIFFO], ruoliGiocatori[RINNEGATO], ruoliGiocatori[FUORILEGGE], ruoliGiocatori[VICESCERIFFO]);

    // iterazione sui giocatori
    for(i = 0; i < nGiocatori; i++) {
        generazioneTerminata = false;
        do {
            // dato che i valori dei ruoli vanno sempre da 0 a 3, la generazione deve essere solo per questi tre valori
            ruoloGenerato = rand() % 4;
            // se ruoliGiocatori[ruoloGenerato] > 0, ci sono ancora dei "posti liberi" per assegnare quel ruolo
            if(ruoliGiocatori[ruoloGenerato] > 0) {
                // un ruolo uguale a "ruoloGenerato" è stato assegnato, quindi si diminuisce di 1 il numero di disponibili
                ruoliGiocatori[ruoloGenerato] -= 1;
                // assegnazione ruolo
                giocatori[i].ruoloAssegnato = ruoloGenerato;
                // calcolo dei punti vita
                if(ruoloGenerato == SCERIFFO) {
                    printf("\n%s sarà lo sceriffo di questa partita!\n", giocatori[i].nomeUtente);
                    giocatori[i].puntiVita = PUNTI_VITA_SCERIFFO;
                } else {
                    giocatori[i].puntiVita = PUNTI_VITA_GENERICO;
                }
                generazioneTerminata = true; // ruolo assegnato, si passa al prossimo giocatore
            }
        } while (!generazioneTerminata); // continua a generare finché non trovo un ruolo da assegnare
    }
}

/**
 * Subroutine che distribuisce le prime carte del mazzo caricato ai giocatori.
 * Ogni giocatore possiede, inizialmente, tante carte quante sono i suoi punti vita.
 * La subroutine modifica il mazzo fornito scartando dalla cima le carte distribuite ai giocatori,
 * e il campo "carteMano" della struttura di ogni giocatore, allocata dinamicamente per ognuno di essi.
 * Se l'allocazione dinamica del campo fallisce, il programma si arresta.
 *
 * @param mazzoPesca Puntatore al mazzo di pesca da cui prendere le carte da distribuire.
 * @param mazzoScarti Puntatore al mazzo delle carte degli scarti.
 * @param giocatori I giocatori a cui distribuire le carte.
 * @param nGiocatori Il numero di giocatori a cui distribuire le carte.
 */
void distribuisciCartePartenza(Mazzo *mazzoPesca, Mazzo *mazzoScarti, Giocatore *giocatori, int nGiocatori) {
    int i;

    // iterazione sui giocatori
    for(i = 0; i < nGiocatori; i++) {
        // a ogni giocatore vengono assegnate le carte dalla cima del mazzo, per un numero totale pari ai suoi punti vita
        pescaCarte(mazzoPesca, mazzoScarti, &giocatori[i], giocatori[i].puntiVita);
    }
}

void chiudiPartita(Ruoli ruoloVincitore) {
    // stringa contenente il nome del ruolo vincitore
    char nomeRuolo[NOME_RUOLO_LEN_MAX + 1];

    switch (ruoloVincitore) {
        case SCERIFFO:
            strcpy(nomeRuolo, "Sceriffi");
            break;
        case VICESCERIFFO:
            strcpy(nomeRuolo, "Vicesceriffi");
            break;
        case FUORILEGGE:
            strcpy(nomeRuolo, "Fuorilegge");
            break;
        case RINNEGATO:
            strcpy(nomeRuolo, "Rinnegati");
            break;
    }

    svuotaSchermo();
    printf("\n%s PARTITA TERMINATA %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
    printf("\nQuesta partita di 'Bang!' è terminata!\n");
    printf("\nCongratulazioni ai vincitori di questa partita: '%s'!", nomeRuolo);
    printf("\n%s PARTITA TERMINATA %s\n\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
    scriviVittoriaSuLog(nomeRuolo);
}

/**
 * Chiude il gioco salutando il giocatore, se questo ha deciso di interrompere la partita in un prompt.
 * Prima della chiusura, il salvataggio viene scritto su file, e le strutture allocate dinamicamente vengono deallocate.
 *
 * @param Puntatore alla partita corrente.
 */
void chiudiGioco(Salvataggio* partita) {
    // salvataggio
    scriviSalvataggio(*partita, partita->nomeSalvataggio);
    
    // deallocazione
    liberaStrutture(partita);

    printf("\nGrazie per aver giocato, a presto!");
    exit(0);
}

/**
 * Subroutine chiamata al termine di una partita per deallocare la memoria.
 *
 * @param Puntatore alla partita attualmente terminata, contenente le strutture da liberare.
 */
void liberaStrutture(Salvataggio* partita) {
    // primo passo: liberazione delle carte dei mazzi
    if(partita->mazzoPesca.numeroCarte > 0)
        free(partita->mazzoPesca.carte);
    if(partita->mazzoScarti.numeroCarte > 0)
        free(partita->mazzoScarti.carte);

    // secondo passo: liberazione delle carte nei mazzi dei giocatori
    for(int i = 0; i < partita->nGiocatori; i++) {
        if(partita->giocatori[i].carteMano.numeroCarte > 0)
            free(partita->giocatori[i].carteMano.carte);
        if(partita->giocatori[i].carteGioco.numeroCarte > 0)
            free(partita->giocatori[i].carteGioco.carte);
    }

    // terzo passo: liberazione dell'array dei giocatori
    free(partita->giocatori);
}