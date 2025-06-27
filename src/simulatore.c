#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../include/types.h"
#include "../include/operations.h"
#include "../include/util.h"
#include "../include/stringManipolation.h"
#include "../include/dataGetter.h"
#include "../include/fileManagement.h"

typedef struct {
    cmatrix** list;         // Puntatore a array dinamico
    int dim;             // Dimensione massima
    int inizio;
    int fine;
    int occupato;
    pthread_mutex_t mutex;
    pthread_cond_t nonVuota;
    pthread_cond_t nonPiena;
} coda;

void creaCoda(coda* q, int max_dim) {
    q->list = malloc(sizeof(cmatrix *) * max_dim);
    if(q->list == NULL) error("Errore allocazione memoria della coda (main)");
    q->dim = max_dim;
    q->inizio = 0;
    q->fine = 0;
    q->occupato = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->nonVuota, NULL);
    pthread_cond_init(&q->nonPiena, NULL);
}

void enqueue(coda* q, cmatrix* m) {
    pthread_mutex_lock(&q->mutex);

    while (q->occupato == q->dim) {
        // coda piena, aspetta che si liberi
        pthread_cond_wait(&q->nonPiena, &q->mutex);
    }

    q->list[q->fine] = m;
    q->fine = (q->fine + 1) % q->dim;
    q->occupato++;

    pthread_cond_signal(&q->nonVuota);  // avvisa che c’è almeno un elemento
    pthread_mutex_unlock(&q->mutex);
}

cmatrix* dequeue(coda *q) {
    pthread_mutex_lock(&q->mutex);

    while (q->occupato == 0) {
        printf("coda vuota, aspetta che arrivi un elemento\n");
        // coda vuota, aspetta che arrivi un elemento
        pthread_cond_wait(&q->nonVuota, &q->mutex);
    }

    cmatrix *m = q->list[q->inizio];
    q->inizio = (q->inizio + 1) % q->dim;
    q->occupato--;

    pthread_cond_signal(&q->nonPiena);  // avvisa che c’è spazio
    pthread_mutex_unlock(&q->mutex);

    return m;
}

void resetCoda(coda *q, int max_dim) {
    pthread_mutex_lock(&q->mutex);

    // opzionale: libera i dati dentro la coda, se serve
    for (int i = 0; i < q->occupato; i++) {
        int idx = (q->inizio + i) % q->dim;
        if (q->list[idx] != NULL) {
            freeMatrice(q->list[idx]);
            q->list[idx] = NULL;
        }
    }

    // resetta gli indici
    q->inizio = 0;
    q->fine = 0;
    q->occupato = 0;

    pthread_mutex_unlock(&q->mutex);
}


void freeCoda(coda* q) {
    for (int i = 0; i < q->occupato; i++) {
        int idx = (q->inizio + i) % q->dim;
        if (q->list[idx] != NULL) {
            freeMatrice(q->list[idx]);
            q->list[idx] = NULL;
        }
    }

    q->inizio = 0;
    q->fine = 0;
    q->occupato = 0;
    q->dim = 0;

    free(q->list);  // libera l'array di puntatori
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->nonVuota);
    pthread_cond_destroy(&q->nonPiena);
}

void printCoda(coda q) {
    printf("Contenuto coda:\n");
    for (int i = 0; i < q.occupato; i++) {
        int idx = (q.inizio + i) % q.dim;
        if (q.list[idx] != NULL) {
            printMatrix(*q.list[idx]);
            //printf("[%i] %s\n", i, q.list[idx]->id);
        }
    }
    printf("-------\n");
}

typedef struct {
    cmatrix* m1;
    cmatrix* m2;
    cmatrix* res;
} thread_data;

void* moltiplicaMatrici_thread(void* arg) {
    thread_data* data = (thread_data*) arg;
    moltiplicaMatrici(*data->m1, *data->m2, data->res);
    free(data->res->id);
    data->res->id = malloc((strlen(data->m1->id) + strlen(data->m2->id) + 1) * sizeof(char));  // +1 per '\0'
    if (data->res->id == NULL) error("Allocazione id fallita");
    strcpy(data->res->id, data->m1->id);
    strcat(data->res->id, data->m2->id);

    pthread_exit(NULL);
}


int main() {
    char fileName[100];

    printf("Inserire il nome del file init: ");
    scanf("%99s", fileName);  // massimo 99 caratteri per evitare buffer overflow

    char* initPath = malloc(strlen("input/") + strlen(fileName) + 1);
    if(initPath == NULL) error("Errore allocazione memoria del path del file init (main)");
    snprintf(initPath, strlen("input/") + strlen(fileName) + 1, "input/%s", fileName);

    char* initContent = getContent(initPath);   // All'interno della stringa initContent memorizzo il contenuto del File "init.txt" 

    free(initPath);

    int nQubits = getNqubit(initContent);   // Utilizzo il metodo getNqubit per memorizzare il valore intero specificato in "init.txt" dopo "#qubits"
    int dim = 1 << nQubits; // Con l'operatore shift mi calcolo 2^nQubit e lo memorizzo in dim

    comp init[dim]; // Inizializzo il vettore di numeri complessi che dovrà momorizzare il vettore in "init.txt" dopo "#init"
    getInit(initContent, init, dim);    // Con il metodo getInit, valorizzo il vettore init con i dati nel file, il quale contenuto è in getInit

    //checkNorma(init, dim);  // Controllo che la norma del vettore fornito in input in "init.txt" dopo "#init" sia 1

    free(initContent);  // Libero lo spazio di memoria dedicato al contenuto file "init.txt", che ora non serve più

    printf("Inserire il nome del file circ: ");
    scanf("%99s", fileName);  // massimo 99 caratteri per evitare buffer overflow

    char* circPath = malloc(strlen("input/") + strlen(fileName) + 1);
    if(circPath == NULL) error("Errore allocazione memoria del path del file circ (main)");
    snprintf(circPath, strlen("input/") + strlen(fileName) + 1, "input/%s", fileName);

    char* circContent = getContent(circPath);   // All'interno della stringa initContent memorizzo il contenuto del File "circ.txt" 

    free(circPath);

    char* circStr = NULL;
                                            // questa verrà espansa dinamicamente a seconda di quante porte sono state selezionate per fare parte del circuito  
                                            // nel file "circ.txt" dopo "#circ"
    getCirc(circContent, &circStr); // Con il metodo getCirc, inserisco dinamicamente nella stringa circ le porte che fanno parte del ciruito definite nel file "circ.txt" dopo "#circ"
    
    int nPort = 0;  // In nPort, memorizzo la quantitò di porte che fanno parte del circuito

    char** circ = NULL;
    char* id = strtok(circStr, " ");
    while (id != NULL)
    {
        nPort++;

        char** temp = realloc(circ, nPort * sizeof(char*));   // A partire dalla grandezza dello spazio di memoria del parametro "circ", creo in "temp" una dimensinoe sufficente
                                                                            // a memorizzare tutti gli id senza sprecare memoria
            if (!temp) {    
                free(circ);                             // Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione
                fprintf(stderr, "Errore realloc\n");    //  dopo aver liberato lo spazio di memoria occupato
                exit(1);                                //
            }
            circ = temp;    // Riassegno a circ lo spazio di memoria ora allargato della dimensione perfetta per ospitare la sequenza

            circ[nPort-1] = malloc((strlen(id)+1) * sizeof(char));
            if(circ[nPort-1] == NULL) error("Errore allocazione memoria dell'id di una matrice in circ (main)");
            strcpy(circ[nPort-1], id);

        id = strtok(NULL, " ");   
    }
    free(circStr);

    cmatrix** port = malloc(nPort * sizeof(cmatrix*));
    if (port == NULL) {
        perror("Errore allocazione memoria per puntatori a matrici di porte (main)");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < nPort; i++) {
        port[i] = malloc(sizeof(cmatrix)); // Alloca la struttura cmatrix
        if(port[i] == NULL) {
            perror("Errore allocazione memoria della struttura cmatrix per porta (main)");
            // Libera la memoria già allocata prima di uscire
            for (int j = 0; j < i; j++) {
                freeMatrice(port[j]);
                free(port[j]);
            }
            free(port);
            exit(EXIT_FAILURE);
        }
        creaMatrice(port[i], dim); // Inizializza i dati interni della matrice
        getMatrix(port[i], circContent, circ[i], dim);
    }
    free(circContent);  // Libero lo spazio di memoria dedicato al contenuto file "circ.txt", che ora non serve più

    int thread_necessari = 0;
    printf("Inserire il numero massimo di thread che si desidera utilizzare: ");
    scanf("%i", &thread_necessari);  // massimo 99 caratteri per evitare buffer overflow

    cmatrix cout;   // Creo una matrice cout, che fungerà da accumulatrice durante la serie di moltiplicazioni tra matrici
    creaMatrice(&cout, dim);    // Inizializzo una struttura matrice appena citata di dimensione dim

    cmatrix temp;   // Creo una matrice temp, che nella serie di moltiplicazioni tra matrici avrà il ruolo di contenere una copia del contenuto della matrice risultante
                    // della moltiplicazione precedente, così da non creare sovrascrizioni durante i calcoli
    creaMatrice(&temp, dim);    // Inizializzo una struttura matrice appena citata di dimensione dim

    coda top;
    coda bottom;
    creaCoda(&top, nPort);
    creaCoda(&bottom, nPort);
    for(int i = nPort-1; i >= 0; i--) {
        enqueue(&top, port[i]);
    }       

    if(thread_necessari > nPort / 2) thread_necessari = nPort / 2;
    pthread_t threads[thread_necessari];

    int prodotti_attesi = 0;
    int prodotti_effettuati = 0;

    while(top.occupato > 1) {
        prodotti_attesi = top.occupato / 2;
        while(prodotti_effettuati < prodotti_attesi) {
            if(thread_necessari > prodotti_attesi-prodotti_effettuati) thread_necessari = prodotti_attesi-prodotti_effettuati;
            thread_data* data = malloc(thread_necessari * sizeof(thread_data));
            
            for(int i = 0; i < thread_necessari; i++) {
                cmatrix* m1 = dequeue(&top);
                cmatrix* m2 = dequeue(&top);
                cmatrix* res = malloc(sizeof(cmatrix));
                creaMatrice(res, dim);
                
                data[i].m1 = m1;
                data[i].m2 = m2;
                data[i].res = res;
                pthread_create(&threads[i], NULL, moltiplicaMatrici_thread, &data[i]);
            }
            
            for(int i = 0; i < thread_necessari; i++) {
                pthread_join(threads[i], NULL);
                enqueue(&bottom, data[i].res);
                prodotti_effettuati++;
            }
            free(data);
        }
        
        if(top.occupato == 1) {
            enqueue(&bottom, dequeue(&top));
        }

        coda tmp = top;
        top = bottom;
        bottom = tmp;

        prodotti_effettuati = 0;
    }
    

    // Risultato finale
    cmatrix *out = dequeue(&top);

    comp vfin[dim]; // Creo il vettore che rappresenterà la stato finale del circuito di dimenione dim
    calcOut(*out, init, vfin);  // Calcolo il contenuto del vettore finale
    
    printf("\nStato finale del circuito:\n");                       // Infine, stampo in stdout il vettore corrispondente allo stato finale del circuito
    printVector(vfin, dim);                                         //

    for(int i = 0; i < nPort; i++) {
        free(circ[i]);
    }
    free(circ);     // Libero lo spazio di memoria dedicato alla sequenza di porte, che ora non serve più

/*   
    printf("Vinit di dimensione %i (%i qubits):\n", dim, nQubits);  // 
    printVector(init, dim);                                         //
    
    for(int p = 0; p < nPort; p++) {                                // Stampo in stdout, a scopo di verifica di correzione dei dati, il vettore di input, la dimensione corrispondente
        printMatrix(port[p]);                                       // ai qBit inseriti, e tutte le matrici lette. Il tutto usando metodi ad hoc per la stampa di vettori e matrici
    }                                                               //
    
    copiaMatrice(&temp, port[nPort-1]); // Copio in temp il contenuto dell'ultima matrice del circuito

    for(int i = nPort-1; i > 0; i--) {
            moltiplicaMatrici(temp, port[i-1], &cout);    // Per ogni porta nel circuito, moltiplico quella attuale per la precedente rispetto alla loro posizion in circ e 
                                                        // memorizzo il risultato in count. 
            copiaMatrice(&temp, cout);                   // Compio il contenuto di cout in temp per evitare errori e strane sovrapposizioni di matrici nei calcoli
    }    

    comp vfin[dim]; // Creo il vettore che rappresenterà la stato finale del circuito di dimenione dim
    calcOut(cout, init, vfin);  // Calcolo il contenuto del vettore finale
    
    printf("\nStato finale del circuito:\n");                       // Infine, stampo in stdout il vettore corrispondente allo stato finale del circuito
    printVector(vfin, dim);                                         //
    

    for(int i = 0; i < nPort; i++) {
        free(circ[i]);
    }
    free(circ);     // Libero lo spazio di memoria dedicato alla sequenza di porte, che ora non serve più

    freeMatrice(&cout);                  //
    freeMatrice(&temp);                  //  Libero attraverso un metodo ad hoc gli spazi di memoria dedicati alle matrici cout e temp, nonché
    for(int i = 0; i < nPort; i++) {    //  tutte le matrici del circuito, che ora arrivati a fine programma non servono più
        freeMatrice(&port[i]);           //
    }                                   //
*/    
    return 0;
}