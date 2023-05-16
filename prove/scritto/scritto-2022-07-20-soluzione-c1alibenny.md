[🔗link testo d'esame](https://csunibo.github.io/sistemi-operativi/prove/scritto/scritto-2022-07-20-testo.pdf)

# Esercizio c1
In un porto con una sola banchina utilizzabile occorre caricare cereali sulle navi. I camion portano i cereali al porto. Una sola nave alla volta può essere attraccata al molo, un solo camion alla volta scarica i cereali nella nave.

Il codice eseguito da ogni nave è:

	nave[i] process:
		porto.attracca(capacità)
		porto.salpa()
		... naviga verso la destinazione

Il codice di ogni camion è:

	camion[j] process:
		while (1):
			quantità = carica_cereali()
			porto.scarica(quantità)

- I camion fanno la spola dai depositi alla nave.
- La nave arriva vuota e può salpare solo se è stata completamente riempita [la somma delle quantità scaricate dai camion raggiunge la capacità indicata come parametro della funzione attracca].
- Se un camion può scaricare solo parzialmente il suo carico rimane in porto e aspetta di completare l'operazione con la prossima nave che attraccherà al molo.

Scrivere il monitor porto.

```c
monitor porto:
	navi = 0			// quante navi sono al porto
	MAX = 0				// capacità max della nave [cereali max]
	cereali_nave = 0	// quanti cereali sono sulla nave
	waiting_cereali = 0 // cereali sul camion che aspetta la prossima nave
	condition camion_ok2scarica, nave_ok2salpa, ok2attracca

	procedure entry attracca(capacita): 
		navi++						// aggiungo la nave al porto
		if (navi > 1)			// Ho delle navi in attesa nel porto che non possono prendere cereali?
			ok2attracca.wait()	// aspettano!	
		MAX = capacita		// salvo quanti cereali servono alla nave
		if (waiting_cereali > 0 && navi > 0) 	// Ho dei cereali in attesa di una nave e ho una nave ?
			cereali_nave += waiting_cereali 	// aggiungo i cereali in attesa alla nave
			waiting_cereali = 0 				// azzero i cereali in attesa perchè sono stati caricati
			camion_ok2scarica.signal()    		// il camion può riprendere a scaricare

	procedure entry salpa(): 		
		if (cereali_nave != MAX) // la nave non è piena
			nave_ok2salpa.wait()			
		navi--  				// tolgo una nave dal porto
		if (navi > 1)				// Ho delle navi che attendono di attraccare?
			ok2attracca.signal() 	// do il segnale che può attraccare
		
	procedure entry scarica(quantita): 	
		cereali_nave += quantita 	// carico i cereali sulla nave
		if (cereali_nave > MAX)
			waiting_cereali += cereali_nave-MAX 	// salvo i cereali che non sono stati scaricati
			cereali_nave = MAX  		// tolgo i cereali che non ci stanno dalla nave
			nave_ok2salpa.signal() 		// la nave può partire
			camion_ok2scarica.wait() 	// il camion deve attendere la prossima nave
		camion_ok2scarica.signal() 	// il camion è vuoto e può andare e lasciare il posto ad altri
```