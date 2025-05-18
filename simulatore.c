#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

char* subString(char stringa[], int inizio, int lunghezza) {
    if(inizio < 0 || inizio+lunghezza > strlen(stringa)) {
        printf("Errore nell'estrazione della sottostringa");
        return NULL;
    }

    char *sottostringa = (char *)malloc(lunghezza * sizeof(char));
    strncpy(sottostringa, stringa + inizio, lunghezza);
    sottostringa[lunghezza] = '\0'; // terminatore stringa
    
    return sottostringa;
}
    

char getLineType(char riga[]) {
    //printf("%s \n", subString(riga, 0, 7));
    if(strncmp(riga, "#qubits", 7) == 0) {
        return 'q';
    }
    if(strncmp(riga, "#init", 5) == 0) {
        return 'i';
    }
    if(strncmp(riga, "#define", 7) == 0) {
        return 'd';
    }
    if(strncmp(riga, "#circ", 5) == 0) {
        return 'c';
    }
    
    return 'n';    
}

char* readFile(char path[], char tipo) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char *riga = NULL;   // puntatore che conterrà la riga
    size_t len = 0;      // dimensione allocata (verrà gestita da getline)

    while ((getline(&riga, &len, f)) != -1) {
        if(riga[0] == '#') {
            printf("%s", riga);
            if(getLineType(riga) == tipo) {
                return riga;
            }
        }
    }
    free(riga);

    fclose(f);
}

int getNCubits(char path[]) {
    char* riga = readFile(path, 'q');
    int inizio, fine = 0;
    bool first = true;
    for(int i = 0; riga[i] != '\0'; i++) {
        unsigned char uc = (unsigned char)riga[i];
        if(isdigit(uc)) {
            if(first) {
                inizio = i;
                first = false;
            } else {
                fine = i;
            }
        }
    }
    printf("dimensione:%i\n", fine-inizio+1);
    
    char* sottostringa = subString(riga, inizio, fine-inizio+1);
    int res = atoi(sottostringa);
    free(sottostringa);

    free(riga);
    return res;
}

int main() {
    printf("%i \n", getNCubits("input/init-ex.txt"));
    readFile("input/circ-ex.txt", 'd');

    return 0;
}



