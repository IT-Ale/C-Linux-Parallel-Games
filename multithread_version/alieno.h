#ifndef ALIENO_H
#define ALIENO_H

#include <pthread.h>
#include "main.h"

// Funzione che gestisce il movimento dell’alieno
void* gestione_alieno(void* arg);

// Funzione che gestisce il proiettile dell’alieno
void* gestione_bullet(void* arg);

#endif

