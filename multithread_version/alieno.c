#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "alieno.h"
#include "main.h"
#include "thread_functions.h"

/*
 * Funzione che gestisce il movimento dell’alieno
 * - muove l’alieno orizzontalmente con cambio di direzione ai bordi
 * - gestisce il tempo tra uno sparo e l’altro
 * - crea un thread per il proiettile dell’alieno
 * - scrive lo stato dell’alieno nel buffer
 */
void* gestione_alieno(void* arg) {
    int x_dir = -1;             // Direzione iniziale: sinistra (-1)
    int cycles_to_wait = 8;     // Numero di cicli prima di sparare
    int cycles_permormed = 0;   // Contatore dei cicli eseguiti
    pthread_t tid_bullet;       // Thread ID del proiettile
    int bullet_active = 0;      // Flag per evitare più proiettili simultanei

    // Messaggio iniziale dell'alieno
    struct Message msg = {2, 2, '='};

    // Scrive la posizione iniziale dell'alieno nel buffer
    buffer_write(msg);

    // Loop principale del thread alieno
    while(1) {
        // Calcola la nuova posizione sull'asse X
        int new_x = msg.x + x_dir;

        // Inverte la direzione se raggiunge i bordi dell'area di gioco
        if(new_x >= WGAME_MAX_X-1 || new_x <= WGAME_MIN_X)
            x_dir *= -1;

        // Aggiorna la posizione orizzontale
        msg.x += x_dir;

        // Incrementa il contatore dei cicli
        cycles_permormed++;

        // Controlla se è il momento di sparare
        if(cycles_permormed > cycles_to_wait && bullet_active == 0) {
            cycles_permormed = 0;   // Reset del contatore
            bullet_active = 1;      // Impedisce la creazione di altri proiettili

            // Alloca dinamicamente un messaggio da passare al thread del proiettile
            struct Message* bullet_msg = malloc(sizeof(struct Message));
            *bullet_msg = msg;      // Copia la posizione corrente dell'alieno

            // Crea il thread che gestisce il proiettile
            pthread_create(&tid_bullet, NULL, gestione_bullet, (void*)bullet_msg);
            pthread_detach(tid_bullet);  // Il thread si libera automaticamente alla fine

            bullet_active = 0;      // Riabilita lo sparo
        }

        // Scrive la nuova posizione dell'alieno nel buffer
        buffer_write(msg);

        // Ritardo per controllare la velocità dell'alieno
        usleep(UDELAY_ALIEN);
    }

    return NULL;
}

/*
 * Funzione che gestisce il proiettile dell’alieno
 * - muove il proiettile verticalmente verso il basso
 * - scrive ogni aggiornamento nel buffer
 */
void* gestione_bullet(void* arg) {
    // Recupera il messaggio passato dal thread alieno
    struct Message* msg_ptr = (struct Message*)arg;
    struct Message msg = *msg_ptr;  // Copia locale
    free(msg_ptr);                  // Libera la memoria allocata dinamicamente

    // Imposta il carattere del proiettile
    msg.c = 'o';

    // Movimento verticale verso il basso del proiettile
    while(msg.y <= WGAME_MAX_Y) {
        msg.y += 1;
        buffer_write(msg);
        usleep(UDELAY_BULLET);  // Ritardo per controllare la velocità del proiettile
    }

    return NULL;
}
