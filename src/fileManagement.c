#include <stdio.h>
#include <stdlib.h>

#include "../include/util.h"

// Il metodo getContent() restituisce una stringa con il contenuto del file passato in input attraverso il suo path corrispondente
char* getContent(char* path) {
    FILE *f = fopen(path, "r"); // Apro il file in modalità lettura attraverso il path passato in input
    if (f == NULL) {
        perror("Errore apertura file in getContent");   // Se l'apertura del file non è andata a buon fine, interrompo il programma con un errore
        exit(1);
    }

    fseek(f, 0, SEEK_END);          // Sposto il puntatore del file alla fine
    long dimensioneFile = ftell(f); // così da poter memorizzare la dimensione totale in byte
    rewind(f);                      // infine, riporto nuovamente il puntatore all'inizio del file

    char* out = malloc(dimensioneFile + 1); // Dedico una porzione di memoria al contenuto del file 
    if (out == NULL) error("Errore allocazione memoria in getContent"); // Se dovessero esserci problemi con l'allocazione della memoria, un errore fermerebbe l'esecuzione
    
    if(dimensioneFile != fread(out, 1, dimensioneFile, f)) {    // Con li metodo fread() copio nella stringa "out" elementi di dimensione 1 byte dal file "f" 
                                                                // per un totale di byte corrispondente al valore di "dimensioneFile".

        free(out);                                      // Se il metodo non dovesse restituire la quantità di elementi letti corretta, 
        fclose(f);                                      // il file verrebbe chiusto, la memoria liberata e infine il programma
        perror("Errore lettura file in getContent");    // verrebbe terminato con un errore
        exit(1);                                        //
    }
    fclose(f);  // Chiudo il file poiché ho finito di utilizzarlo

    return out; // Restiruisco il contenuto del file sotto forma di stringa
}