#include <stdio.h>
#include <stdlib.h>

#include "../include/types.h"
#include "../include/util.h"

// Il metodo creaMatrice() inizializza lo spazio necessario per i dati della matrice, ne memorizza la dimensione e assegna un carattere "nullo" all'id
void creaMatrice(cmatrix* m, int dimensione) {
    m->id = malloc(1 * sizeof(char));
    m->id = "\0";    // Carattere generico per l'id che verrà comunque sovrascritto in seguito
    m->dim = dimensione;    // Memorizzio per la matrice la sua dimensione per rendere più agevoli calcoli successivi
    m->m = malloc(dimensione * sizeof(comp*));  // Alloco lo spazio necessario per contenere una quantità di puntatori di complessi corrispondente alla dimensione in input
    if(m->m == NULL) error("Errore allocazione memoria delle righe in creaMatrice");    // Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione
    for(int i = 0; i < dimensione; i++) {
        m->m[i] = malloc(dimensione * sizeof(comp));    // Per ogni puntatore di numeri complessi, alloco lo spazio necessario per contenere una quantità di numeri complessi
                                                        // corrispondente alla dimensione in input, ottenendo così una struttura a "matrice" funzionante
        if(m->m[i] == NULL) error("Errore allocazione memoria delle colonne in creaMatrice");   // Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione
    }
}

// Il metodo copiaComplesso() ha il compito di compiare i valori reali e immaginari del parametro "part" in quelli del parametro "dest" entrambi in input
void copiaComplesso(comp* dest, comp part) {
    dest->r = part.r;   // copio il parametro reale
    dest->i = part.i;   // copio il parametro immaginario
}

// Il metodo copiaMatrice() ha il compito di compiare il contenuto della matrice di numeri complessi "part" all'interno di "dest", entrambi presi in input
void copiaMatrice(cmatrix* dest, cmatrix part) {
    int dim = part.dim; // In dim inserisco la dimensione delle matrici (tutte della stessa dimensione)
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            copiaComplesso(&dest->m[i][j], part.m[i][j]);   // Per ogni elemento della matrice "part", ne creo una copia nella stessa posizione nella matrice "dest"
        }
    }
}

// Il metodo freeMatrice() libera gli spazi di memoria dedicati ai numeri complessi e ai puntatori di questi ultimi nella matrice di numeri complessi passata in input
void freeMatrice(cmatrix m) {
    for(int i = 0; i < m.dim; i++) {
        free(m.m[i]);   // Per ogni riga, libero lo spazio di memoria dedicato ad ospitare i numeri complessi
    }
    free(m.m);  // Libero lo spazio di memoria dedicato ad ospitare i puntatori dei numeri complessi 
}

// Il metodo parseComplesso() si occpua di trasformare il contenuto della stringa "num" in corrispondenti valori accettabili da inserire nel parametro "n", entrambi passati in input
void parseComplesso(comp* n, char* num) {
    char operazione_i = ' ';    // Questo valore conterrà eventualmente la positività (+) o la negatività (-) del parametro immaginario
    char operazione_r = ' ';    // Questo valore conterrà eventualmente la positività (+) o la negatività (-) del parametro reale
    char immaginary;    // Questo valore è devoto solo a contenere il carattere "i"
    char temp;  // Questo parametro, se dovesse ricevere un valore, significherebbe che la stringa "num" in input contiene caratteri non consentiti alla fine della stringa

    int res = sscanf(num, "%lf%c%c%lf%c", &n->r, &operazione_i, &immaginary, &n->i, &temp); // Nel parametro "res" memorizzo l'output della funzione sscanf(), che corrisponde al numero di valori assegnati.
                                                                                            // Con la scansione "%lf%c%c%lf%c" cerco sequenzialmente da sinistra verso destra:
                                                                                            // un numero reale, due caratteri, un altro numero reale ed infine un ultimo carattere
                                                                                            // se questi dovessero essere trovati, verrebbero assegnati rispettivamente
                                                                                            // alla parte reale di "n", ad "operazione_i", ad "immaginary" e infine alla parte immaginaria di "n". 
                                                                                            // Sto cercando quindi numeri come: "0.5-i1.5"
    if(operazione_i == '-') n->i = -n->i;   // Nel caso in cui il segno della parte immaginaria sia negativo, lo inverto in "n"
    
    // Se il primo sscanf() ha assegnato solo i primi tre valori e i parametri "immaginary" e "operazione_i" sono corretti, significa che siamo in una situazione nella quale la parte immaginaria è 1
    // ad esempio: "0.5-i"
    if(res == 3 && immaginary == 'i' && (operazione_i == '+' || operazione_i == '-')) {
        n->i = 1;   // Valorizzo la parte immaginaria con 1
        if(operazione_i == '-') n->i = -n->i;   // Nel caso in cui il segno della parte immaginaria sia negativo, lo inverto in "n"

    // Se il primo sscanf() ha assegnato solo il primo valore, significa che siamo in una situazione nella quale la parte immaginaria è 0
    // ad esempio: "0.5"    
    } else if(res == 1) {
        n->i = 0;   // Valorizzo la parte immaginaria con 0

    // Se il primo sscanf() non ha assegnato alcun valore, allora entriamo in un'altra serie di casistiche
    } else if(res == 0) {

        // In questo caso, cerco un valore di tipo "%c%c%lf%c%lf", cioè cerco sequenzialmente da sinistra verso destra: due caratteri, un numero reale, un altro carattere
        // e un ultimo numero reale. Qualora questi dovessero essere trovati, verrebbero assegnati rispettivamente a "operazione_i", "immaginary", la parte immaginaria di "n",
        // "operazione_r" e la parte reale di "n".
        // Sto quindi cercando numeri come: "-i0.5+0.5". Questa condizione è verificata se tutti i parametri carattere sono corretti e se l'ultimo "temp" non è valorizzato.
        if((sscanf(num, "%c%c%lf%c%lf%c", &operazione_i, &immaginary, &n->i, &operazione_r, &n->r, &temp) == 5) && immaginary == 'i' && (operazione_i == '+' || operazione_i == '-') && (operazione_r == '+' || operazione_r == '-')) {
            
            if(operazione_i == '-') n->i = -n->i;   // Nel caso in cui il segno della parte immaginaria sia negativo, lo inverto in "n"
            if(operazione_r == '-') n->r = -n->r;   // Nel caso in cui il segno della parte reale sia negativo, lo inverto in "n"

            return; // Esco dalla funzione in quanto il numero è stato converito
        }

        // In questo caso, cerco un valore di tipo "%c%c%lf", cioè cerco sequenzialmente da sinistra verso destra: due caratteri, un numero reale.
        // Qualora questi dovessero essere trovati, verrebbero assegnati rispettivamente a "operazione_i", "immaginary", la parte immaginaria di "n".
        // Sto quindi cercando numeri come: "-i0.5". Questa condizione è verificata se tutti i parametri carattere sono corretti e se l'ultimo "temp" non è valorizzato.
        if((sscanf(num, "%c%c%lf%c", &operazione_i, &immaginary, &n->i, &temp) == 3) && (operazione_i == '+' || operazione_i == '-') && immaginary == 'i') {

            n->r = 0;   // Questo caso prevede che la parte reale sia 0
            if(operazione_i == '-') n->i = -n->i;   // Nel caso in cui il segno della parte immaginaria sia negativo, lo inverto in "n"
            if(operazione_r == '-') n->r = -n->r;    // Nel caso in cui il segno della parte reale sia negativo, lo inverto in "n"

            return; // Esco dalla funzione in quanto il numero è stato converito
        }

        // In questo caso, cerco un valore di tipo "%c%c%lf", cioè cerco sequenzialmente da sinistra verso destra: due caratteri, un numero reale.
        // Qualora questi dovessero essere trovati, verrebbero assegnati rispettivamente a "immaginary", "operazione_r", la parte reale di "n".
        // Sto quindi cercando numeri come: "i+0.5". Questa condizione è verificata se tutti i parametri carattere sono corretti e se l'ultimo "temp" non è valorizzato.
        if((sscanf(num, "%c%c%lf%c", &immaginary, &operazione_r, &n->r, &temp) == 3) && (operazione_r == '+' || operazione_r == '-') && immaginary == 'i') {

            n->i = 1;   // Questo caso prevede che la parte immaginaria sia 1
            if(operazione_i == '-') n->i = -n->i;   // Nel caso in cui il segno della parte immaginaria sia negativo, lo inverto in "n"
            if(operazione_r == '-') n->r = -n->r;   // Nel caso in cui il segno della parte reale sia negativo, lo inverto in "n"

            return; // Esco dalla funzione in quanto il numero è stato converito
        }

        // In questo caso, cerco un valore di tipo "%c%lf%c%lf", cioè cerco sequenzialmente da sinistra verso destra: un carattere, un numero reale, un altro carattere ed un altro numero reale.
        // Qualora questi dovessero essere trovati, verrebbero assegnati rispettivamente a "immaginary", la parte immaginaria di "n", "operazione_r" e alla parte reale di "n".
        // Sto quindi cercando numeri come: "i0.5+0.5". Questa condizione è verificata se tutti i parametri carattere sono corretti e se l'ultimo "temp" non è valorizzato.
        if((sscanf(num, "%c%lf%c%lf%c", &immaginary, &n->i, &operazione_r, &n->r, &temp) == 4) && (operazione_r == '+' || operazione_r == '-') && immaginary == 'i') {

            if(operazione_i == '-') n->i = -n->i;   // Nel caso in cui il segno della parte immaginaria sia negativo, lo inverto in "n"
            if(operazione_r == '-') n->r = -n->r;   // Nel caso in cui il segno della parte reale sia negativo, lo inverto in "n"

            return; // Esco dalla funzione in quanto il numero è stato converito
        }

        // In questo caso, cerco un valore di tipo "%c%lf%c", cioè cerco sequenzialmente da sinistra verso destra: un carattere, un numero reale e un altro carattere.
        // Qualora questi dovessero essere trovati, verrebbero assegnati rispettivamente a "immaginary" e alla parte immaginaria di "n".
        // Sto quindi cercando numeri come: "i0.5". Questa condizione è verificata se tutti i parametri carattere sono corretti e se l'ultimo "temp" non è valorizzato.
        if((sscanf(num, "%c%lf%c", &immaginary, &n->i, &temp) == 2) && immaginary == 'i') {

            n->r = 0;   // Questo caso prevede che la parte immaginaria sia 0
            if(operazione_i == '-') n->i = -n->i;   // Nel caso in cui il segno della parte immaginaria sia negativo, lo inverto in "n"
            if(operazione_r == '-') n->r = -n->r;   // Nel caso in cui il segno della parte reale sia negativo, lo inverto in "n"

            return; // Esco dalla funzione in quanto il numero è stato converito
        }

        // In questo caso, cerco un valore di tipo "%c%c", cioè cerco sequenzialmente da sinistra verso destra: due caratteri.
        // Qualora questi dovessero essere trovati, verrebbero assegnati rispettivamente a "operazione_i" e a "immaginary".
        // Sto quindi cercando numeri come: "-i". Questa condizione è verificata se tutti i parametri carattere sono corretti e se l'ultimo "temp" non è valorizzato.
        if((sscanf(num, "%c%c%c", &operazione_i, &immaginary, &temp) == 2) && immaginary == 'i' && (operazione_i == '+' || operazione_i == '-')) {

            n->r = 0;   // Questo caso prevede che la parte reale sia 0
            n->i = 1;   // Questo caso prevede che la parte immaginaria sia 1
            if(operazione_i == '-') n->i = -n->i;   // Nel caso in cui il segno della parte immaginaria sia negativo, lo inverto in "n"

            return; // Esco dalla funzione in quanto il numero è stato converito
        }

        // In questo caso, cerco un valore di tipo "%c", cioè cerco sequenzialmente da sinistra verso destra: un caratteri.
        // Qualora questo dovessero essere trovato, verrebbe assegnato a "immaginary".
        // Sto quindi cercando numeri come: "i". Questa condizione è verificata se tutti i parametri carattere sono corretti e se l'ultimo "temp" non è valorizzato.
        if((sscanf(num, "%c%c", &immaginary, &temp) == 1) && immaginary == 'i') {

            n->r = 0;   // Questo caso prevede che la parte reale sia 0
            n->i = 1;   // Questo caso prevede che la parte immaginaria sia 1

            return; // Esco dalla funzione in quanto il numero è stato converito
        }

        fprintf(stderr, "ERRORE FORMATTAZIONE NUMERO COMPLESSO %s (cod. 2)\n", num);    // Se nessuno dei casi in cui res = 0 è soddisfatto, allora vuol dire che il numero fornito
        exit(1);                                                                        // non è del formato corretto, interrompendo perciò il programma con un errore
    } else if(res =! 4 || immaginary != 'i' || (operazione_i != '+' && operazione_i != '-')){
        fprintf(stderr, "ERRORE FORMATTAZIONE NUMERO COMPLESSO %s (cod. 1)\n", num);    // Se nessuno dei casi è soddisfatto, allora vuol dire che il numero fornito
        exit(1);                                                                        // non è del formato corretto, interrompendo perciò il programma con un errore
    }
}