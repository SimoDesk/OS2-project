#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdbool.h>
#include "types.h"

// Il metodo cadd() ha lo scopo restituire il risultato della somma di due numeri complessi in input
comp cadd(comp z, comp w);

// Il metodo cmol() ha lo scopo restituire il risultato del prodotto di due numeri complessi in input
comp cmol(comp z, comp w);

// Il metodo cmod() ha lo scopo restituire il risultato del modulo di un numero complesso preso in input
double cmod(comp z);

// Il metodo moltiplicaMatrici modifica il la matrice res con i risultati delle moltiplicazioni tra i numeri complessi delle altre due matrici 
void moltiplicaMatrici(cmatrix c1, cmatrix c2, cmatrix res);

// Il metodo calcOut() modifica il vettore di numeri complessi out per contenere il risultato del prodotto tra la matrice di numeri complessi c e il vettore di numeri complessi v presi in input
void calcOut(cmatrix c, comp* v, comp* out);

// Il metodo checkNorma() restituisce True se la norma calcolata del vettore passato come input è 1, e restituisce False altrimenti
bool checkNorma(comp* v, int dim);

#endif // OPERATIONS_H
