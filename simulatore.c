#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct {
    double r;
    double i;
} comp;

typedef struct {
    comp** m;
    char id;
} cmatrix;

cmatrix creaMatrice(int dimensione , char id) {
    cmatrix m;
    m.id = id;
    m.m = malloc(dimensione * sizeof(comp*));
    for(int i = 0; i < dimensione; i++) {
        m.m[i] = malloc(dimensione * sizeof(comp));
    }

    return m;
}


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

comp parseComplesso(char* num) {
    comp n;

    char operazione = ' ';
    char immaginary;

    int res = sscanf(num, "%lf%c%c%lf", &n.r, &operazione, &immaginary, &n.i);

    if(res == 3) {
        sscanf(num, "%lf%c%c", &n.r, &operazione, &immaginary);
        n.i = 1;
    }
    if(res == 1) n.i = 0;
    if(res == 0) {
        n.r = 0;
        if(sscanf(num, "%c%c%lf", &operazione, &immaginary, &n.i) == 3) {
        } else {
            if(sscanf(num, "%c%lf", &immaginary, &n.i) == 1) {
                n.i = 1;
            }
            if(sscanf(num, "%c%c", &operazione, &immaginary) >= 1) {
                n.i = 1;
            }
        }
    }
    if(operazione == '-') n.i = -n.i;

    return n;
}

void rimuoviCarattere(char* str, char carattere) {
    int j = 0;
    for(int i = 0; i < strlen(str); i++) {
        if(str[i] != carattere) {
            str[j] = str[i];
            j++;
        } 
    }
    str[j] = '\0';
}

void getSubstring(char* sottostringa, char* stringa, int start, int end) {
    int j = 0;
    for(int i = start+1; i < end; i++) {
        sottostringa[j] = stringa[i];
        j++;
    }
    sottostringa[j] = '\0';
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
            char temp[strlen(riga)-6];
            getSubstring(temp, riga, 7, strlen(riga)+1);
            rimuoviCarattere(temp, ' ');

            out = atoi(temp);
        }
        riga = strtok(NULL, "\n");  // prossima riga
    }
    free(copiaContent);

    return out;
}

void* getInit(char* content, comp* init) {

    char* copiaContent = malloc(strlen(content)+1);
    strcpy(copiaContent, content);

    char* riga = strtok(copiaContent, "\n");

    while (riga != NULL) {
        if(strncmp(riga, "#init", 5) == 0) {

            int start = (int)(strchr(riga, '[') - riga);
            int end = (int)(strchr(riga, ']') - riga);
            
            char init_s[end-start];
            getSubstring(init_s, riga, start, end);

            rimuoviCarattere(init_s, ' ');
            
            char* num = strtok(init_s, ",");
            
            int i = 0;
            while(num != NULL) {
                init[i] = parseComplesso(num);

                i++;
                num = strtok(NULL, ",");
            }
        }
        riga = strtok(NULL, "\n");  // prossima riga       
    }    
    free(copiaContent);
}

void getMatrix(char* content, char id, int dimensione) {
    char* copiaContent = malloc(strlen(content)+1);
    strcpy(copiaContent, content);

    char* riga = strtok(copiaContent, "\n");

    while (riga != NULL) {
        if(strncmp(riga, "#define", 7) == 0) {
            char temp[strlen(riga)-6];
            getSubstring(temp, riga, 7, strlen(riga)+1);
            rimuoviCarattere(temp, ' ');

            if(temp[0] == id) {
                int start = (int)(strchr(riga, '[') - riga);
                int end = (int)(strchr(riga, ']') - riga)-1;
                
                char matrixContent[end-start];
                getSubstring(matrixContent, riga, start, end);

                rimuoviCarattere(matrixContent, '(');
                rimuoviCarattere(matrixContent, ' ');

                int i, j = 0;

                char* pt1, pt2;
                char* vettoreComplesso = strtok_r(matrixContent, ")", &pt1);
                while(vettoreComplesso != NULL) {
                    char* numComplesso = strtok_r(vettoreComplesso, ",", &pt2);
                    while(numComplesso != NULL) {

                        printf("%s\n", numComplesso);

                        numComplesso = strtok_r(NULL, ",", &pt2);
                    }
                                       
                    vettoreComplesso = strtok_r(NULL, ")", &pt1);
                }
            }
        }
        riga = strtok(NULL, "\n");  // prossima riga
    }
    free(copiaContent);
}

int main() {
    char* initContent = getContent("input/init-ex.txt");
    int nQubits = getNqubit(initContent);
    int dim = 1 << nQubits;

    comp init[dim];
    getInit(initContent, init);

    free(initContent);
    
    printf("dimensione vettore: %i. Qubits = %i\n", dim, nQubits);
    for(int i = 0; i < dim; i++) {
        printf("complesso %i: %lf i%lf\n", i, init[i].r, init[i].i);
    }

    char* circContent = getContent("input/circ-ex.txt");

    // creare vettore di cmatrix di dimensinoe nCirc(da calcolare) e modificare getMatrix che deve passare in input la struct cmatrix di turno
    getMatrix(circContent, 'I', dim);

    return 0;
}