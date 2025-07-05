#include <string.h>

/*  Il metodo rimuoviCarattere() toglie dalla stringa in input "str" tutti i caratteri uguali al parametro in input "carattere" */
void rimuoviCarattere(char* str, const char* caratteri) {
    int i = 0; /*  Inizializzo un contatore che corrisponderà alla lunghezza della stringa */
    int j = 0;  /*  Inizializzo un secondo contatore che servirà a costruire la nuova stringa senza i caratteri da rimuovere */
    int lenStr = strlen(str);   /*  Calcolo la lunghezza della stringa di partenza */
    for(i = 0; i < lenStr; i++) {
        /*  Se il carattere corrente NON è tra quelli da rimuovere, lo mantieni */
        if(strchr(caratteri, str[i]) == NULL) {
            str[j++] = str[i];  /*  Copio il carattere corrente nella nuova stringa */
        }
    }
    str[j] = '\0'; /*  Chiudi correttamente la nuova stringa */
}


/*  Il metodo getSubstring() preso in input la stringa da cui estrarre un segmento, inserisce nel parametro in input "sottostringa" tutti i caratteri compresi tra l'indice di inizio "start" */
/*  e l'indice di fine "end" della stringa di partenza */
void getSubstring(char* sottostringa, char* stringa, int start, int end) {
    int j = 0;  /*  Inizializzo un contatore che corrisponderà alla lunghezza della sottostringa */
    int i;
    for(i = start; i < end && stringa[i] != '\0'; i++) {
        sottostringa[j] = stringa[i];   /*  Per ogni carattere del segmento evidentziato dai parametri in input, ne creo una copia dalla stringa di partenza alla sottostringa */
        j++;    /*  incremento il contatore */
    }
    sottostringa[j] = '\0'; /*  Aggiungo in fondo alla sottostringa il carattero di chiusura '\0' per renderla una stringa valida */
}