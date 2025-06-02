#include <stdio.h>
#include <stdlib.h>

#include "../include/util.h"
#include "../include/types.h"

// Il metodo error() ha lo scopo di risparimare qualche riga di codice per interrompere il programma
// in caso di errori di sistema, comunicando un messaggio preso in input
void error(char* messaggio) {   
    perror(messaggio);          
    exit(1);
}

// Il metodo printComplex() ha lo scopo di stampare in maniera leggibile il numero complesso preso in input
void printComplex(comp n) {         
    printf("%lf i%lf\n", n.r, n.i); 
}

// Il metodo printVector() ha lo scopo di stampare in maniera leggibile il vettore di numeri complessi preso in input
void printVector(comp* vett, int dim) {
    for(int i = 0; i < dim; i++) {
        printf("[%i] ", i);
        printComplex(vett[i]);
    }
    printf("\n");
}

// Il metodo printMatrix() ha lo scopo di stampare in maniera leggibile la matrice di numeri complessi preso in input
void printMatrix(cmatrix matrix) {
    int dim = matrix.dim;
    printf("Matrice %c: \n", matrix.id);
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            printf("[%i][%i] ", i, j);
            printComplex(matrix.m[i][j]);
        }
    }
    printf("\n");
}