#ifndef DATA_GETTER_H
#define DATA_GETTER_H

#include "../include/types.h"

// Il metodo getNqubit cerca un valore intero specificato dopo "#qubits" nel contenuto del file passato in input, e se lo trova lo restituisce in output
int getNqubit(char* content);

// Il metodo getInit cerca il vettore di numeri complessi specificato dopo "#init" nel contenuto del file passato in input, e se lo trova lo restituisce in output
void* getInit(char* content, comp* init, int dim);

// Il metodo getMatrix cerca la matrice di numeri complessi specificata dopo "#define" e l'id corrispondente all'omonimo parametro in input.
// La ricreca avviene nel contenuto del file passato anch'esso passato in input, e se la matrice viene trovata viene assegnata al parametro in input "m"
void getMatrix(cmatrix* m, char* content, char id, int dimensione);

// Il metodo getCirc cerca la sequenza di id delle matrici specificata dopo "#circ" nel contenuto del file passato in input, e se la trova viene assegnata al parametro in input "circ"
void* getCirc(char* content, char* circ);

#endif // DATA_GETTER_H