#include "thread_functions.h"

// Importa Buffer condiviso e primitive di sincronizzazione da main.c
extern struct Message buffer[DIM_BUFFER];
extern int i_scrivi;
extern int i_leggi;
extern pthread_mutex_t mutex_buffer;
extern sem_t sem_liberi, sem_occupati;

// Scrittura nel buffer circolare (producer)
void buffer_write(struct Message msg) {
	
	sem_wait(&sem_liberi);
	pthread_mutex_lock(&mutex_buffer);
	// --- ZONA CRITICA ---
	buffer[i_scrivi] = msg; // Copio dal buffer alla variabile puntata da msg
	i_scrivi = (i_scrivi + 1) % DIM_BUFFER;
	// --------------------
	pthread_mutex_unlock(&mutex_buffer);
   
	sem_post(&sem_occupati);
}

// Lettura dal buffer circolare (consumer)
void buffer_read(struct Message* msg) {
	
	// 1. Aspetto che ci sia un messaggio (semaforo occupati)
	sem_wait(&sem_occupati);

	// 2. Prendo la chiave (Mutex)
	pthread_mutex_lock(&mutex_buffer);

	// --- ZONA CRITICA ---
	*msg = buffer[i_leggi]; // Copio dal buffer alla variabile puntata da msg
	i_leggi = (i_leggi + 1) % DIM_BUFFER;
	// --------------------

	// 3. Rilascio la chiave (Mutex)
	pthread_mutex_unlock(&mutex_buffer);

	// 4. Avviso che si è liberato un posto (semaforo liberi)
	sem_post(&sem_liberi);
    
}

// Inizializza i meccanismi di sincronizzazione
void init_sync(){
	
	// Inizializzazione semaforo
	 pthread_mutex_init(&mutex_buffer, NULL);
	sem_init(&sem_liberi,0,DIM_BUFFER);
	sem_init(&sem_occupati,0,0);
   
}


// Dealloca i meccanismi di sincronizzazione
void destroy_sync(){
    pthread_mutex_destroy(&mutex_buffer);
    sem_destroy(&sem_liberi);
    sem_destroy(&sem_occupati);
}



