#ifndef THREAD_H
#define THREAD_H

#include "types.h"
#include "queue.h"

typedef struct {    /*  Struttura per passare i dati ai thread */
    cmatrix* m1;    /*  Puntatore alla prima matrice */
    cmatrix* m2;    /*  Puntatore alla seconda matrice */
    cmatrix* res;   /*  Puntatore alla matrice di risultato */
} thread_data;

/*  Funzione eseguita dai thread per moltiplicare due matrici */
void* moltiplicaMatrici_thread(void* arg);

#endif