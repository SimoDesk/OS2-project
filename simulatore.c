#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct {    // Struttura che rappresenta un numero complesso
    double r;   // Il valore r corrisopnde alla parte reale del numero complesso
    double i;   // Il valore i corrisponde alla parte immaginaria del numero complesso
} comp;

typedef struct {    // Struttura che rappresenta le matrici di numeri complessi
    comp** m;   // Il parametro m in quanto puntatore di puntatori di numeri complessi, rappresenta la matrice vera e propria
    char id;    // Il valore id conterrà la lettera identificativa della matrice
    int dim;    // Il valore dim conterrà la dimensione della matrice, che in quanto quadrata vede sufficente una sola dimensione
} cmatrix;

// Il metodo error() ha lo scopo di risparimare qualche riga di codice per interrompere il programma
// in caso di errori di sistema, comunicando un messaggio preso in input
void error(char* messaggio) {   
    perror(messaggio);          
    exit(1);
}

// Il metodo printComplex() ha lo scopo di stampare in maniera leggibile il numero complesso preso in input
void printComplex(comp n) {         
    printf("%lf i%lf\n", n.r, n.i); 
}

// Il metodo printVector() ha lo scopo di stampare in maniera leggibile il vettore di numeri complessi preso in input
void printVector(comp* vett, int dim) {
    for(int i = 0; i < dim; i++) {
        printf("[%i] ", i);
        printComplex(vett[i]);
    }
    printf("\n");
}


// Il metodo printMatrix() ha lo scopo di stampare in maniera leggibile la matrice di numeri complessi preso in input
void printMatrix(cmatrix matrix) {
    int dim = matrix.dim;
    printf("Matrice %c: \n", matrix.id);
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            printf("[%i][%i] ", i, j);
            printComplex(matrix.m[i][j]);
        }
    }
    printf("\n");
}

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

// Il metodo creaMatrice() inizializza lo spazio necessario per i dati della matrice, ne memorizza la dimensione e assegna un carattere "nullo" all'id
void creaMatrice(cmatrix* m, int dimensione) {
    m->id = '-';    // Carattere generico per l'id che verrà comunque sovrascritto in seguito
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

// Il metodo rimuoviCarattere() toglie dalla stringa in input "str" tutti i caratteri uguali al parametro in input "carattere"
void rimuoviCarattere(char* str, char carattere) {
    int j = 0;  // Inizializzo un contatore che corrisponderà alla nuova lunghezza della stringa contando gli eventuali caratteri rimossi
    for(int i = 0; i < strlen(str); i++) {
        if(str[i] != carattere) {   
            str[j] = str[i];    // Per ogni carattere della stringa in input "str", controllo se questo è diverso dal parametro in input "carattere" e se lo è lo aggiungo alla nuova stringa filtrata
            j++;    // incremento del contatore
        } 
    }
    str[j] = '\0';  // Aggiungo in fondo alla stringa il carattero di chiusura '\0' per renderla una stringa valida
}

// Il metodo getSubstring() preso in input la stringa da cui estrarre un segmento, inserisce nel parametro in input "sottostringa" tutti i caratteri compresi tra l'indice di inizio "start"
// e l'indice di fine "end" della stringa di partenza
void getSubstring(char* sottostringa, char* stringa, int start, int end) {
    int j = 0;  // Inizializzo un contatore che corrisponderà alla lunghezza della sottostringa
    for(int i = start+1; i < end; i++) {
        sottostringa[j] = stringa[i];   // Per ogni carattere del segmento evidentziato dai parametri in input, ne creo una copia dalla stringa di partenza alla sottostringa
        j++;    // incremento il contatore
    }
    sottostringa[j] = '\0'; // Aggiungo in fondo alla sottostringa il carattero di chiusura '\0' per renderla una stringa valida
}

char* getContent(char path[]) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror("Errore apertura file in getContent");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long dimensioneFile = ftell(f);
    rewind(f);

    char* out = malloc(dimensioneFile + 1);
    if (out == NULL) error("Errore allocazione memoria in getContent");
    
    if(dimensioneFile != fread(out, 1, dimensioneFile, f)) {
        free(out);
        fclose(f);
        perror("Errore lettura file in getContent");
        exit(1);
    }
    fclose(f);

    return out;
}

int getNqubit(char* content) {
    char* copiaContent = malloc(strlen(content)+1);
    if(copiaContent == NULL) error("Errore allocazione memoria in getNqubits");
    strcpy(copiaContent, content);
    
    int out = 0;

    char* riga = strtok(copiaContent, "\n");

    bool found = false;
    while (riga != NULL) {
        if(strncmp(riga, "#qubits", 7) == 0) {
            found = true;

            char temp[strlen(riga)-6];
            getSubstring(temp, riga, 7, strlen(riga)+1);
            rimuoviCarattere(temp, ' ');

            out = atoi(temp);
        }
        riga = strtok(NULL, "\n");  // prossima riga
    }
    free(copiaContent);

    if(!found) {
        fprintf(stderr, "identificatore #qubits NON TROVATO\n");
        exit(1);
    }
    return out;
}

void* getInit(char* content, comp* init, int dim) {

    char* copiaContent = malloc(strlen(content)+1);
    if(copiaContent == NULL) error("Errore allocazione memoria in getInit");
    strcpy(copiaContent, content);

    char* riga = strtok(copiaContent, "\n");

    bool found = false;
    while (riga != NULL) {
        if(strncmp(riga, "#init", 5) == 0) {
            found = true;

            int start = (int)(strchr(riga, '[') - riga);
            int end = (int)(strchr(riga, ']') - riga);
            
            char init_s[end-start];
            getSubstring(init_s, riga, start, end);

            rimuoviCarattere(init_s, ' ');
            
            char* num = strtok(init_s, ",");
            
            int i = 0;
            while(num != NULL) {
                parseComplesso(&init[i], num);

                i++;
                num = strtok(NULL, ",");
                if(i >= dim) break;
            }
        }
        riga = strtok(NULL, "\n");  // prossima riga       
    }    
    free(copiaContent);

    if(!found) {
        fprintf(stderr, "identificatore #init NON TROVATO\n");
        exit(1);
    }
}

void getMatrix(cmatrix* m, char* content, char id, int dimensione) {
    m->id = id;

    char* copiaContent = malloc(strlen(content)+1);
    if(copiaContent == NULL) error("Errore allocazione memoria in getMatrix");
    strcpy(copiaContent, content);

    char* riga = strtok(copiaContent, "\n");

    bool fuond = false;
    while (riga != NULL) {
        if(strncmp(riga, "#define", 7) == 0) {
            char tempStr[strlen(riga)-6];
            getSubstring(tempStr, riga, 7, strlen(riga)+1);
            rimuoviCarattere(tempStr, ' ');

            if(tempStr[0] == id) {
                fuond = true;

                int start = (int)(strchr(riga, '[') - riga);
                int end = (int)(strchr(riga, ']') - riga)-1;
                
                char matrixContent[end-start];
                getSubstring(matrixContent, riga, start, end);

                rimuoviCarattere(matrixContent, '(');
                rimuoviCarattere(matrixContent, ' ');

                int i = 0;
                int j = 0;
                comp temp;

                char* pt1;
                char* pt2;
                char* vettoreComplesso = strtok_r(matrixContent, ")", &pt1);
                while(vettoreComplesso != NULL) {
                    char* numComplesso = strtok_r(vettoreComplesso, ",", &pt2);

                    j = 0;
                    while(numComplesso != NULL) {

                        parseComplesso(&temp, numComplesso);
                        m->m[i][j] = temp;

                        j++;
                        numComplesso = strtok_r(NULL, ",", &pt2);
                    }
                    i++;               
                    vettoreComplesso = strtok_r(NULL, ")", &pt1);
                }
            }
        }
        riga = strtok(NULL, "\n");  // prossima riga
    }
    free(copiaContent);

    if(!fuond) {
        fprintf(stderr, "identificatore #define %c NON TROVATO\n", id);
        exit(1);
    }
}

void* getCirc(char* content, char* circ) {
    char* copiaContent = malloc(strlen(content)+1);
    if(copiaContent == NULL) error("Errore allocazione memoria in getCirc");
    strcpy(copiaContent, content);

    char* riga = strtok(copiaContent, "\n");

    bool found = false;
    while (riga != NULL) {
        if(strncmp(riga, "#circ", 5) == 0) {
            found = true;

            char tempStr[strlen(riga)-4];
            getSubstring(tempStr, riga, 5, strlen(riga)+1);
            rimuoviCarattere(tempStr, ' ');

            char* temp = realloc(circ, strlen(tempStr)+1 * sizeof(char));
            if (!temp) {
                free(circ);
                fprintf(stderr, "Errore realloc\n");
                exit(1);
            }
            circ = temp;

            strcpy(circ, tempStr);

        }
        riga = strtok(NULL, "\n");  // prossima riga       
    }    
    free(copiaContent);

    if(!found) {
        fprintf(stderr, "identificatore #circ NON TROVATO\n");
        exit(1);
    }
}

int main() {
    char* initContent = getContent("input/init.txt");   // All'interno della stringa initContent memorizzo il contenuto del File "init.txt" 
    int nQubits = getNqubit(initContent);   // Utilizzo il metodo getNqubit per memorizzare dentro il valore intero 
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