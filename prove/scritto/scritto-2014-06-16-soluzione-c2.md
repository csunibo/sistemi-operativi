


## Esercizio c.2

```python
semaforo processi[NPROC]   #NPROC è il numero massimo di processi
Queue messaggi<msg> [NPROC];

init():
    for i in NPROC:
        processi[i] = new Semaforo(0)
        messaggi[i] = new Queue()

void asend(msg m, pid_t dst):
    messaggi[dest].enqueue( (m, getpid()) )
    processi[dest].V();

msg arecv(pid_t sender):
    while True:
        for x in messaggi[getpid()]:
            if x[1] = sender:
                messaggi[getpid()].remove(x)
                return x[0]

        processi[getpid()].P()
```
