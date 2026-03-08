#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <pthread.h>

#include "difese.h"
#include "main.h"
#include "thread_functions.h"

/*
 * Funzione che gestisce il movimento della difesa (giocatore)
 * - aggiorna la posizione della nave delle difese
 * - crea un thread per il laser quando si spara
 * - scrive lo stato sul buffer condiviso
 */
void* gestione_difese(void* arg) {
    int act;                 // Variabile per memorizzare l'input da tastiera
    pthread_t tid_laser;     // Thread ID del laser
    int laser_active = 0;    // Flag per evitare laser multipli

    // Posizione iniziale della difesa
    struct Message msg = {WGAME_MAX_X-3, WGAME_MAX_Y-WGAME_MIN_Y, '^'};

    // Scrive la difesa nel buffer condiviso
    buffer_write(msg);

    // Loop principale del thread
    while(1) {
        // Legge un tasto dalla tastiera
        act = (int)getch();

        switch(act) {
            case KEY_RIGHT:
                // Spostamento a destra
                if(msg.x < WGAME_MAX_X-2)
                    msg.x += 1;
                break;

            case KEY_LEFT:
                // Spostamento a sinistra, se non si supera il bordo
                if(msg.x > 1) 
                    msg.x -= 1;
                break;

            case (int)' ':
                // Sparo con barra spaziatrice
                if(laser_active == 0) {
                    laser_active = 1;  // Imposta il flag per evitare doppio sparo

                    // Alloca dinamicamente una Message da passare al thread laser
                    struct Message* laser_msg = malloc(sizeof(struct Message));
                    *laser_msg = msg;   // Copia la posizione corrente della difesa

                    // Crea il thread che gestisce il laser
                    pthread_create(&tid_laser, NULL, gestione_laser, (void*)laser_msg);
                    pthread_detach(tid_laser);  // Il thread si libera automaticamente alla fine

                    laser_active = 0;  // Riabilita lo sparo
                }
                break;
        }

        // Scrive nel buffer la nuova posizione della difesa
        buffer_write(msg);
    }

    return NULL;
}

/*
 * Funzione che gestisce il laser delle difese
 * - muove il laser verticalmente verso l’alto
 * - scrive aggiornamenti nel buffer
 */
void* gestione_laser(void* arg) {
    // Recupera messaggio passato dal thread principale
    struct Message* msg_ptr = (struct Message*)arg;
    struct Message msg = *msg_ptr;  // Copia locale del messaggio
    free(msg_ptr);                  // Libera la memoria allocata dinamicamente

    // Imposta il carattere del laser
    msg.c = '|';

    // Movimento verticale del laser verso l'alto
    while(msg.y > 0) {
        msg.y -= 1;
        buffer_write(msg);
        usleep(UDELAY_LASER);  // Ritardo per controllare la velocità del laser
    }

    return NULL;
}

