#ifndef DIFESE_H
#define DIFESE_H

#include <pthread.h>
#include "main.h"  // Include main.h per struct Message

// Funzione che gestisce il movimento della difesa (giocatore)
void* gestione_difese(void* arg);

// Funzione che gestisce il laser della difesa
void* gestione_laser(void* arg);

#endif

