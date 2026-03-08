#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ncurses.h>

#include "difese.h"
#include "main.h"

/*
 * Funzione principale che gestisce le difese:
 * - legge input da tastiera (frecce, spazio)
 * - aggiorna la posizione della nave delle difese
 * - crea un processo figlio per il laser quando si spara
 * - invia costantemente lo stato al processo padre tramite pipe
 */
void gestione_difese(int pipe_out) {

    // Messaggio iniziale: posizione iniziale delle difese
    struct Message msg = {WGAME_MAX_X-3, WGAME_MAX_Y-WGAME_MIN_Y, '^'};

    int act = -11;         // Codice dell’azione (tasto premuto)
    int pid_laser;         // PID del processo laser
    int laser_active = 0;  // Flag: 1 se il laser è attivo, 0 altrimenti
    int status;            // Stato del processo figlio (laser)
    int result;            // Risultato di waitpid

	

    // Ciclo infinito di gestione input e aggiornamento posizione
    while(1) {

        // Legge un carattere dalla tastiera (ncurses)
        act = (int)getch();

        switch (act) {
            case KEY_RIGHT:
                // Sposta le difese a destra, se non oltre il bordo
                if (msg.x < WGAME_MAX_X-2) 
                	msg.x += 1;
                break;

            case KEY_LEFT:
                // Sposta le difese a sinistra, se non oltre il bordo
                if (msg.x > 1) 
                	msg.x -= 1;
                break;

            case (int)' ':
                // Tasto spazio: spara un laser
                if (laser_active == 0) {   // Solo se non ce n'è già uno attivo
                    laser_active = 1;
                    
                    pid_laser = fork();    // Crea processo figlio per il laser

                    if (pid_laser == -1)
                        perror("Errore nella fork: Laser");
                    else if (pid_laser == 0)
                        // Ramo figlio: esegue il movimento del laser
                        gestione_laser(pipe_out, msg);
                }
                break;
        }

		
        result = waitpid(pid_laser,&status,WNOHANG);
       
        
        if (result == pid_laser)
            laser_active = 0;  // Il laser è terminato, se ne può creare un altro

        
	
        write(pipe_out,&msg,sizeof msg);
        
    }

    // Uscita di sicurezza (in realtà non viene mai raggiunta)
    _exit(0);
}

/*
 * Funzione che gestisce il laser:
 * - viene eseguita in un processo figlio
 * - fa muovere il laser verso l’alto
 * - invia continuamente la posizione al processo padre tramite pipe
 */
void gestione_laser(int pipe_out, struct Message pos_origin) {

    int curr_pid = getpid();   // PID del processo laser

    // Messaggio iniziale del laser:
    // include il PID per poter essere terminato dal padre in caso di collisione
    struct Message msg = {pos_origin.x, pos_origin.y-1, '|', curr_pid};

    // Il laser si muove verso l'alto finché non esce dallo schermo
    while(msg.y > 0) {

        msg.y = msg.y - 1;               	// Spostamento verso l’alto
        
		
            write(pipe_out,&msg,sizeof msg);
        
        
        usleep(UDELAY_LASER);            	// Ritardo per controllare la velocità
    }

    // Terminazione del processo laser
    _exit(0);
}
