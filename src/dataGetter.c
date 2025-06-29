#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/types.h"
#include "../include/util.h"
#include "../include/stringManipolation.h"

// Il metodo getNqubit cerca un valore intero specificato dopo "#qubits" nel contenuto del file passato in input, e se lo trova lo restituisce in output
int getNqubit(char* content) {
    char* copiaContent = malloc(strlen(content)+1); // Dedico una porzione di memoria ad una copia del file in input, per evitare effetti collaterali sul contenuto originale
    if(copiaContent == NULL) error("Errore allocazione memoria in getNqubits"); // Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione
    strcpy(copiaContent, content);  // Copio il contenuto del file nella nuova stringa
    
    int out = 0;    // Il parametro "out" conterrà il valore definito dopo "#qubits", qualora venisse trovato

    char* riga = strtok(copiaContent, "#"); // Inizializzo la variabile "riga" con la prima riga del file, separando le righe con il carattere "#"

    bool found = false; // Il parametro "found" verrà impostato a True qualora il valore cercato dovesse essere trovato, e verrà lasciato a False altrimenti
    while (riga != NULL) {
        if(strncmp(riga, "qubits", 6) == 0) {  // Finché ci sono righe nel file, controllo se i primi 7 caratteri della riga corrente corrispondono all'identificatore "#qubits"
            found = true;   // se l'identificatore "#qubits" è stato trovato all'inizio della riga allora imposto il parametro "found" a True

            char temp[strlen(riga)-5];  // Creo una stringa che conterrà il valore specificato dopo "#qubits" della dimensione sufficiente a comprendere tutto meno l'identifictore stesso
            getSubstring(temp, riga, 6, strlen(riga)+1);    // Copio nella stringa "temp" il contenuto del resto della riga (tutto meno l'identificatore)
            if(strcmp(temp, "") == 0) { // Se la stringa "temp" è vuota, significa che non è stato specificato un valore dopo "#qubits"
                fprintf(stderr, "Quantità di qubits mal formattata\n"); // Qualora la stringa "temp" fosse vuota   
                exit(1);                                                // il programma verrebbe interrotto con un errore
            }

            out = atoi(temp);   // Converto la stringa in un numero intero
        }
        riga = strtok(NULL, "#"); // Proseguo nel controllo delle righe, andando alla prossima chiamando nuovamente strtok()
        if(found) riga = NULL;  // Seleziono solo la prima riga corretta che incontro, ignorando altre eventuali
    }
    free(copiaContent); // Libero lo spazio di memoria dedicato alla copia del contenuto del file, che ora non serve più

    if(!found) { // Se il parametro "found" non è stato impostato a True, significa che l'identificatore "#qubits" non è stato trovato
        fprintf(stderr, "identificatore #qubits NON TROVATO\n");    // Qualora l'identificatore "#qubits" non dovesse essere stato trovato all'inizio di nessuna riga
        exit(1);                                                    // il programma verrebbe interrotto con un errore
    }

    return out; // Restituisco il numero trovato
}

// Il metodo getInit cerca il vettore di numeri complessi specificato dopo "#init" nel contenuto del file passato in input, e se lo trova lo restituisce in output
void getInit(char* content, comp* init, int dim) {

    int complexCounter = 0; // Il parametro "complexCounter" tiene traccia della quantità di numeri complessi trovati nel vettore

    char* copiaContent = malloc(strlen(content)+1); // Dedico una porzione di memoria ad una copia del file in input, per evitare effetti collaterali sul contenuto originale
    if(copiaContent == NULL) error("Errore allocazione memoria in getInit");    // Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione
    strcpy(copiaContent, content);  // Copio il contenuto del file nella nuova stringa

    char* riga = strtok(copiaContent, "#"); // Inizializzo la variabile "riga" con la prima riga del file, separando le righe con il carattere "#"
    bool found = false; // Il parametro "found" verrà impostato a True qualora il vettore cercato dovesse essere trovato, e verrà lasciato a False altrimenti
    while (riga != NULL) {
        if(strncmp(riga, "init", 4) == 0) {    // Finché ci sono righe nel file, controllo se i primi 5 caratteri della riga corrente corrispondono all'identificatore "#init"
            found = true;   // se l'identificatore "#init" è stato trovato all'inizio della riga allora imposto il parametro "found" a True

            char* pos_start = strchr(riga, '[');    // Dando per scontato che la sintassi del vettore preveda un solo carattere '[', ne ricavo la posizione con il metodo strchr() e la memorizzo nel parametro "start"
            if (pos_start == NULL) {    // Se il carattere '[' non viene trovato, significa che la sintassi del vettore non è corretta
                fprintf(stderr, "Errore: carattere '[' non trovato nel vettore init\n");    // Qualora il carattere '[' non dovesse essere stato trovato nella riga
                exit(1);                                                                    // il programma verrebbe interrotto con un errore
            }
            int start = (int)(pos_start - riga)+1;    // Dando per scontato che la sintassi del vettore preveda un solo carattere '[', ne ricavo la posizione con il metodo strchr() e la memorizzo nel parametro "end"

            char* pos_end = strchr(riga, ']');  // Dando per scontato che la sintassi del vettore preveda un solo carattere ']', ne ricavo la posizione con il metodo strchr() e la memorizzo nel parametro "end"
            if (pos_end == NULL) {  // Se il carattere ']' non viene trovato, significa che la sintassi del vettore non è corretta
                fprintf(stderr, "Errore: carattere ']' non trovato nel vettore init\n");    // Qualora il carattere ']' non dovesse essere stato trovato nella riga
                exit(1);                                                                    // il programma verrebbe interrotto con un errore
            }
            int end = (int)(pos_end - riga);  // Dando per scontato che la sintassi del vettore preveda un solo carattere ']', ne ricavo la posizione con il metodo strchr() e la memorizzo nel parametro "end"


            char init_s[end-start]; // Dichiaro una stringa "init_s" che memorizzerà la sottostringa contenente solo e unicamente il vettore dichiarato dopo "#init"
            getSubstring(init_s, riga, start, end); // Con il metodo getSubstring() creo in "init_s" una copia della sottostringa corrispondente al vettore dichiarato dopo "#init"
            
            char* num = strtok(init_s, ","); // Utilizzo il metodo strtok() con divisore "," per poter controllare singolarmente i numeri complessi nel vettore
            
            int i = 0;  // In "i" memorizzo la quantità di numeri complessi trovati, così che anche se ne dovessero essere stati inseriti più di quanto ammesso dal valore definito dopo "#qubit",
                        // il programma prenda in considerazione solo i primi che rientrano nel valore consentito
            while(num != NULL) {
                parseComplesso(&init[i], num);  // Finché ci sono numeri complessi nel vettore, utilizzo il metodo parseComplesso() per convertire la stringa in una struct comp

                i++;    // Incrementando "i", tengo il conto dei numeri complessi trovati
                complexCounter++;   // Incrementando "complexCounter", tengo il conto dei numeri complessi trovati
                num = strtok(NULL, ",");    // Proseguo nel controllo dei numeri complessi, andando al prossimo chiamando nuovamente strtok()  
                if(i >= dim) break; // Se dovessero essere stati inseriti più numeri complessi di quanto ammesso dal valore definito dopo "#qubit",
            }                       // il programma prenda in considerazione solo i primi che rientrano nel valore consentito
        }
        riga = strtok(NULL, "#");   // Proseguo nel controllo delle righe, andando alla prossima chiamando nuovamente strtok()
        if (found) riga = NULL; // Seleziono solo la prima riga corretta che incontro, ignorando altre eventuali
    }    
    free(copiaContent); // Libero lo spazio di memoria dedicato alla copia del contenuto del file, che ora non serve più


    if(!found) {
        fprintf(stderr, "identificatore #init NON TROVATO\n");  // Qualora l'identificatore "#init" non dovesse essere stato trovato all'inizio di nessuna riga
        exit(1);                                                // il programma verrebbe interrotto con un errore
    }

    if(complexCounter != dim) { // Se il numero di numeri complessi trovati non corrisponde alla dimensione del vettore, allora il programma non può proseguire 
        fprintf(stderr, "Dimensione del vettore non corrispondente al numero dei Qubits\n");    // Qualora il numero di numeri complessi trovati non corrispondesse alla dimensione del vettore
        exit(1);                                                                                // il programma verrebbe interrotto con un errore
    }
}

// Il metodo getMatrix cerca la matrice di numeri complessi specificata dopo "#define" e l'id corrispondente all'omonimo parametro in input.
// La ricreca avviene nel contenuto del file passato anch'esso passato in input, e se la matrice viene trovata viene assegnata al parametro in input "m"
void getMatrix(cmatrix* m, char* content, char* id, int dimensione) {

    int complexCounter = 0; // Il parametro "complexCounter" tiene traccia della quantità di numeri complessi trovati nella matrice

    char* copiaContent = malloc(strlen(content)+1); // Dedico una porzione di memoria ad una copia del file in input, per evitare effetti collaterali sul contenuto originale
    if(copiaContent == NULL) error("Errore allocazione memoria in getMatrix");  // Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione
    strcpy(copiaContent, content);  // Copio il contenuto del file nella nuova stringa

    char* riga = strtok(copiaContent, "#"); // Inizializzo la variabile "riga" con la prima riga del file, separando le righe con il carattere "#" 

    bool found = false; // Il parametro "found" verrà impostato a True qualora la matrice cercata dovesse essere trovata, e verrà lasciato a False altrimenti
    while (riga != NULL) {
        if(strncmp(riga, "define", 6) == 0) {  // Finché ci sono righe nel file, controllo se i primi 7 caratteri della riga corrente corrispondono all'identificatore "#define"
            char* tempStr = malloc(strlen(riga)-5);   // Creo una stringa che conterrà la sottostringa dopo "#define" della dimensione sufficiente a comprendere tutto meno l'identifictore stesso
            tempStr[0] = '\0';  // Inizializzo la stringa "tempStr" con un carattere di terminazione
            getSubstring(tempStr, riga, 6, strlen(riga)-5); // Copio nella stringa "tempStr" il contenuto del resto della riga (tutto meno l'identificatore)
            rimuoviCarattere(tempStr, " "); // Rimuovo gli spazi dalla stringa "tempStr"

            char* idFound = malloc(strlen(id)+1);   // Dichiaro una stringa "idFound" che conterrà l'id della matrice, per poterlo confrontare con l'id passato in input
            idFound[0] = '\0';  // Inizializzo la stringa "idFound" con un carattere di terminazione
            getSubstring(idFound, tempStr, 0, strlen(id));  // Con il metodo getSubstring() creo in "idFound" una copia della sottostringa corrispondente all'id della matrice dichiarato dopo "#define"

            if(strcmp(idFound, id) == 0) {  // Controllo se l'id della matrice corrisponde a l'omonimo parametro passato in input
                found = true;   // se l'identificatore "#define" con il corretto id è stato trovato all'inizio della riga allora imposto il parametro "found" a True

                free(m->id);    // Libero lo spazio di memoria precedentemente allocato per l'id della matrice
                m->id = strdup(id);  // strdup già alloca la memoria e copia la stringa
                if (!m->id) {   // Se la memoria non è stata allocata correttamente, il programma viene interrotto con un errore
                    free(copiaContent);
                    fprintf(stderr, "Errore strdup\n"); // Qualora la memoria non dovesse essere allocata correttamente
                    exit(1);                            // il programma verrebbe interrotto con un errore
                }
                
                char* pos_start = strchr(riga, '[');    // Dando per scontato che la sintassi del vettore preveda un solo carattere '[', ne ricavo la posizione con il metodo strchr() e la memorizzo nel parametro "start"
                if (pos_start == NULL) {    // Se il carattere '[' non viene trovato, significa che la sintassi del vettore non è corretta
                    fprintf(stderr, "Errore: carattere '[' non trovato nel vettore init\n");    // Qualora il carattere '[' non dovesse essere stato trovato nella riga
                    exit(1);                                                                    // il programma verrebbe interrotto con un errore
                }
                int start = (int)(pos_start - riga)+1;    // Dando per scontato che la sintassi del vettore preveda un solo carattere '[', ne ricavo la posizione con il metodo strchr() e la memorizzo nel parametro "start"
    
                char* pos_end = strchr(riga, ']');  // Dando per scontato che la sintassi del vettore preveda un solo carattere ']', ne ricavo la posizione con il metodo strchr() e la memorizzo nel parametro "end"
                if (pos_end == NULL) {  // Se il carattere ']' non viene trovato, significa che la sintassi del vettore non è corretta
                    fprintf(stderr, "Errore: carattere ']' non trovato nel vettore init\n");    // Qualora il carattere ']' non dovesse essere stato trovato nella riga
                    exit(1);                                                                    // il programma verrebbe interrotto con un errore
                }
                int end = (int)(pos_end - riga);    // Dando per scontato che la sintassi del vettore preveda un solo carattere ']', ne ricavo la posizione con il metodo strchr() e la memorizzo nel parametro "end"

                char* matrixContent = malloc(end-start+1); // Dichiaro una stringa "matrixContent" che memorizzerà la sottostringa contenente solo e unicamente la matrice dichiarato dopo "#define" con il corretto id
                matrixContent[0] = '\0';
                getSubstring(matrixContent, riga, start, end);  // Con il metodo getSubstring() creo in "matrixContent" una copia della sottostringa corrispondente al vettore dichiarato dopo "#define" con il corretto id

                rimuoviCarattere(matrixContent, "( ");  // Rimuovo gli spazi e le parentesi dalla stringa "matrixContent", in modo da poterla gestire più facilmente

                int i = 0;  // Il parametro "i" corrisponderà all'indice di riga della matrice
                int j = 0;  // Il parametro "j" corrisponderà all'indice di colonna della matrice

                char* pt1;  // Dichiaro due puntatori distinti per gestire due strtok() annidati in contemporanea
                char* pt2;  //

                int countVettori = 0;   // Il parametro "countVettori" tiene traccia della quantità di vettori trovati nella matrice, in modo da poter verificare che la matrice sia completa
                
                char* vettoreComplesso = strtok_r(matrixContent, ")", &pt1);    // Utilizzo il metodo strtok_r() con divisore ")" per poter controllare singolarmente i vettori nella matrice 
                while(vettoreComplesso != NULL) {   // Finché ci sono vettori nella matrice, utilizzo il metodo strtok_r() con divisore ")" per poter controllare singolarmente i vettori nella matrice
                    
                    countVettori++; // Incremento il conteggio dei vettori trovati nella matrice

                    char* numComplesso = strtok_r(vettoreComplesso, ",", &pt2); // Per ogni vettore nella matrice, utilizzo il metodo strtok_r() con divisore "," per poter controllare singolarmente i numeri complessi nei vettori

                    j = 0;  // All'inizio di ogni riga, resetto l'indice di colonna
                    while(numComplesso != NULL) {

                        parseComplesso(&m->m[i][j], numComplesso);  // Per ogni numero complesso in ogni vettore, utilizzo il metodo parseComplesso() per convertire la stringa in una struct comp
                        complexCounter++;   // Incrementando "complexCounter", tengo il conto dei numeri complessi trovati

                        j++;    // Incremento l'indice di colonna
                        numComplesso = strtok_r(NULL, ",", &pt2);      // Proseguo nel controllo dei numeri complessi nel vettore, andando al prossimo chiamando nuovamente strtok()
                    }
                    i++;    // Incremento l'indice di riga             
                    vettoreComplesso = strtok_r(NULL, ")", &pt1);   // Proseguo nel controllo dei vettori nella matrice, andando al prossimo chiamando nuovamente strtok()
                }
                free(matrixContent); // Libero lo spazio di memoria dedicato al contenuto della matrice, che ora non serve più

                if(countVettori < dimensione) { // Se il numero di vettori trovati nella matrice è inferiore alla dimensione della matrice, significa che la matrice non è completa
                    fprintf(stderr, "Errore formattazione vettori in matrice %s\n", m->id); // Qualora il numero di vettori trovati nella matrice fosse inferiore alla dimensione della matrice
                    exit(1);                                                                // il programma verrebbe interrotto con un errore
                }

            }
            free(tempStr); // Libero lo spazio di memoria dedicato alla riga dopo il "#define", che ora non serve più
            free(idFound);  // Libero lo spazio di memoria dedicato all'id della matrice, che ora non serve più
        }
        riga = strtok(NULL, "#"); // Proseguo nel controllo delle righe, andando alla prossima chiamando nuovamente strtok()

        if(found) riga = NULL;  // Seleziono solo la prima riga corretta che incontro, ignorando altre eventuali
    }
    free(copiaContent); // Libero lo spazio di memoria dedicato alla copia del contenuto del file, che ora non serve più

    if(!found) {    // Se il parametro "found" non è stato impostato a True, significa che l'identificatore "#define" con il corretto id non è stato trovato
        fprintf(stderr, "identificatore #define (%s) NON TROVATO\n", id); // Qualora l'identificatore "#init" non dovesse essere stato trovato all'inizio di nessuna riga
        exit(1);                                                        // il programma verrebbe interrotto con un errore
    }

    if(complexCounter != dimensione*dimensione) {   // Se il numero di numeri complessi trovati non corrisponde alla dimensione della matrice, allora il programma non può proseguire
        fprintf(stderr, "Dimensione delle matrici non corrispondente al numero dei Qubits\n");  // Qualora il numero di numeri complessi trovati non corrispondesse alla dimensione della matrice  
        exit(1);                                                                                // il programma verrebbe interrotto con un errore                                                   
    }
}

// Il metodo getCirc cerca la sequenza di id delle matrici specificata dopo "#circ" nel contenuto del file passato in input, e se la trova viene assegnata al parametro in input "circ"
void getCirc(char* content, char** circ) {  
    char* copiaContent = malloc(strlen(content)+1); // Dedico una porzione di memoria ad una copia del file in input, per evitare effetti collaterali sul contenuto originale
    if(copiaContent == NULL) error("Errore allocazione memoria in getCirc");    // Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione
    strcpy(copiaContent, content);  // Copio il contenuto del file nella nuova stringa

    char* riga = strtok(copiaContent, "#"); // Inizializzo la variabile "riga" con la prima riga del file, separando le righe con il carattere "#"   

    bool found = false; // Il parametro "found" verrà impostato a True qualora la sequenza cercata dovesse essere trovata, e verrà lasciato a False altrimenti
    while (riga != NULL) {
        if(strncmp(riga, "circ", 4) == 0) {    // Finché ci sono righe nel file, controllo se i primi 5 caratteri della riga corrente corrispondono all'identificatore "#circ"
            found = true;   // se l'identificatore "#circ" è stato trovato all'inizio della riga allora imposto il parametro "found" a True

            char tempStr[strlen(riga)-4];   // Creo una stringa che conterrà il valore specificato dopo "#circ" della dimensione sufficiente a comprendere tutto meno l'identifictore stesso
            getSubstring(tempStr, riga, 4, strlen(riga)+1);  // Copio nella stringa "tempStr" il contenuto del resto della riga (tutto meno l'identificatore)

            char* temp = realloc(*circ, strlen(tempStr)+1 * sizeof(char));   // A partire dalla grandezza dello spazio di memoria del parametro "circ", creo in "temp" una dimensinoe sufficente
                                                                            // a memorizzare tutti gli id senza sprecare memoria
            if (!temp) {    // Se la memoria non è stata allocata correttamente, il programma viene interrotto con un errore  
                free(copiaContent);
                free(*circ);                             // Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione
                fprintf(stderr, "Errore realloc\n");    //  dopo aver liberato lo spazio di memoria occupato
                exit(1);                                //
            }
            *circ = temp;    // Riassegno a circ lo spazio di memoria ora allargato della dimensione perfetta per ospitare la sequenza

            strcpy(*circ, tempStr);  // Copio la sequenza di id dentro il parametro "circ"
        }
        riga = strtok(NULL, "#"); // Proseguo nel controllo delle righe, andando alla prossima chiamando nuovamente strtok()

        if(found) riga = NULL;  // Seleziono solo la prima riga corretta che incontro, ignorando altre eventuali  
    }    
    free(copiaContent); // Libero lo spazio di memoria dedicato alla copia del contenuto del file, che ora non serve più

    if(!found) {    // Se il parametro "found" non è stato impostato a True, significa che l'identificatore "#circ" non è stato trovato
        fprintf(stderr, "identificatore #circ NON TROVATO\n");  // Qualora l'identificatore "#init" non dovesse essere stato trovato all'inizio di nessuna riga
        exit(1);                                                // il programma verrebbe interrotto con un errore
    }   
}