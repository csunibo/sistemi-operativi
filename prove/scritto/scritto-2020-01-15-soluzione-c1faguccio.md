# Prova 2020-01-15

## Esercizio c.1

```python
monitor semtimout:
    int value = 0
    Queue proc <conditions, int> = new Queue()

    void V(void):
        if (proc.isEmpty()):
            value++
        else:
            x = proc.dequeue()
            x.signal()
    
    boolean P(int timeout):
        if (value == 0):
            proc.enqueue(new condition, timeout)
            proc.last[condition].wait()
        else:
            value--

    void tick(void):
        for x in proc:
            x.time--
            if x.time <= 0:
                x.condition.signal()

```


## Esercizio g.2

a) Esempio con file Video e processo della posta in Background. Il video non può aspettare i processi in background (tipo quello della posta) mentre è vero il viceversa. Qui ci vorrebbe uno scheduler con priorità dove i processi multimediali hanno priotià più alto.

b) 
