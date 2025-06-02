#ifndef STRING_MANIPOLATION_H
#define STRING_MANIPOLATION_H

// Il metodo rimuoviCarattere() toglie dalla stringa in input "str" tutti i caratteri uguali al parametro in input "carattere"
void rimuoviCarattere(char* str, char carattere);

// Il metodo getSubstring() preso in input la stringa da cui estrarre un segmento, inserisce nel parametro in input "sottostringa" tutti i caratteri compresi tra l'indice di inizio "start"
// e l'indice di fine "end" della stringa di partenza
void getSubstring(char* sottostringa, char* stringa, int start, int end);



#endif // STRING_MANIPOLATION_H