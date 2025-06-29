#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/util.h"
#include "../include/types.h"
#include "../include/queue.h"

// Il metodo error() ha lo scopo di risparimare qualche riga di codice per interrompere il programma
// in caso di errori di sistema, comunicando un messaggio preso in input
void error(char* messaggio) {   
    perror(messaggio);  // Stampa il messaggio di errore passato in input
    exit(1);    // Interrompe l'esecuzione del programma
}

// Il metodo printComplex() ha lo scopo di stampare in maniera leggibile il numero complesso preso in input
void printComplex(comp n) {
    if(n.i >= 0) {  // Se la parte immaginaria del numero complesso è positiva o zero, lo stampo in formato "reale + i immaginario"
        printf("%.5f+i%.5f", n.r, n.i); 
    } else {    // Se la parte immaginaria del numero complesso è negativa, lo stampo in formato "reale - i immaginario"
        double pos_i = fabs(n.i);
        printf("%.5f-i%.5f", n.r, pos_i); 
    }
    
}

// Il metodo printVector() ha lo scopo di stampare in maniera leggibile il vettore di numeri complessi preso in input
void printVector(comp* vett, int dim) {
    printf("[(");
    for(int i = 0; i < dim; i++) {  // Per ogni numero complesso nel vettore, stampo l'indice e il numero stesso
        printComplex(vett[i]);
        if(i < dim-1) printf(", ");
    }
    printf(")]\n");
}

// Il metodo printMatrix() ha lo scopo di stampare in maniera leggibile la matrice di numeri complessi preso in input
void printMatrix(cmatrix matrix) {
    int dim = matrix.dim;  // In "dim" memorizzo la dimensione della matrice, che in quanto quadrata vede sufficente una sola dimensione
    printf("Matrice %s: \n", matrix.id); 
    for(int i = 0; i < dim; i++) { // Per ogni riga della matrice, stampo l'indice di riga e i numeri complessi in essa contenuti
        for(int j = 0; j < dim; j++) {
            printf("[%i][%i] ", i, j);
            printComplex(matrix.m[i][j]);
        }
    }
    printf("\n");
}

// Il metodo printCoda() ha lo scopo di stampare in maniera leggibile il contenuto della coda di matrici
void printCoda(coda q) {
    printf("Contenuto coda:\n");
    for (int i = 0; i < q.occupato; i++) {  // Per ogni matrice nella coda, stampo l'indice e la matrice stessa
        int idx = (q.inizio + i) % q.dim;
        if (q.list[idx] != NULL) {
            printMatrix(*q.list[idx]);
        }
    }
    printf("-------\n");
}