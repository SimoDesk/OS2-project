#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/operations.h"

// Il metodo cadd() ha lo scopo restituire il risultato della somma di due numeri complessi in input
comp cadd(comp z, comp w) {
    comp res;   // Numero di output
    res.r = z.r + w.r;  // Calcolo parte reale
    res.i = z.i + w.i;  // Calcolo parte immaginaria
    return res;
}

// Il metodo cmol() ha lo scopo restituire il risultato del prodotto di due numeri complessi in input
comp cmol(comp z, comp w) {
    comp res;   // Numero di output
    res.r = ((z.r * w.r)-(z.i * w.i));  // Calcolo parte reale
    res.i = ((z.r * w.i)+(z.i * w.r));  // Calcolo parte immaginaria
    return res;
}

// Il metodo cmod() ha lo scopo restituire il risultato del modulo di un numero complesso preso in input
double cmod(comp z) {  
    return sqrt((z.r * z.r) + (z.i * z.i)); // Calcolo e restituzione del modulo
}

// Il metodo moltiplicaMatrici modifica il la matrice res con i risultati delle moltiplicazioni tra i numeri complessi delle altre due matrici 
void moltiplicaMatrici(cmatrix c1, cmatrix c2, cmatrix res) {
    int dim = c1.dim;   // In dim inserisco la dimensione delle matrici (tutte della stessa dimensione)
    for(int i = 0; i < dim; i++) {  
        for(int j = 0; j < dim; j++) {
            comp sum = (comp){0, 0};   // Sum funge da parametro accomulatore per la somma di numeri complessi e verrà inserita nelle coordinate della matrice res
            for(int k = 0; k < dim; k++) {
                sum = cadd(sum, cmol(c1.m[i][k], c2.m[k][j]));  // Calcolo un elemento della somma che rappresenterà un numero complesso della matrice res
            }
            res.m[i][j] = sum;  // Assegno la somma alla corrispondente coordinata nella matrice res
        }
    }
}

// Il metodo calcOut() modifica il vettore di numeri complessi out per contenere il risultato del prodotto tra la matrice di numeri complessi c e il vettore di numeri complessi v presi in input
void calcOut(cmatrix c, comp* v, comp* out) {
    int dim = c.dim;    // In dim inserisco la dimensione delle matrici (tutte della stessa dimensione)
    for (int i = 0; i < dim; i++) {
        comp sum = (comp){0, 0};   // Sum funge da parametro accomulatore per la somma di numeri complessi e verrà inserita nelle coordinate della matrice res
        for (int k = 0; k < dim; k++) {
            sum = cadd(sum, cmol(c.m[i][k], v[k])); // Calcolo un elemento della somma che rappresenterà un numero complesso del vettore out
        }
        out[i] = sum;   // Assegno la somma alla corrispondente coordinata nel vettore out
    }
}

// Il metodo checkNorma() restituisce True se la norma calcolata del vettore passato come input è 1, e restituisce False altrimenti
bool checkNorma(comp* v, int dim) {
    double sum = 0;
    for(int i = 0; i < dim; i++) {      //
        sum += (cmod(v[i])*cmod(v[i])); // Sommo il quadrato del modulo di ogni elemento del vettore in input
    }                                   //

    if((round(sum * 1e9) / 1e9) == 1.000000000) {   // Verifico se la norma è 1 con una precisione massima di 10^(-9)
        return true;
    } else {
        fprintf(stderr, "ERRORE, init non è un vettore di norma 1\n");  // Se la norma non è 1, stampo un errore
        exit(1);                                                        // ed esco dal programma
    }
}