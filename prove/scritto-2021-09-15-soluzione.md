# Prova scritta di sistemi operativi del 15 settembre 2021

## c.1

Per semplicità, supponiamo che vengano richieste sincronizzazioni di al più
`MAX` processi, dove `MAX` è un intero positivo.

```
monitor alrv {

  condition okc[1 : MAX + 1]; // okc[i]: rendez-vous con almeno #i processi
  int alc[1 : MAX]; // alc[i]: w[i]

  p.e. at_least(int n) {
    int s = 0, // sommatoria dei primi #i elementi di alc
        max = 0; // max indice "i" di alc per cui s_i >= i
    ++alc[n]; // conteggia l'at_least attuale
    for (int i = 1; i <= MAX; ++i) {
      s += alc[i];
      if (s >= i)
        max = i;
    }
    if (!max)
      okc[n].wait(); // non abbiamo combinato un rendez-vous (sicuramente n > 1)
    else
      for (int i = 1; i <= max; ++i) // rendez-vous di max processi
        okc[i].signal();
    okc[n].signal();
    --alc[n]; // non conteggiare più l'at_least attuale
  }

}
```

## c.2

```
void chained_send(T msg, list_of_pids dests) {
  if (dests != NULL)
    ssend(Pair<T, list_of_pids>(msg, dests.next), dests.value);
}

T chained_recv(void) {
  Pair<T, list_of_pids> res = srecv(ANY);
  chained_send(res.first, res.second);
  return res.first;
}
```

## g.1

1. 1,2,3,4,5,1,2,3,6,1,2,3,7,8,6 genera 12 _page fault_ con una memoria di 5
   _frame_, ma solo 11 con una memoria di 4.
2. Sia n un numero naturale strettamente maggiore di 3. Scegliamo la sequenza
   $k$ in questione con indici successivi da 1 a $2 n + 4$ inclusi. Sia $i$ un
   indice valido per la sequenza. Allora:
   $$k_i = \begin{cases} i, & \text{se} 1 \leq i \leq n \\ 1, & \text{se} i = n + 1 \\ 2, & \text{se} i = n + 2 \\ n + 1, & \text{se} i = n + 3 \\ i - n - 3, & \text{se} n + 4 \leq i \leq 2 * n + 4 \end{cases}$$
