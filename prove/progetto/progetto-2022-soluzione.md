# Progetti 2022 (PandOS+)

Le domande effettuate durante la discussione coprono solo la fase 3 e
influenzano solo il suo voto. Si discute con il codice (e i suoi commenti)
davanti. Oltre a una prima domande generica, a ogni persona ne viene fatta una
seconda, più specifica, in base alle peculiarità del sorgente presentato, e che
quindi **cambia da gruppo a gruppo**.

## Gruppo 10

Il sorgente consegnato è disponibile su questa [repository](https://github.com/lucat1/unibo_08574_progetto).
Il gruppo partiva da un voto alto: forse le domande sono state tarate tenenedo
conto di questo.

Primo candidato:

- in cosa consiste la vostra implementatione del _TLB exception handler_?
- in questo particolare punto del codice, non avete fatto un controllo un po'
  paranoico?

Secondo candidato:

- in cosa consiste la vostra ottimizzazione che fa uso di `TLBP` e `TLBWI`
  invece di `TLBCLR`?
- come mai avete scelto di usare molti `inline` nel vostro progetto?

Terzo candidato:

- in che cosa consistono le vostre lettura e scrittura da e su dispositivi
  _flash_?
- è davvero necessario disabilitare tutti gli _interrupt_ durante queste
  operazioni?

Quarto candidato:

- in che cosa consiste la vostra implementazione del _TLB-Refill event handler_?
- come funziona il vostro elaborato sistema di _logging_?
