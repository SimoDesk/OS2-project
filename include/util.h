#ifndef UTIL_H
#define UTIL_H

#include "types.h"
#include "queue.h"

/*  Il metodo error() ha lo scopo di risparimare qualche riga di codice per interrompere il programma */
/*  in caso di errori di sistema, comunicando un messaggio preso in input */
void error(char* messaggio);

/*  Il metodo printComplex() ha lo scopo di stampare in maniera leggibile il numero complesso preso in input */
void printComplex(comp n);

/*  Il metodo printVector() ha lo scopo di stampare in maniera leggibile il vettore di numeri complessi preso in input */
void printVector(comp* vett, int dim);

/*  Il metodo printMatrix() ha lo scopo di stampare in maniera leggibile la matrice di numeri complessi preso in input */
void printMatrix(cmatrix matrix);

/*  Il metodo printCoda() ha lo scopo di stampare in maniera leggibile il contenuto della coda di matrici */
void printCoda(coda q);

/* Il metodo strDuplicate crea una copia della stringa src e la restituisce */
char* strDuplicate(const char *src);

#endif