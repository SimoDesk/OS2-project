#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "../include/types.h"
#include "../include/operations.h"
#include "../include/queue.h"
#include "../include/thread.h"
#include "../include/util.h"

// Funzione eseguita dai thread per moltiplicare due matrici
// La funzione prende un argomento di tipo thread_data che contiene i puntatori alle matrici
// da moltiplicare e alla matrice di risultato.
void* moltiplicaMatrici_thread(void* arg) {
    thread_data* data = (thread_data*) arg; // Cast dell'argomento a thread_data  

    moltiplicaMatrici(*data->m1, *data->m2, data->res); // Chiamata alla funzione che moltiplica le matrici

    free(data->res->id);    // Libero l'id della matrice di risultato, che è stato allocato dinamicamente
    data->res->id = malloc((strlen(data->m1->id) + strlen(data->m2->id) + 1) * sizeof(char));  // Alloco spazio per l'id della matrice di risultato
    if (data->res->id == NULL) error("Allocazione id fallita"); // Allocazione dinamica della stringa id per la matrice di risultato
    strcpy(data->res->id, data->m1->id);    // Copio l'id della prima matrice
    strcat(data->res->id, data->m2->id);    // Concateno l'id della seconda matrice

    pthread_exit(NULL); // Termina il thread
}