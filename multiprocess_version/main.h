#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>

#define UDELAY_LASER 	30000
#define UDELAY_BULLET 	50000
#define UDELAY_ALIEN 	100000

#define WGAME_MAX_X		60
#define WGAME_MAX_Y		20
#define WGAME_MIN_X		0
#define WGAME_MIN_Y		3

// Struttura dei messaggi salvati nel buffer
struct Message {
    int x;
    int y;
    char c;
    int pid;
};

// Funzione che gestisce l'area di gioco e la logica delle collisioni
int gestione_area_gioco(WINDOW *w, int pipe_in);

#endif

