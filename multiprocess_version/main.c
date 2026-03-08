#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <ncurses.h>

#include "main.h"
#include "alieno.h"
#include "difese.h"

/*
 * Funzione principale del programma
 * - inizializza ncurses
 * - crea i processi di alieno e difese
 * - avvia la gestione dell’area di gioco
 * - gestisce la chiusura del programma e stampa l’esito finale
 */
int main() {
    int pid_alieno, pid_difese;    // PID dei processi figlio
    int game_end_status = 1;       // Stato di fine gioco: 1=continua, 0=vittoria, -1=sconfitta
    
	// ***** COMPLETARE QUI ***** 
	// Crezione pipe
	int pipe_fd[2];  // Dichiarazione
	if(pipe(pipe_fd)== -1){   //Creazione
		perror("Errore crezione PIPE");
		exit(1);
	}
	// ***** FINE MODIFICA *****
	

    // Inizializza il generatore di numeri casuali
    srand((int)time(NULL));

    // Inizializzazione ncurses
    initscr();      // Avvia modalità ncurses
    noecho();       // Non mostra i caratteri digitati
    curs_set(0);    // Nasconde il cursore
    cbreak();       // Input immediato da tastiera

    // Creazione finestra di gioco
    WINDOW *w = newwin(WGAME_MAX_Y, WGAME_MAX_X, WGAME_MIN_Y, WGAME_MIN_X);
    box(w, ACS_VLINE, ACS_HLINE);  // Disegna il bordo della finestra
    keypad(stdscr, true);         // Abilita tasti speciali (frecce, ecc.)


	// ***** COMPLETARE QUI ***** 
	wrefresh(w);
    // ***** FINE MODIFICA *****
    
    
    // Creazione del processo Alieno
    pid_alieno = fork();
    if(pid_alieno == -1) { 
        perror("Errore fork alieno"); 
        _exit(2); 
    } else if(pid_alieno == 0) { 
        // Ramo Figlio: processo Alieno
        close(pipe_fd[0]);              // Chiude lettura della pipe
        gestione_alieno(pipe_fd[1]);    // Avvia la logica della nave aliena
    } else {
        // Creazione del processo Difese
        pid_difese = fork();
        if(pid_difese == -1) { 
            perror("Errore fork difese"); 
            _exit(3); 
        } else if(pid_difese == 0) { 
            // Ramo Figlio: processo Difese
            close(pipe_fd[0]);              // Chiude lettura della pipe
            gestione_difese(pipe_fd[1]);    // Avvia la logica del giocatore
        } else {    
            // Ramo Padre: processo di controllo del gioco
            close(pipe_fd[1]);                         // Chiude scrittura della pipe
            game_end_status = gestione_area_gioco(w, pipe_fd[0]); // Loop di controllo
        }
    }
	
	// ***** COMPLETARE QUI *****
	//Uccido
	kill(pid_alieno,SIGKILL);
	kill(pid_difese,SIGKILL);
	// Aspetto i filgi
	waitpid(pid_alieno,NULL,0);
	waitpid(pid_difese,NULL,0);
	// ***** FINE MODIFICA *****
	
	
    // Chiusura modalità ncurses
    endwin();

    // Messaggio finale
    if(game_end_status == -1) printf("Game Over\n");
    else if(game_end_status == 0) printf("You Win\n");

    return 0;
}

/* Gestisce l’area di gioco:
 * - legge i messaggi dalla pipe
 * - aggiorna le posizioni di alieno, difese e proiettili
 * - controlla collisioni e vite
 */
int gestione_area_gioco(WINDOW* w, int pipe_in) {
    struct Message msg_in;                 // Messaggio ricevuto dalla pipe
    struct Message pos_alieno, pos_difese, pos_laser, pos_bullet; // Posizioni attuali
    int vite_alieno = 3;                  // Vite iniziali alieno
    int vite_difese = 3;                  // Vite iniziali difese
    int end_condition = 1;                // Stato del gioco

    // Ciclo principale del gioco
    while(end_condition == 1) {
    
		// ***** COMPLETARE QUI *****
    	read(pipe_in,&msg_in,sizeof msg_in);  //Leggo quello che scrivono i processi figli
		// ***** FINE MODIFICA *****
		
		
        // Stampa contatori delle vite
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
                	vite_alieno -= 1;              // Riduce vita alieno
                	kill(pos_laser.pid, 1);        // Termina processo del laser
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
            	    vite_difese -= 1;             // Riduce vita difese
            	    kill(pos_bullet.pid, 1);      // Termina processo del proiettile
            	}
				break;
        }

        // Verifica condizioni di fine partita
        if(vite_difese == 0) end_condition = -1;     // Sconfitta
        else if(vite_alieno == 0) end_condition = 0; // Vittoria

        // Ridisegno e aggiorno la finestra
        box(w, ACS_VLINE, ACS_HLINE);
        wrefresh(w);
    }

	// ***** COMPLETARE QUI ***** 

	// ***** FINE MODIFICA *****
				
 	// Ritorna esito del gioco
    return end_condition;  
}
