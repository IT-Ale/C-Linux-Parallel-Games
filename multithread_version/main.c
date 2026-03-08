#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>

#include "main.h"
#include "alieno.h"
#include "difese.h"
#include "thread_functions.h"


struct Message buffer[DIM_BUFFER];	// Buffer circolare condiviso tra i thread
int i_scrivi = 0;   				// Indice di scrittura nel buffer
int i_leggi = 0;    				// Indice di lettura nel buffer

// Primitive di sincronizzazione
pthread_mutex_t mutex_buffer;
sem_t sem_liberi, sem_occupati;


/*
 * Funzione principale del programma
 * - inizializza ncurses
 * - crea i thread di alieno e difese
 * - avvia la gestione dell’area di gioco
 * - gestisce la chiusura del programma e stampa l’esito finale
 */
int main() {
    pthread_t tid_alieno, tid_difese;  // Thread per alieno e difese
    int game_end_status = 1;           // Stato finale del gioco

    // Inizializza il generatore di numeri casuali
    srand((int)time(NULL));

    // Inizializzazione ncurses
    initscr();
    noecho();       // Disabilita la stampa dei caratteri digitati
    curs_set(0);    // Nasconde il cursore
    cbreak();       // Input immediato senza buffer di linea

    // Crea la finestra di gioco
    WINDOW *w = newwin(WGAME_MAX_Y, WGAME_MAX_X, WGAME_MIN_Y, WGAME_MIN_X);
    box(w, ACS_VLINE, ACS_HLINE);   // Disegna il bordo
    keypad(stdscr, true);           // Abilita i tasti speciali (es. frecce)

	// Inizializza mutex e semafori
    init_sync();
    
    // Avvia i thread dell'alieno e delle difese
    pthread_create(&tid_alieno, NULL, gestione_alieno, NULL);
    pthread_create(&tid_difese, NULL, gestione_difese, NULL);

    // Avvia il loop principale di gestione dell'area di gioco
    game_end_status = gestione_area_gioco(w);
    
	
	// 1. Dico ai thread di smettere immediatamente
	pthread_cancel(tid_alieno);
	pthread_cancel(tid_difese);

	// 2. Aspetto che abbiano terminato le operazioni di chiusura
	pthread_join(tid_alieno, NULL);
	pthread_join(tid_difese, NULL);
    
	
	// Dealloca meccanismi di sincronizzazione
	destroy_sync();

    // Chiude la modalità ncurses
    endwin();

    // Stampa l'esito della partita
    if(game_end_status == -1) printf("Game Over\n");
    else if(game_end_status == 0) printf("You Win\n");

    return 0;
}

/*
 * Funzione che gestisce l'area di gioco e la logica delle collisioni
 * - legge i messaggi dal buffer condiviso
 * - aggiorna la posizione grafica di alieno, difese, laser e proiettili
 * - rileva le collisioni tra entità
 * - aggiorna le vite e determina le condizioni di fine partita
 */
int gestione_area_gioco(WINDOW* w) {
    struct Message msg_in;   // Messaggio letto dal buffer

    // Posizioni correnti delle entità
    struct Message pos_alieno = {2, 2, '='};
    struct Message pos_difese = {WGAME_MAX_X-3, WGAME_MAX_Y-WGAME_MIN_Y, '^'};
    struct Message pos_laser = {0,0,0};
    struct Message pos_bullet = {0,0,0};

    int vite_alieno = 3;   // Vite dell'alieno
    int vite_difese = 3;   // Vite delle difese
    int end_condition = 1; // Stato del gioco: 1=continua, 0=vittoria, -1=sconfitta

    // Loop principale del gioco
    while(end_condition == 1) {

        // Legge il prossimo messaggio dal buffer in modo sincronizzato
        buffer_read(&msg_in);

        // Stampa i contatori delle vite
        mvprintw(1,2,"Vita Alieno: %3d",vite_alieno);
        mvprintw(2,2,"Vita Difese: %3d",vite_difese);
        box(w, ACS_VLINE, ACS_HLINE);
        refresh();

		// Gestione del tipo di entità in base al carattere
        switch(msg_in.c) {
        	case '=':
        		// Aggiornamento posizione alieno
            	mvwaddch(w, pos_alieno.y, pos_alieno.x, ' '); // Cancella vecchia posizione
            	pos_alieno = msg_in;						  // Aggiorna posizione	
            	mvwaddch(w, pos_alieno.y, pos_alieno.x, pos_alieno.c); // Disegna nuova
        		break;
        	case '^':
        		// Aggiornamento posizione difese
            	mvwaddch(w, pos_difese.y, pos_difese.x, ' ');
            	pos_difese = msg_in;
            	mvwaddch(w, pos_difese.y, pos_difese.x, pos_difese.c);
        		break;
        	case '|':
        		// Aggiornamento posizione laser dell’alieno
            	mvwaddch(w, pos_laser.y, pos_laser.x, ' ');
            	pos_laser = msg_in;
            	mvwaddch(w, pos_laser.y, pos_laser.x, pos_laser.c);
            	
            	// Controllo collisione laser-alieno
            	if ((pos_alieno.x == pos_laser.x) && (pos_alieno.y == pos_laser.y)) {
            		flash();
                	vite_alieno--;              // Riduce vita alieno
            	}
        		break;
        		
        	case 'o':
        		// Aggiornamento posizione proiettile delle difese
            	mvwaddch(w, pos_bullet.y, pos_bullet.x, ' ');
            	pos_bullet = msg_in;
            	mvwaddch(w, pos_bullet.y, pos_bullet.x, pos_bullet.c);

            	// Controllo collisione proiettile -> difese
            	if ((pos_difese.x == pos_bullet.x) && (pos_difese.y == pos_bullet.y)) {
            		flash();
            	    vite_difese--;             // Riduce vita difese
            	}
        		break;
        }
        

        // Verifica condizioni di fine partita
        if(vite_difese == 0) end_condition = -1;   // Sconfitta
        else if(vite_alieno == 0) end_condition = 0; // Vittoria

        // Ridisegna il bordo e aggiorna la finestra
        box(w, ACS_VLINE, ACS_HLINE);
        wrefresh(w);
    }

    return end_condition;
}

