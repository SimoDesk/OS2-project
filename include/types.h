#ifndef TYPES_H
#define TYPES_H

typedef struct {    // Struttura che rappresenta un numero complesso
    double r;   // Il valore r corrisopnde alla parte reale del numero complesso
    double i;   // Il valore i corrisponde alla parte immaginaria del numero complesso
} comp;

typedef struct {    // Struttura che rappresenta le matrici di numeri complessi
    comp** m;   // Il parametro m in quanto puntatore di puntatori di numeri complessi, rappresenta la matrice vera e propria
    char* id;    // Il valore id conterrà la lettera identificativa della matrice
    int dim;    // Il valore dim conterrà la dimensione della matrice, che in quanto quadrata vede sufficente una sola dimensione
} cmatrix;

// Il metodo creaMatrice() inizializza lo spazio necessario per i dati della matrice, ne memorizza la dimensione e assegna un carattere "nullo" all'id
void creaMatrice(cmatrix* m, int dimensione);

// Il metodo copiaComplesso() ha il compito di compiare i valori reali e immaginari del parametro "part" in quelli del parametro "dest" entrambi in input
void copiaComplesso(comp* dest, comp part);

// Il metodo copiaMatrice() ha il compito di compiare il contenuto della matrice di numeri complessi "part" all'interno di "dest", entrambi presi in input
void copiaMatrice(cmatrix* dest, cmatrix part);

// Il metodo freeMatrice() libera gli spazi di memoria dedicati ai numeri complessi e ai puntatori di questi ultimi nella matrice di numeri complessi passata in input
void freeMatrice(cmatrix m);

// Il metodo parseComplesso() si occpua di trasformare il contenuto della stringa "num" in corrispondenti valori accettabili da inserire nel parametro "n", entrambi passati in input
void parseComplesso(comp* n, char* num);

#endif // TYPES_H
