#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "alieno.h"
#include "main.h"

/*
 * Funzione principale che gestisce l'alieno:
 * - muove l’alieno orizzontalmente avanti e indietro
 * - dopo un certo numero di cicli spara un proiettile
 * - comunica continuamente la posizione al processo padre tramite pipe
 */
void gestione_alieno(int pipe_out) {

    // Messaggio iniziale dell’alieno:
    struct Message msg = {2, 2, '='};

    int x_dir = -1;   // Direzione di movimento sull'asse X

    int cycles_to_wait = 8;       // Numero di cicli prima di poter sparare
    int cycles_permormed = 0;     // Contatore dei cicli eseguiti

    int pid_bullet;               // PID del processo bullet
    int bullet_active = 0;        // Flag: 1 se un proiettile è attivo, 0 altrimenti
    int status;                   // Stato del processo figlio
    int result;                   // Risultato di waitpid



    // Ciclo infinito di aggiornamento dell’alieno
    while(1) {

        // Calcola la nuova posizione X in base alla direzione
        int new_x = msg.x + x_dir;

        // Se si raggiungono i bordi dell’area di gioco, inverte la direzione
        if (new_x >= WGAME_MAX_X-1 || new_x <= WGAME_MIN_X)
            x_dir = x_dir * -1;

        // Aggiorna la posizione dell’alieno
        msg.x = msg.x + x_dir;

        // Incrementa il contatore dei cicli
        cycles_permormed += 1;

        // Se è passato abbastanza tempo e non c’è già un proiettile attivo, spara
        if (cycles_permormed > cycles_to_wait && bullet_active == 0) {
            cycles_permormed = 0;
            bullet_active = 1;

            // Crea un processo figlio per il proiettile
            pid_bullet = fork();
            if (pid_bullet == -1)
                perror("Errore nella creazione del processo bullet");
            else if (pid_bullet == 0)
                // Ramo figlio: gestisce il movimento del proiettile
                gestione_bullet(pipe_out, msg);
        }

		
		//Aspetto il processo proiettile
		result = waitpid(pid_bullet,&status,WNOHANG);
       *
        
        if (result == pid_bullet)
            bullet_active = 0;   // Il proiettile è terminato, se ne può creare un altro

		
        write(pipe_out,&msg,sizeof msg);
        
        
        // Ritardo per controllare la velocità di movimento dell’alieno
        usleep(UDELAY_ALIEN);
    }

    // Uscita di sicurezza (non viene mai raggiunta)
    _exit(0);
}

/*
 * Funzione che gestisce il proiettile dell’alieno:
 * - viene eseguita in un processo figlio
 * - fa muovere il proiettile verso il basso
 * - invia la posizione al processo padre tramite pipe
 */
void gestione_bullet(int pipe_out, struct Message pos_origin) {

    int curr_pid = getpid();   // PID del processo proiettile

    // Messaggio iniziale del proiettile:
    struct Message msg = {pos_origin.x, pos_origin.y+1, 'o', curr_pid};

    // Il proiettile si muove verso il basso finché non esce dall’area di gioco
    while(msg.y <= WGAME_MAX_Y) {

        msg.y = msg.y + 1;               	// Spostamento verso il basso

		
        write(pipe_out,&msg,sizeof msg);  // Scrive
        
        
        usleep(UDELAY_BULLET);           	// Ritardo per controllare la velocità
    }

    // Terminazione del processo proiettile
    _exit(0);
}

