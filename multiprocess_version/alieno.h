#include <unistd.h>
#include "main.h"

// Funzione principale che gestisce l'alieno
void gestione_alieno(int pipe_out);

// Funzione che gestisce il proiettile dell’alieno
void gestione_bullet(int pipe_out, struct Message pos_origin);


