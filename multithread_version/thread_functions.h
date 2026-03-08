#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

#include <pthread.h>
#include <semaphore.h>
#include "main.h"

#define DIM_BUFFER 50

// Scrittura nel buffer circolare (producer)
void buffer_write(struct Message msg);

// Lettura dal buffer circolare (consumer)
void buffer_read(struct Message* msg);

// Inizializza i meccanismi di sincronizzazione
void init_sync();

// Dealloca i meccanismi di sincronizzazione
void destroy_sync();

#endif

