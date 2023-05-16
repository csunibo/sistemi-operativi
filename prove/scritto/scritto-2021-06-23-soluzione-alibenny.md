[🔗link testo esame](https://csunibo.github.io/sistemi-operativi/prove/scritto/scritto-2021-06-23-testo.pdf)

- [Esercizio c1](#esercizio-c1)
- [Esercizio c2](#esercizio-c2)
- [Esercizio g1](#esercizio-g1)
- [Esercizio g2](#esercizio-g2)
		- [Risposte](#risposte)

# Esercizio c1
Scrivere il monitori `delayvalue` con una sola procedure entry:

	int delay(int value);
- Il monitor deve sospendere NDELAY processi che chiamano la delay.
- Le successive chiamate di `delay` devono mantenere costante il numero di processi sospesi, ogni successiva chiamata devo riattivare il primo processo in attesa prima di sospendersi, la `delay` ritorna il valore passato come parametro dal processo che ne ha riattivato l'esecuzione.

- Se `NDELAY = 2`:
```
	P1: delay(44) -> P1 si sospende
	P2: delay(40) -> P2 si sospende
	P3: delay(42) -> P1 si riattiva e ritorna 42, P3 si sospende
	P4: delay(22) -> P2 si riattiva e ritorna 22, P4 si sospende
```

```c
#define NDELAY 2

monitor delayvalue{
	int waiting = 0; 	// quanti processi stanno aspettando
	condition process;
	
	procedure entry delay(int value) {
		waiting++;
		if (waiting >= NDELAY) {
			process.signal();
			return value;
		}
		process.wait();
		waiting--;
	}
}
```

# Esercizio c2
Implementare usando semafori ordinari (fair, fifo) un servizio di semafori a priorità lifo che
fornisca le seguenti primitive:

	void PLP(int prio);
	void PLV()
PLP e PLV si devono comportare rispettivamente come P e V. Quando la PLV deve riattivare un processo
sceglie fra quelli in attesa quello a priorità massima, nel caso siano presenti più processi a priorità massima
sceglie quello in attesa da **meno** tempo. 


```c


void PLP(int prio){

}
void PLV(){

}
```



---
# Esercizio g1
Lo scheduler ED è a priorità dinamica, preemptive e gestisce processi periodici (processi che
si riattivano periodicamente, allo scadere di un periodo se ne attiva una nuova istanza). Lo scheduler sceglie
il processo ready che ha la fine del proprio periodo più prossima. Sono processi che fanno solo calcolo. Ai
fini di questo esercizio il costo dei context switch sia considerato nullo.
Si considerino due sistemi. 

Sistema A: sono presenti 3 processi:
```
Processo 	Periodo 	tempo di elaborazione
p1 			15			 5
p2 			20			 5
p3 			30			 6
```
Sistema B: sono presenti 3 processi:

```
Processo 	Periodo 	tempo di elaborazione
p1 			8 			 2
p2 			16			 4
p3 			32			 16
```
I processi del sistema A sono schedulabili con ED? (si può costruire uno schedule di durata indefinita che
consenta di eseguire tutti i processi). E i processi del sistema B? 


# Esercizio g2
rispondere alle seguenti domande (motivando opportunamente le risposte:
- a) perché DMA viene utilizzato per le unità di memoria secondaria (es dischi) e non per terminali?
- b) cosa succede in un sistema operativo quando un processo utente tenta di eseguire una operazione illegale (es. divisione per zero)? Lo standard POSIX (UNIX) cosa prevede in questo caso?
- c) L'algoritmo di rimpiazzamento second chance (detto anche dell'orologio) è a stack? Perché viene preferito a LRU?
- d) Quali sono le caratteristiche negative dell'uso delle Access Control List per la memorizzazione delle
informazioni di autorizzazione

### Risposte
- a.
- b.
- c.
- d.
