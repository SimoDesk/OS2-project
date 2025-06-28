#include <stdio.h>

#include <stdlib.h>

#include "../include/queue.h"
#include "../include/types.h"
#include "../include/util.h"

// Il metodo creaCoda() inizializza lo spazio necessario per i dati della coda
void creaCoda(coda* q, int max_dim) {
    q->list = malloc(sizeof(cmatrix *) * max_dim);  // Alloco lo spazio per l'array di puntatori a cmatrix
    if(q->list == NULL) error("Errore allocazione memoria della coda (main)"); // Se l'allocazione fallisce, esce con errore

    q->dim = max_dim;   // Imposto la dimensione massima delle singole matrici
    q->inizio = 0;  // Imposto l'indice di inizio della coda a 0
    q->fine = 0;    // Imposto l'indice di fine della coda a 0
    q->occupato = 0;    // Imposto il numero di elementi attualmente nella coda a 0
}

// Il metodo enqueue() inserisce una matrice di numeri complessi nella coda
void enqueue(coda* q, cmatrix* m) {
    if (q->occupato == q->dim) {    // Controlla se la coda è piena
        printf("Coda piena!\n");    // Se la coda è piena, stampa un messaggio di errore
        return;
    }

    q->list[q->fine] = m;   // Inserisce la matrice nella posizione di fine della coda
    q->fine = (q->fine + 1) % q->dim;   // Aggiorna l'indice di fine della coda in modo circolare
    q->occupato++;  // Incrementa il numero di elementi nella coda
}

// Il metodo dequeue() rimuove e restituisce la matrice di numeri complessi dalla coda
cmatrix* dequeue(coda *q) {
    if (q->occupato == 0) { // Controlla se la coda è vuota
        printf("Coda vuota!\n");    // Se la coda è vuota, stampa un messaggio di errore
        return NULL;
    }

    cmatrix *m = q->list[q->inizio];    // Prende la matrice dalla posizione di inizio della coda
    q->inizio = (q->inizio + 1) % q->dim;   // Aggiorna l'indice di inizio della coda in modo circolare
    q->occupato--;  // Decrementa il numero di elementi nella coda

    return m;   // Restituisce la matrice rimossa dalla coda
}

// Il metodo resetCoda() resetta la coda, liberando gli spazi di memoria dedicati alle matrici di numeri complessi
void resetCoda(coda *q, int max_dim) {
    for (int i = 0; i < q->occupato; i++) { // Itera su tutti gli elementi occupati nella coda
        int idx = (q->inizio + i) % q->dim; // Calcola l'indice circolare dell'elemento corrente
        if (q->list[idx] != NULL) { // Se l'elemento corrente non è NULL libero la matrice
            freeMatrice(q->list[idx]);
            q->list[idx] = NULL;
        }
    }

    q->inizio = 0;  // Resetta l'indice di inizio della coda a 0
    q->fine = 0;    // Resetta l'indice di fine della coda a 0
    q->occupato = 0;    // Resetta il numero di elementi nella coda a 0
}

// Il metodo freeCoda() libera gli spazi di memoria dedicati alla coda e alle matrici di numeri complessi in essa contenute
void freeCoda(coda* q) {
    for (int i = 0; i < q->occupato; i++) { // Itera su tutti gli elementi occupati nella coda
        int idx = (q->inizio + i) % q->dim; // Calcola l'indice circolare dell'elemento corrente
        if (q->list[idx] != NULL) { // Se l'elemento corrente non è NULL libero la matrice
            freeMatrice(q->list[idx]);
            q->list[idx] = NULL;
        }
    }

    q->inizio = 0;  // Resetta l'indice di inizio della coda a 0
    q->fine = 0;    // Resetta l'indice di fine della coda a 0
    q->occupato = 0;    // Resetta il numero di elementi nella coda a 0
    q->dim = 0; // Resetta la dimensione massima delle singole matrici a 0

    free(q->list);  // Libera lo spazio di memoria dedicato all'array di puntatori a cmatrix
}