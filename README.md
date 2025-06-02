# OS2 Project – Simulatore di Circuiti Quantistici

> GitHub: [SimoDesk/OS2-project](https://github.com/SimoDesk/OS2-project.git)

## Manuale Utente

### Input richiesto

Inserire nella cartella `input` due file con i seguenti nomi e formati:

---

#### `init.txt`

Contenuto richiesto:

- Una riga che inizia con `#qubits`, seguita da un numero intero che rappresenta la quantità di qubit del simulatore.  
  Il simulatore selezionerà **solo la prima riga** che inizia con questo identificatore, ignorando le successive.

- Una riga che inizia con `#init`, seguita da un vettore di numeri complessi.  
  Il vettore deve:
  - Essere racchiuso tra parentesi quadre `[ ]`
  - Contenere numeri complessi separati da virgole
  - Essere ben formattato (vedi sezione "Formattazione numeri complessi")

---

#### `circ.txt`

Contenuto richiesto:

- Una o più righe che iniziano con `#define K`, dove `K` è un identificatore (una singola lettera) associato a una matrice di numeri complessi.  
  La matrice deve:
  - Essere racchiusa tra parentesi quadre `[ ]`
  - Contenere righe (vettori) racchiuse tra parentesi tonde `( )`
  - Avere numeri complessi formattati correttamente, separati da virgole

- Una riga che inizia con `#circ`, seguita da una sequenza di lettere che identificano le matrici da applicare nel circuito.  
  Anche in questo caso, verrà usata **solo la prima riga** con questo identificatore.

---

### Esecuzione

Per eseguire il progetto è sufficiente aprire il terminale nella cartella dov'è presente il Makefile e lanciare il comando:

```
make
```

---

### Formattazione dei Numeri Complessi

È accettato qualunque formato matematicamente corretto, purché l'unità immaginaria `i` preceda sempre il coefficiente.

| Valido | Non valido |
|--------|------------|
| `-0.5` | `0.5-0.5i` |
| `0.5+i` | `0.5i-1` |
| `0.5-i0.5` | |
| `i0.5+1` | |
| `i-0.5` | |

---

## Struttura dei File

| File | Descrizione |
|------|-------------|
| `simulatore.c` | File `main`, contiene la logica principale del simulatore |
| `dataGetter.c` | Funzioni per l’estrazione dei dati da `init.txt` e `circ.txt` |
| `fileManagement.c` | Funzione per l’apertura dei file e il recupero del contenuto come stringa |
| `operations.c` | Operazioni matematiche tra numeri complessi e matrici |
| `stringManipolation.c` | Funzioni per manipolazione di stringhe (es. rimozione caratteri, estrazione sottostringhe) |
| `types.c` | Definizione delle `struct` dati e funzioni di utilità per parsing, copia, ecc. |
| `util.c` | Funzioni di stampa delle `struct` e gestione degli errori |
