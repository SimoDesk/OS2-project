#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/types.h"
#include "../include/operations.h"
#include "../include/util.h"
#include "../include/stringManipolation.h"
#include "../include/dataGetter.h"
#include "../include/fileManagement.h"

int main() {
    char* initContent = getContent("input/init.txt");   // All'interno della stringa initContent memorizzo il contenuto del File "init.txt" 
    int nQubits = getNqubit(initContent);   // Utilizzo il metodo getNqubit per memorizzare il valore intero specificato in "init.txt" dopo "#qubits"
    int dim = 1 << nQubits; // Con l'operatore shift mi calcolo 2^nQubit e lo memorizzo in dim

    comp init[dim]; // Inizializzo il vettore di numeri complessi che dovrà momorizzare il vettore in "init.txt" dopo "#init"
    getInit(initContent, init, dim);    // Con il metodo getInit, valorizzo il vettore init con i dati nel file, il quale contenuto è in getInit

    checkNorma(init, dim);  // Controllo che la norma del vettore fornito in input in "init.txt" dopo "#init" sia 1

    free(initContent);  // Libero lo spazio di memoria dedicato al contenuto file "init.txt", che ora non serve più

    char* circContent = getContent("input/circ.txt");   // All'interno della stringa initContent memorizzo il contenuto del File "circ.txt" 

    char* circ = malloc(1 * sizeof(char));  // Dedico un piccolo spazio di memoria della dimesnsione di un carattere, 
                                            // questa verrà espansa dinamicamente a seconda di quante porte sono state selezionate per fare parte del circuito  
                                            // nel file "circ.txt" dopo "#circ"
    if(circ == NULL) error("Errore allocazione memoria di circ (main)");    // Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione

    getCirc(circContent, circ); // Con il metodo getCirc, inserisco dinamicamente nella stringa circ le porte che fanno parte del ciruito definite nel file "circ.txt" dopo "#circ"
    int nPort = strlen(circ);   // In nPort, memorizzo la quantitò di porte che fanno parte del circuito
    

    cmatrix port[nPort];    // Creo un vettore di matrici di numeri complessi, ognuna delle quali corrisponderà ad una porta
    for(int i = 0; i < nPort; i++) {
        creaMatrice(&port[i], dim); // per ogni porta, inizializzo una struttuta matrice di dimensione dim
        getMatrix(&port[i], circContent, circ[i], dim); // Con il metodo getMatrix, assegno ad ogni coordinata della matrice corrispondente alla porta i corretti valori 
                                                        // corrispondenti contenuti nel file "circ.txt" dopo "#define" e la lettera corrispondente contenuta in circ
    }
    free(circ);     // Libero lo spazio di memoria dedicato alla sequenza di porte, che ora non serve più
    free(circContent);  // Libero lo spazio di memoria dedicato al contenuto file "circ.txt", che ora non serve più

    cmatrix cout;   // Creo una matrice cout, che fungerà da accumulatrice durante la serie di moltiplicazioni tra matrici
    creaMatrice(&cout, dim);    // Inizializzo una struttura matrice appena citata di dimensione dim

    cmatrix temp;   // Creo una matrice temp, che nella serie di moltiplicazioni tra matrici avrà il ruolo di contenere una copia del contenuto della matrice risultante
                    // della moltiplicazione precedente, così da non creare sovrascrizioni durante i calcoli
    creaMatrice(&temp, dim);    // Inizializzo una struttura matrice appena citata di dimensione dim

    copiaMatrice(&temp, port[nPort-1]); // Copio in temp il contenuto dell'ultima matrice del circuito

    for(int i = nPort-1; i > 0; i--) {
        moltiplicaMatrici(temp, port[i-1], cout);    // Per ogni porta nel circuito, moltiplico quella attuale per la precedente rispetto alla loro posizion in circ e 
                                                     // memorizzo il risultato in count. 
        copiaMatrice(&temp, cout);                   // Compio il contenuto di cout in temp per evitare errori e strane sovrapposizioni di matrici nei calcoli
    }    

    comp vfin[dim]; // Creo il vettore che rappresenterà la stato finale del circuito di dimenione dim
    calcOut(cout, init, vfin);  // Calcolo il contenuto del vettore finale

    printf("Vinit di dimensione %i (%i qubits):\n", dim, nQubits);  // 
    printVector(init, dim);                                         //
    for(int p = 0; p < nPort; p++) {                                // Stampo in stdout, a scopo di verifica di correzione dei dati, il vettore di input, la dimensione corrispondente
        printMatrix(port[p]);                                       // ai qBit inseriti, e tutte le matrici lette. Il tutto usando metodi ad hoc per la stampa di vettori e matrici
    }                                                               //
    printf("\nStato finale del circuito:\n");                       // Infine, stampo in stdout il vettore corrispondente allo stato finale del circuito
    printVector(vfin, dim);                                         //
    

    freeMatrice(cout);                  //
    freeMatrice(temp);                  //  Libero attraverso un metodo ad hoc gli spazi di memoria dedicati alle matrici cout e temp, nonché
    for(int i = 0; i < nPort; i++) {    //  tutte le matrici del circuito, che ora arrivati a fine programma non servono più
        freeMatrice(port[i]);           //
    }                                   //

    return 0;
}