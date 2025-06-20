#include <string.h>

// Il metodo rimuoviCarattere() toglie dalla stringa in input "str" tutti i caratteri uguali al parametro in input "carattere"
void rimuoviCarattere(char* str, char carattere) {
    int j = 0;  // Inizializzo un contatore che corrisponderà alla nuova lunghezza della stringa contando gli eventuali caratteri rimossi
    int len = strlen(str);
    for(int i = 0; i < len; i++) {
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