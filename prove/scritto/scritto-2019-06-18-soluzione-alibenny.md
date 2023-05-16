/* link testo http://www.cs.unibo.it/~renzo/so/compiti/2019.06.18.tot.pdf */

# Esercizio c1
```py
class Libro {
	T dato2send	
	waitingLettori = 0
	waitingScrittori = 0
	condition ok2lettore, ok2scrittore

	procedure entry put(T dato)->void {
		if waitingScrittori == 0
			dato2send.first() = dato 	# salvo il dato da inviare del primo scrittore
		else 
			dato2send.next() = dato 	# aggiungo in coda il dato
		if waitingLettori == 0 		# non si sono lettori in attesa
			waitingScrittori++
			ok2scrittore.wait()
			waitingScrittori--
		while (waitingScrittori > 0)
			ok2lettore.signal()
	}
	procedure entry get(void)->T {
		if waitingScrittori == 0{
			waitingLettori++
			ok2lettore.wait()	
			waitingLettori--
		}	
		dato = dato2send.first()
		dato2send.remove(dato)		# tolgo il primo e aggiorno la lista
		ok2scrittore.signal()
		return dato
	}
}
```
Non si riesce ad avere un'unica condizione in quanto le due procedure dipendono l'una dall'altra.

# Esercizio c2
```c
class semaphore {
	int value;
	int blocked = 0;
	binary_semaphore mutex(1);
	binary_semaphore sem(0);
	
	void init(int initval) {
		value = initval;
	}
	void P() {
		mutex.P();
		if (value == 0) {
			blocked++;
			mutex.V();
			sem.P();
			blocked--;
		} 
		value--;
		mutex.V();
	} 
	void V() {
		mutex.P();
		value++;
		if (blocked > 0)
			sem.V();
		else
			mutex.V();
	}
} // class semaphore
```
In questa implementazione di semafori generali dati semafori ordinari:

a) viene usata la tecnica di passaggio del testimone (passing le batôn), spiegare come è usata e quale effetto ha.
![slides concorrenza](../../docs/img/passaggio_testimone.png)
>
b) questa implementazione non garantisce la FIFONESS di riattivazione dei processi bloccati, mostrare perché.
>
c) modificare il codice, mantenendone la struttura, per ottenere una soluzione che risolva il problema del punto 2.
>