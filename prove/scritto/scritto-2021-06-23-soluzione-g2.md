a) perché DMA viene utilizzato per le unità di memoria secondaria (es dischi) e non per terminali?
DMA viene utilizzato per memoria secondaria perchè si ha necessità di trasferire interi blocchi. Mentre

b) cosa succede in un sistema operativo quando un processo utente tenta di eseguire una operazione
illegale (es. divisione per zero)? Lo standard POSIX (UNIX) cosa prevede in questo caso?

Viene generata una trap. La trap viene intercettata dal sistema operativo che sceglie come gestirla (solitamente viene ucciso il processo). Il processo potrebbe essere equipaggiato di un exception handler, in tal caso l'esecuzione potrebbe essere trasferita ad esso (tale handler sarebbe all'interno del PCB)


c) L'algoritmo di rimpiazzamento second chance (detto anche dell'orologio) è a stack? Perché viene
preferito a LRU?

Non è a stack, potrebbe soffrire dell'anomalia di Belady.




d) Quali sono le caratteristiche negative dell'uso delle Access Control List per la memorizzazione delle
informazioni di autorizzazione.
