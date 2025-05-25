#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct{
    double r;
    double i;
} comp;

void* error(const char* meggassio, int numAreeOccupate, ...) {
    printf("%s\n", meggassio); // stampa il messaggio d’errore

    va_list args;                       // lista variabile di argomenti
    va_start(args, numAreeOccupate);    //

    for (int i = 0; i < numAreeOccupate; i++) { //  
        void* area = va_arg(args, void*);       // libera ogni puntatore passato
        free(area);                             //
    }                                           //

    va_end(args);

    return NULL; 
}

char* getContent(char path[]) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror("ERRORE aprttura file");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long dimensioneFile = ftell(f);
    rewind(f);

    char* out = malloc(dimensioneFile + 1);
    if (out == NULL) 
        return error("ERRORE di allocazione memoria in GetContent \n", 1, out);
    
    if(dimensioneFile != fread(out, 1, dimensioneFile, f)) {
        printf("ERRORE di lettura del file %s", path);
        free(out);
        fclose(f);
        return NULL;
    }
    fclose(f);

    return out;
}

int getNqubit(char* content) {
    char* copiaContent = malloc(strlen(content)+1);
    strcpy(copiaContent, content);
    
    int out = 0;

    char* riga = strtok(copiaContent, "\n");

    while (riga != NULL) {
        if(strncmp(riga, "#qubits", 7) == 0) {

            int dimensione = 0;
            char* num = malloc(1);
            if (num == NULL) {
                error("ERRORE di riallocazione memoria in getNqubit \n", 1, num);
                        return -2;
            }
            num[0] = '\0';

            for(int i = 0; riga[i] != '\0'; i++) {
                if(isdigit(riga[i])) {
                    char* temp = realloc(num, dimensione+2);
                    if(temp != NULL) {
                        num = temp;
                        num[dimensione] = riga[i];
                        dimensione++;
                        num[dimensione] = '\0';
                    } else {
                        error("ERRORE di riallocazione memoria in getNqubit \n", 1, num);
                        return -2;
                    }
                }
            }

            out = atoi(num);
            free(num);
        }
        riga = strtok(NULL, "\n");  // prossima riga
    }
    free(copiaContent);

    return out;
}

void* getInit(char* content, comp* init) {
    int initIndex = 0;

    char* copiaContent = malloc(strlen(content)+1);
    strcpy(copiaContent, content);

    char* riga = strtok(copiaContent, "\n");

    int dimensione = 0;
    char* num = malloc(1);
    if (num == NULL) 
        return error("ERRORE di allocazione memoria in getInit \n", 2, num, copiaContent);

    num[0] = '\0';

    while (riga != NULL) {
        if(strncmp(riga, "#init", 5) == 0) {

            int stato = 0;
            char l;
            for(int i = 0; riga[i] != '\0'; i++) {
                l = riga[i];

                switch (stato) // macchina a stati finiti per la lettura di init
                {
                    case 0: // nello stato Iniziale      
                        if(l == '[') {  // 
                            stato = 1;  // se incontro "[" allora sono all'inizio del vettore, vai a stato 1(Attesa Valori Reale)
                        }               //          
                        break; 

                    case 1: // nello stato di Attesa Valori Reale
                        if(l == '+' || l == '-') { // se incontro un "+" o un "-" lo aggiungo alla stringa di num e passo allo stato 2(Parte Intera Reale)
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 1)\n", 2, num, copiaContent);

                            stato = 2; // passaggio a stato 2
                            break;
                        }
                        if(isdigit(l)) // se incontro una cifra la aggiungo alla stringa di num e passo allo stato 2(Parte Intera Reale)
                        {
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 1)\n", 2, num, copiaContent);
                            
                            stato = 2; // passaggio a stato 2
                            break;
                        }
                        if(!isdigit(l)) // in questo caso, init non è formattata come richiesto nel README
                            return error("ERRORE di formattazione di Init (stato 1) \n", 2, num, copiaContent);
                        break;

                    case 2: // nello stato di Parte Intera Reale
                        if(isdigit(l)) { // se incontro un numero lo aggiungo alla stringa di num e resto nello stato 2(Parte Intera Reale)
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 2)\n", 2, num, copiaContent);

                            stato = 2; // resto in stato 2
                            break;
                        }
                        if(l == '.') { // se incontro un "." lo aggiungo alla stringa di num e passo allo stato 3(Parte Decimale Reale)
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 2)\n", 2, num, copiaContent);

                            stato = 3; //passaggio in stato 3
                            break;
                        }
                        if(l == '+' || l == '-') { // se incontro un "+" o una "-" significa che la parte reale è finita, perciò passo allo stato 4(Parte Intera Immaginaria)
                            init[initIndex].r = strtod(num, NULL);

                            dimensione = 0;                            
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 3)\n", 2, num, copiaContent);

                            stato = 4; //passaggio in stato 4
                            break;
                        }
                        if(!isdigit(l)) // in questo caso, init non è formattata come richiesto nel README
                            return error("ERRORE di formattazione di Init (stato 2) \n", 2, num, copiaContent);
                        break;

                    case 3: // nello stato di Parte Decimale Reale
                        if(isdigit(l)) { // se incontro un numero lo aggiungo alla stringa di num e resto nello stato 3(Parte Decimale Reale)
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 3)\n", 2, num, copiaContent);

                            stato = 3; // resto in stato 3
                            break;
                        }
                        if(l == '+' || l == '-') { // se incontro un "+" o una "-" significa che la parte reale è finita, perciò passo allo stato 4(Parte Intera Immaginaria)
                            init[initIndex].r = strtod(num, NULL);

                            dimensione = 0;                            
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 3)\n", 2, num, copiaContent);

                            stato = 4; //passaggio in stato 4
                            break;
                        }
                        if(!isdigit(l)) // in questo caso, init non è formattata come richiesto nel README
                            return error("ERRORE di formattazione di Init (stato 3) \n", 2, num, copiaContent);
                        break;
                        
                    case 4: // nello stato di Parte Intera Immaginaria
                        if(isdigit(l)) { // se incontro un numero lo aggiungo alla stringa di num e resto nello stato 4(Parte Intera Immaginaria)
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 4)\n", 2, num, copiaContent);

                            stato = 4; // resto in stato 4
                            break;
                        }
                        if(l == '.') { // se incontro un "." lo aggiungo alla stringa di num e passo allo stato 5(Parte Decimale Immaginaria)
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 4)\n", 2, num, copiaContent);

                            stato = 5; //passaggio in stato 5
                            break;
                        }
                        if(l == ',') {
                            init[initIndex].i = strtod(num, NULL);
                            initIndex++;

                            dimensione = 0;                            
                            char* temp = realloc(num, 1);
                            if(temp != NULL) {
                                num[0] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 5)\n", 2, num, copiaContent);

                            stato = 6;
                            break;
                        }
                        if(l == ']') {
                            init[initIndex].i = strtod(num, NULL);
                            stato = 7; // passaggio allo stato 10(Lettura Completata Con Successo)
                            break;
                        }
                        if(l == 'i') {
                            stato = 4; // resto in stato 4
                            break;
                        }
                        if(!isdigit(l)) // in questo caso, init non è formattata come richiesto nel README
                            return error("ERRORE di formattazione di Init (stato 4) \n", 2, num, copiaContent);
                        break;

                    case 5: // nello stato di Parte Decimale Immaginaria
                        if(isdigit(l)) { // se incontro un numero lo aggiungo alla stringa di num e resto nello stato 5(Parte Decimale Immaginaria)
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 5)\n", 2, num, copiaContent);

                            stato = 5; // resto in stato 5
                            break;
                        }
                        if(l == ']') {
                            init[initIndex].i = strtod(num, NULL);
                            stato = 7; // passaggio allo stato 10(Lettura Completata Con Successo)
                            break;
                        }
                        if(l == ',') { // se  incontro ',' vuol dire che la parte decimale immaginaria è conclusa, perciò passo allo stato 6(Attesa)
                            init[initIndex].i = strtod(num, NULL);
                            initIndex++;

                            dimensione = 0;                            
                            char* temp = realloc(num, 1);
                            if(temp != NULL) {
                                num[0] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 5)\n", 2, num, copiaContent);

                            stato = 6; //passaggio in stato 6
                            break;
                        }
                        if(!isdigit(l)) // in questo caso, init non è formattata come richiesto nel README
                            return error("ERRORE di formattazione di Init (stato 5) \n", 2, num, copiaContent);
                        break;

                    case 6: // nello stato di Attesa
                        if(l == ']') {
                            stato = 7; // passaggio allo stato 10(Lettura Completata Con Successo)
                            break;
                        }
                        if(l == '+' || l == '-') { // se incontro un "+" o un "-" lo aggiungo alla stringa di num e passo allo stato 2(Parte Intera Reale)
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 6)\n", 2, num, copiaContent);

                            stato = 2; // passaggio a stato 2
                            break;
                        }
                        if(!isdigit(l)) { // se non incontro un numero o init non è ancora terminato resto nello stato 6(Attesa)
                            stato = 6; // resto in stato 6
                            break;
                        }
                        if(isdigit(l)) // se incontro una cifra la aggiungo alla stringa di num e passo allo stato 2(Parte Intera Reale)
                        {
                            char* temp = realloc(num, dimensione+2);
                            if(temp != NULL) {
                                num = temp;
                                num[dimensione] = l;
                                dimensione++;
                                num[dimensione] = '\0';
                            } else 
                                return error("ERRORE di riallocazione memoria in getInit (Stato 6)\n", 2, num, copiaContent);
                            
                            stato = 2; // passaggio a stato 2
                            break;
                        }
                        break;
                        
                    default:
                        break;
                }
            }
            if(stato != 7) 
                return error("ERRORE nella lettura di init! \n", 2, num, copiaContent);
                        
            free(num);
            goto out;
        }
        riga = strtok(NULL, "\n");  // prossima riga
        
    }
    out:
    free(copiaContent);
}

int main() {
    char* initPath = getContent("input/init-ex.txt");
    int nQubits = getNqubit(initPath);
    int dim = 1 << nQubits;
 
    comp init[dim];
    getInit(initPath, init);
    printf("numero Qubit: %i\n", nQubits);
    for(int k = 0; k < (sizeof(init)/sizeof(init[0])); k++) {
        printf("init[%i]=%.3f i%.3f\n", k, init[k].r, init[k].i);
    }

    free(initPath);
    return 0;
}



