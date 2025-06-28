#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"

typedef struct {    // Struttura che rappresenta una coda di matrici di numeri complessi
    cmatrix** list;      // La lista di matrici di numeri complessi
    int dim;             // Dimensione delle singole matrici
    int inizio;          // Indice di inizio della coda
    int fine;            // Indice di fine della coda
    int occupato;       // Numero di elementi attualmente nella coda
} coda;

// Il metodo creaCoda() inizializza lo spazio necessario per i dati della coda
void creaCoda(coda* q, int max_dim);

// Il metodo enqueue() inserisce una matrice di numeri complessi nella coda
void enqueue(coda* q, cmatrix* m);

// Il metodo dequeue() rimuove e restituisce la matrice di numeri complessi dalla coda
cmatrix* dequeue(coda *q);

// Il metodo resetCoda() resetta la coda, liberando gli spazi di memoria dedicati alle matrici di numeri complessi
void resetCoda(coda *q, int max_dim);

// Il metodo freeCoda() libera gli spazi di memoria dedicati alla coda e alle matrici di numeri complessi in essa contenute
void freeCoda(coda* q);

#endif // QUEUE_H