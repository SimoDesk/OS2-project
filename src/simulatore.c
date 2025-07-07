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
#include "../include/queue.h"
#include "../include/thread.h"

int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Errore, l'esecuzione deve avere questo formato: ./simulatore <initFile> <circFile> <Nqubits>\n");
        return 1;
    }

    char* initPath = malloc(strlen("input/") + strlen(argv[1]) + 1);   /* Alloco dinamicamente lo spazio di memoria per il path del file init, che sarà "input/" + nome del file + terminatore di stringa */
    if(initPath == NULL) error("Errore allocazione memoria del path del file init (main)"); /* Controllo che l'allocazione della memoria sia andata a buon fine  */
                                                                                            /* se no, stampo un messaggio di errore e termino l'esecuzione */
    
    snprintf(initPath, strlen("input/") + strlen(argv[1]) + 1, "input/%s", argv[1]);  /* Con snprintf, creo il path del file init, che sarà "input/" + nome del file + terminatore di stringa */

    char* initContent = getContent(initPath);   /* All'interno della stringa initContent memorizzo il contenuto del File "init.txt"  */
    rimuoviCarattere(initContent, "\n\t "); /* Rimuovo i caratteri di nuova linea, tabulazione e spazio dalla stringa initContent */

    int nQubits = getNqubit(initContent);   /* Utilizzo il metodo getNqubit per memorizzare il valore intero specificato in "init.txt" dopo "#qubits" */
    int dim = 1 << nQubits; /* Con l'operatore shift mi calcolo 2^nQubit e lo memorizzo in dim */

    comp init[dim]; /* Inizializzo il vettore di numeri complessi che dovrà momorizzare il vettore in "init.txt" dopo "#init" */
    getInit(initContent, init, dim);    /* Con il metodo getInit, valorizzo il vettore init con i dati nel file, il quale contenuto è in getInit */

    checkNorma(init, dim);  /* Controllo che la norma del vettore fornito in input in "init.txt" dopo "#init" sia 1 */

    free(initPath); /* Libero lo spazio di memoria dedicato al path del file init, che ora non serve più */
    free(initContent);  /* Libero lo spazio di memoria dedicato al contenuto file init, che ora non serve più */

    char* circPath = malloc(strlen("input/") + strlen(argv[2]) + 1);   /* Alloco dinamicamente lo spazio di memoria per il path del file circ, che sarà "input/" + nome del file + terminatore di stringa */
    if(circPath == NULL) error("Errore allocazione memoria del path del file circ (main)"); /* Controllo che l'allocazione della memoria sia andata a buon fine  */
                                                                                            /* se no, stampo un messaggio di errore e termino l'esecuzione */
    
    snprintf(circPath, strlen("input/") + strlen(argv[2]) + 1, "input/%s", argv[2]);  /* Con snprintf, creo il path del file circ, che sarà "input/" + nome del file + terminatore di stringa */

    char* circContent = getContent(circPath);   /* All'interno della stringa initContent memorizzo il contenuto del File "circ.txt"  */
    rimuoviCarattere(circContent, "\n\t");  /* Rimuovo i caratteri di nuova linea, tabulazione e spazio dalla stringa circContent */

    char* circStr = NULL;   /* Inizializzo la stringa circStr, che conterrà gli id delle porte del circuito */
                                            /* questa verrà espansa dinamicamente a seconda di quante porte sono state selezionate per fare parte del circuito   */
                                            /* nel file "circ.txt" dopo "#circ" */
    getCirc(circContent, &circStr); /* Con il metodo getCirc, inserisco dinamicamente nella stringa circ le porte che fanno parte del ciruito definite nel file "circ.txt" dopo "#circ" */
    
    int nPort = 0;  /* In nPort, memorizzo la quantitò di porte che fanno parte del circuito */

    char** circ = NULL; /* Inizializzo un array di puntatori a stringhe che conterrà gli id delle porte del circuito, che sono memorizzati in circStr */
    char* id = strtok(circStr, " ");    /* Con strtok, divido la stringa circStr in id delle porte, separati da uno spazio */
    while (id != NULL)  /* Finché ci sono id da leggere, continuo a leggere */
    {
        nPort++;    /* Incremento il contatore delle porte del circuito */

        char** temp = realloc(circ, nPort * sizeof(char*));   /* A partire dalla grandezza dello spazio di memoria del parametro "circ", creo in "temp" una dimensinoe sufficente */
                                                                            /* a memorizzare tutti gli id senza sprecare memoria */
            if (!temp) {    
                free(circ);                             /* Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione */
                fprintf(stderr, "Errore realloc\n");    /*  dopo aver liberato lo spazio di memoria occupato */
                exit(1);                                
            }
            circ = temp;    /* Riassegno a circ lo spazio di memoria ora allargato della dimensione perfetta per ospitare la sequenza */

            circ[nPort-1] = malloc((strlen(id)+1) * sizeof(char));  /* Alloco dinamicamente lo spazio di memoria per l'id della porta corrente, che è una stringa */
            if(circ[nPort-1] == NULL) error("Errore allocazione memoria dell'id di una matrice in circ (main)");    /* Controllo che l'allocazione della memoria sia andata a buon fine */
                                                                                                                    /* se no, stampo un messaggio di errore e termino l'esecuzione */
            strcpy(circ[nPort-1], id);  /* Copio l'id della porta corrente nella posizione corretta dell'array di puntatori a stringhe circ */

        id = strtok(NULL, " ");   /* Continuo a leggere gli id delle porte del circuito, separati da uno spazio */
    }
    free(circStr);  /* Libero lo spazio di memoria dedicato alla stringa circStr, che ora non serve più */

    cmatrix** port = malloc(nPort * sizeof(cmatrix*));  /* Alloco dinamicamente lo spazio di memoria per un array di puntatori a matrici di numeri complessi, che conterrà le porte del circuito */
    if (port == NULL) {
        perror("Errore allocazione memoria per puntatori a matrici di porte (main)");   /* Controllo che l'allocazione della memoria sia andata a buon fine */
                                                                                        /* Se no, stampo un messaggio di errore e */
        exit(EXIT_FAILURE);
    }

    int i, j;
    for(i = 0; i < nPort; i++) {    /* Per ogni porta del circuito, alloco dinamicamente lo spazio di memoria per una matrice di numeri complessi */
        port[i] = malloc(sizeof(cmatrix)); /* Alloca la struttura cmatrix */
        if(port[i] == NULL) {                                                               /* Controllo che l'allocazione della memoria sia andata a buon fine */
            perror("Errore allocazione memoria della struttura cmatrix per porta (main)");  /* Se no, stampo un messaggio di errore  */
            for(j = 0; j < i; j++) {   /* Libera la memoria già allocata prima di uscire */
                freeMatrice(port[j]);
                free(port[j]);
            }
            free(port); 
            exit(EXIT_FAILURE); /* ed esco dal programma */
        }
        creaMatrice(port[i], dim); /* Inizializza i dati interni della matrice */

        int presente = -1;
        for(j = 0; j < i; j++) {   /* Controllo se l'id della porta corrente è già stato inserito nell'array di puntatori a matrici di porte */
            if(strcmp(circ[i], port[j]->id) == 0) {  /* Se l'id della porta corrente è già presente, allora non devo creare una nuova matrice */
                presente = j;  /* Imposto il flag presente a true */
            }
        }

        if(presente == -1) {  /* Se l'id della porta corrente è già presente, allora non devo creare una nuova matrice */
            getMatrix(port[i], circContent, circ[i], dim);  /* Con il metodo getMatrix, valorizzo la matrice port[i] con i dati nel file "circ.txt" dopo "#circ" */ 
        } else {
            copiaMatrice(port[i], *port[presente]); /* Se l'id della porta corrente è già presente, allora copio la matrice già esistente nella matrice corrente */
        }


    }
    
    free(circPath); /* Libero lo spazio di memoria dedicato al path del file circ, che ora non serve più */
    free(circContent);  /* Libero lo spazio di memoria dedicato al contenuto file "circ.txt", che ora non serve più */

    int thread_necessari = atoi(argv[3]);   /* Inizializzo il numero di thread necessari, che sarà pari al terzo argomento passato in input al programma */

    char *endptr;
    thread_necessari = strtol(argv[3], &endptr, 10); /* Con strtol, converto il terzo argomento passato in input al programma in un intero, che rappresenta il numero di thread necessari */
    if (*endptr != '\0') {
        printf("Errore: il terzo argomento deve essere un numero intero valido.\n");    /* Controllo che il terzo argomento passato in input al programma sia un numero intero valido */
        return 1;
    }

    coda top;   /* Creo la coda che conterrà le matrici da moltiplicare */
    coda bottom;    /* Creo la coda che conterrà i risultati delle moltiplicazioni */
    creaCoda(&top, nPort);  /* Inizializzo la coda che conterrà le matrici da moltiplicare */
    creaCoda(&bottom, nPort);   /* Inizializzo la coda che conterrà i risultati delle moltiplicazioni */
    for(i = nPort-1; i >= 0; i--) {
        enqueue(&top, port[i]); /* Inserisco le matrici delle porte nella coda top, in modo che siano pronte per essere moltiplicate */
    }       
    
    if(thread_necessari > nPort / 2) thread_necessari = nPort / 2; /* Se il numero di thread richiesti è maggiore della metà delle porte, lo riduco a metà per evitare di creare più thread del necessario */
    pthread_t threads[thread_necessari];    /* Inizializzo l'array di thread che verranno utilizzati per eseguire le moltiplicazioni in parallelo */

    int prodotti_attesi = 0;    /* Inizializzo il numero di prodotti attesi, che sarà pari alla metà del numero di matrici nella coda top */
    int prodotti_effettuati = 0;    /* Inizializzo il numero di prodotti effettuati, che sarà inizialmente 0 */

    while(top.occupato > 1) {   /* Finché ci sono più di una matrice nella coda top, continuo a moltiplicare le matrici */
        
        prodotti_attesi = top.occupato / 2; /* Calcolo il numero di prodotti attesi, che sarà pari alla metà del numero di matrici nella coda top */
        
        while(prodotti_effettuati < prodotti_attesi) {  /* Finché non ho effettuato il numero di prodotti attesi, continuo a moltiplicare le matrici */
           
            if(thread_necessari > prodotti_attesi-prodotti_effettuati) thread_necessari = prodotti_attesi-prodotti_effettuati;  /* Se il numero di thread richiesti è maggiore del numero di prodotti attesi, lo riduco al numero di prodotti attesi */
            
            thread_data* data = malloc(thread_necessari * sizeof(thread_data)); /* Alloco dinamicamente lo spazio di memoria per l'array di strutture thread_data, che conterrà i dati da passare ai thread */
            
            for(i = 0; i < thread_necessari; i++) { /* Per ogni thread necessario, creo una struttura thread_data che conterrà i dati da passare al thread */
                
                cmatrix* m1 = dequeue(&top);    /* Dequeue restituisce la matrice  1 da moltiplicare in testa alla coda top e la rimuove dalla coda */
                cmatrix* m2 = dequeue(&top);    /* Dequeue restituisce la matrice  1 da moltiplicare in testa alla coda top e la rimuove dalla coda */
                cmatrix* res = malloc(sizeof(cmatrix)); /* Alloco dinamicamente lo spazio di memoria per la matrice di risultato, che conterrà il risultato della moltiplicazione delle due matrici */

                creaMatrice(res, dim);  /* Inizializzo i dati interni della matrice di risultato */
                
                data[i].m1 = m1;    /* Assegno il puntatore alla prima matrice alla struttura thread_data */
                data[i].m2 = m2;    /* Assegno il puntatore alla seconda matrice alla struttura thread_data */
                data[i].res = res;  /* Assegno il puntatore alla matrice di risultato alla struttura thread_data */

                pthread_create(&threads[i], NULL, moltiplicaMatrici_thread, &data[i]);  /* Creo il thread che eseguirà la moltiplicazione delle due matrici, passando i dati della struttura thread_data come argomento */
            }
            
            for(i = 0; i < thread_necessari; i++) { /* Attendo che tutti i thread abbiano terminato l'esecuzione */
                pthread_join(threads[i], NULL);

                freeMatrice(data[i].m1);    /* Libero lo spazio di memoria dedicato alla matrice m1, che ora non serve più */
                free(data[i].m1);   /* Libero lo spazio di memoria dedicato al puntatore m1, che ora non serve più */

                freeMatrice(data[i].m2);    /* Libero lo spazio di memoria dedicato alla matrice m2, che ora non serve più */
                free(data[i].m2);   /*  Libero lo spazio di memoria dedicato al puntatore m2, che ora non serve più */

                enqueue(&bottom, data[i].res);  /* Inserisco la matrice di risultato nella coda bottom, che conterrà i risultati delle moltiplicazioni */
                prodotti_effettuati++;  /* Incremento il numero di prodotti effettuati */
            }
            free(data); /* Libero lo spazio di memoria dedicato all'array di strutture thread_data, che per questo ciclo non serve più */
        }
        
        if(top.occupato == 1) { 
            enqueue(&bottom, dequeue(&top));    /* Se nella coda top è rimasta una sola matrice, la inserisco nella coda bottom */
        }

        coda tmp = top; 
        top = bottom;   /* Scambio le code top e bottom, in modo che la coda top contenga i risultati delle moltiplicazioni e la coda bottom contenga le matrici da moltiplicare */
        bottom = tmp;   

        prodotti_effettuati = 0;    /* Inizializzo il numero di prodotti effettuati a 0, in modo da poter ricominciare il ciclo con un nuovo set di matrici da moltiplicare */
    }
    
    cmatrix *out = dequeue(&top);   /* Dequeue restituisce la matrice finale risultato dei prodotti in testa alla coda top e la rimuove dalla coda, che sarà l'unica matrice rimasta nella coda top */

    comp vfin[dim]; /* Creo il vettore che rappresenterà la stato finale del circuito di dimenione dim */
    calcOut(*out, init, vfin);  /* Calcolo il contenuto del vettore finale */
    
    printf("\nStato finale del circuito:\n");                       /* Infine, stampo in stdout il vettore corrispondente allo stato finale del circuito */
    printVector(vfin, dim);                                         

    freeCoda(&top); /* Libero la coda top, che ora non serve più */
    freeCoda(&bottom);  /* Libero la coda bottom, che ora non serve più */
    for(i = 0; i < nPort; i++) {
        free(circ[i]);  /* Libero lo spazio di memoria dedicato alla stringa che contiene l'id di ogni porta, che ora non servono più */
    }
    free(circ);     /* Libero lo spazio di memoria dedicato alla sequenza di porte, che ora non serve più */
   
    freeMatrice(out);   /* Libero lo spazio di memoria dedicato alla matrice di risultato finale, che ora non serve più */
    free(out);  /* Libero lo spazio di memoria dedicato al puntatore alla matrice di risultato finale, che ora non serve più */

    free(port);  /* libera l'array di puntatori */

    return 0;
}