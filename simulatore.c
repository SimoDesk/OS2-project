#include <stdio.h>
#include <stdlib.h>

void readFile(char path[]) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror("Error opening file");
        return;
    }

    char *riga = NULL;   // puntatore che conterrà la riga
    size_t len = 0;      // dimensione allocata (verrà gestita da getline)

    while ((getline(&riga, &len, f)) != -1) {
        if(riga[0] == '#') {
            printf("%s", riga);
        }
    }
    free(riga);

    fclose(f);
}

int main() {
    readFile("input/init-ex.txt");
    readFile("input/circ-ex.txt");

    return 0;
}



