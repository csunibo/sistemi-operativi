#include <stdio.h>

void carica(int *n){
	scanf("%d", n);
}

void init(int *inpadd){
	carica(inpadd);
	printf("input value: %d\n", inpadd);
}

int fattoriale(int n){
	int res= 1;
	for (int i=n; i>= 0; i--){ //mettendo così non viene interpretata come una singola linea di comando quindi possiamo
		res*=i;
	}
	return res;
}

void stampa(int n, int risultato){
	printf("il risultato di \"input\"(%d) vale (%d)", n, risultato);
}

int main(){
	int input;
	init(&input);
	stampa(input, fattoriale(input));
}
//step in(step), step over(next), step out(finish)
//RICORDA CHE LE VARIABILI NON SONO INIZIALIZZATE A 0!!!
//http://www.unknownroad.com/rtfm/gdbtut/gdbstep.html
//It may help to turn off all optimization (-O0) when compiling
//NOTE: If you have a larger program with several files, each must be compiled with the -g flag
//riguardo le righe di codice numerate, riferirsi a bugOrig.c
//(se davanti non c'è (gdb) => la shell sta aspettando un input)
//gcc -g DebugOrig.c -o output
//-g => tutte le informazioni sui vari nomi di funzioni e variabili vengano mantenute. Inoltre stoppiamo il compilatore a fare un'"ottimizzazione"
//del codice ovvero andare a cancellare parti del codice che secondo lui non servono
//gdb -ex 'command1' -ex 'command2' fileDaCheckare
//flag -ex per eseguire comandi in automatico (dopo puoi continuare ad analizzarlo dal quel punto).
//esempio: gdb -ex "break main" -ex "run" -ex "print args" output

/*
5
input value: -1278598588
il risultato di "input"(5) vale (0)
=> gdb output
COMANDI
-1)gdb —args <file>  <arg1> <arg2> .. <argn>
or inside gdb :

    set args <arg1> <arg2> ...
    run
0)quit per uscire
1)shell comandodaterminale (per eseguire comandi da terminale su gdb)(non posso direttamente scrivere clear!)

2)run (manda semplicemente il programma in esecuzione (come se facessimo ./output)) oppure possiamo far ripartire il programma
mentre lo stiamo analizzando
2b) start => identico a run, solo che ti crea da solo un tbreak alla prima elaborazione del corpo mani() (anche su i=0;)

3)break (associato ai break point, ovvero pause, ovvero punti che noi andremo a fissare nel nostro programma e che diranno al computer
di fermare l'esecuzione di quel programma non appena avrà raggiunto quel punto o appena entra in quella funzione) => break nomeFunzioneOnumeroLinea
Se non c'è nessuno elaborazione di dati (es dichiarazione di variabile input come in riga 22) => passa direttamente alla prossima
Se facciamo ripartire il programma con run (o r), => (gdb) r => il programma si nella riga associata al breakpoint (23)(init(&input))
In questo momento possiamo visualizzare tutti i valori correnti nella vriabili, i vari indirizzi di memoria, etc.
Per proseguire anzichè ricominciare da zero, => continue (si fermerà al prossimo breakpoint)
to stop execution = ctrl c
Use the continue command to restart execution of your program whenever it is stopped.

3b)How do I set a temporary breakpoint?
Use the tbreak command instead of break. A temporary breakpoint only stops the program once, and is then removed.


4)clear nomeDellaFunzioneOppureNumeroRigaEffettivaSullaQualec'èilbreakpoint (quindi clear main o clear 23 (se prima avevamo messo break 22)
(per togliere breakpoint per futuri run!)

5)delete IDBreakpoint (il contatore degli ID si basa anche se i precedenti sono stati eliminati)
delete (senza nessun argomento)=> ti chiede se vuoi eliminare tutti i breakpoints

6) info breakpoint => vedi tutti i breakpoint attivi
6b)How do I skip breakpoints? To skip a breakpoint a certain number of times, we use the ignore command.
The ignore command takes two arguments: the breakpoint number to skip, and the number of times to skip it.
(gdb) ignore 2 5 //Will ignore next 5 crossings of breakpoint 2.



7) next =>
First stop your program by sending it signals or using breakpoints. Then use the next and step commands.
*NOTE* the next and step commands are different. On a line of code that has a function call, next will go 'over' the function call to the
next line of code, while step will go 'into' the function call.


andiamo avanti di solo una linea (dopo che si siamo fermati ad un break point durante l'esecuzione). e si fermerà alla riga successiva
(esegue la riga corrente e procede alla riga successiva (NON ENTRA ESPLICITAMENTE NELLA FUNZIONE ANALIZZATA COME STEP la quale "salta")
(next salta dal corpo della funzione ausiliaria al corpo della funzione principale solo quando l'esecuzione della funzione ausiliaria ha terminato))
8) step => se anzichè passare alla riga successiva volessimo entrare e vedere cosa succede nella funzione della riga corrente
pronta per essere elaborata
(quando si arriva con funzioni come scanf o printf (ovvero funzioni non tue) E' SCONSIGLIATO FARE STEP poichè ti addentri nelle varie syscall
e loro personali funzioni! => in quei casi fai next)

23              init(&input);
(gdb) step
init (inpadd=0x5555555552b0 <__libc_csu_init>) at DebugOrig.c:7
7       void init(int *inpadd){

(a sx c'è scritto il numero della riga attualmente analizzata)

9) finish => se avessimo una funzione molto lunga e volessimo continuare ad eseguire tutte le istruzioni (solo di quella determinata funzione)
per poi fermarci una volta usciti da questa => o schiacciamo tante volte n(ext) oppure finish (23 - step - 7 - finish - 24)


10) (per quando ci si perde nel codice)
list => ci mostra il codice (o meglio il contesto) nel quale ci siamo fermati (riga in mezzo tra quelle mostrate è quella sulla quale siamo fermi)
10b)l # - lists 10 lines of source code surrounding a specific line (#)

11) frame => è più specifico di list, infatti ci dice che funzione stiamo eseguendo e a quale linea di codice ci siamo fermati(quindi prossima
ad essere eseguita (ma non ancora eseguita!))
11b) where => ti dice in quale funzione nello stack sei

How do I change stack frames?
Notice in the backtrace above that each frame has a number beside it. Pass the number of the frame you want as an argument to the command.

How do I examine stack frames?
To look at the contents of the current frame, there are 3 useful gdb commands:
-info frame displays information about the current stack frame.
-info locals displays the list of local variables and their values for the current stack frame.
-info args displays the list of arguments.

12)(quali funzioni hanno chiamato quali altre funzioni per arrivare al punto dove siamo => scorrere la lista delle chiamate a funzioni che ci
hanno portato fino a questo punto) => up (previous) e down

13) backtrace => per stampare l'intera pila delle chiamate a funzioni (da linea corrente a main)(rimanendo sulla linea corrente a diff di up/down)

14) print (delle variabili) => print nomeVariabile
(per vedere se le varie funzioni mettono correttamente i valori nel loro posto es init())

15) whatis => oltre a poter stampare il valore della variabile, possiamo stamparne anche il tipo (puntatore? intero? double? etc)
=> whatis NomeVariabile

16a) (se ci troviamo in un ciclo, potremmo voler vedere come cambiano le variabili durante il ciclo) display
=> break fattoriale - display res - n (break conditionale (useful for breaking in loops) break lineNUmber if var [><=]= value)
(con watch puoi inserire subito la condizione/espressione eg watch i>10)
ad ogni nuova linea di codice o istruzione che diamo a gdb, printerà il valore di res (digitare ogni volta n OPPURE guarda giù
(gdb) break <line number just after start of the loop>

16b) (gdb) commands (IDBreakpoint)
(You can give any breakpoint (or watchpoint or catchpoint) a series of commands to execute when your program stops due to that breakpoint.
For example, you might want to print the values of certain expressions, or enable other breakpoints.)
(To remove all commands from a breakpoint, type commands ID and follow it immediately with end; that is, give no commands. )
With no argument, commands refers to the last breakpoint, watchpoint, or catchpoint set (not to the breakpoint most recently encountered).

Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
>silent (If the first command you specify in a command list is silent, the usual message about stopping at a breakpoint is not printed)
>printf "res == %d, i = %d\n", res, i (The commands echo, output, and printf allow you to print precisely controlled output)
>continue
>end
)
printo tutti quei valori che influenzano res ;)
aggiungo e faccio un break su linea aggiunta res *=1 per vedere alla fine del ciclo che valore res ha ottenuto

16c)per non visualizzare più il valore della variabile => undisplay (se lanciato senza ID ti chiede di eliminare tutti i display (come delete))

17) watch nomeVar => watch fermerà l'esecuzione del programma ogniqualvolta vedrà cambiare il valore della variabile
(ricordati di inizializzare sempre le variabili!!!!!!!!! altrimenti il watchpoint te lo farà notare! (da random value to quello che hai inserito)
GDB automatically deletes watchpoints that watch local (automatic) variables, or expressions that involve such variables, when they go out of
scope, that is, when the execution leaves the block in which these variables were defined.
HOWEVER, the watch command accepts a -location argument: Ordinarily a watchpoint respects the scope of variables in expr.
The -location argument tells GDB to instead watch the memory referred to by expr. In this case, GDB will evaluate expr, take the address of
the result, and watch the memory at that address. The type of the result is used to determine the size of the watched memory.
(chiedere di eval al prof e di come monitora tra vari scope una stessa variabile) => watch -l varname
per togliere il watch, poichè un watch è considerato come un breakpoint, si utilizza delete IDbreakpoint (info breakpoint o "i b")
the variable you want to set a watchpoint on must be in the current scope
17a) sopra abbiamo definitivo un write watchpoint, ovvero veniamo notificati appena la variabile viene sovrascritta
read watchpoint => rwatch, rw watchpoint => awatch (stesso uso di sopra)

18)set variabile varName = valore (per cambiare valore alla variabile varName in runtime!)

6       {
7           int i;
8           struct file *f, *ftmp;
9
(gdb) set variable i = 10
(gdb) p i
$1 = 10

(gdb) p &i
$2 = (int *) 0xbfbb0000 // se una variabile contiene un indirizzo di memoria e faccio print di essa => uscirà esplicitamente
che è un pointer di un certo tipo, ex: (int *) 0xbfbb0000 (come whatis ma quest'ultimo non stampa il suo valore(ex type = int *))
(gdb) set *((int *) 0xbfbb0000) = 20 //pointer di tipo int dereferenziato
(gdb) p i
$3 = 20

"syntax without 'variable'is not recommended because the variable name may clash with a set sub-command.
As an example set w=1 would not be valid.
This means that you should prefer the syntax: set variable idx =  1 or set var idx = 1."


19) until main.c:60 (https://bbs.archlinux.org/viewtopic.php?id=107297)
Continue running your program until either the specified location is reached, or the current stack frame returns.
The specified location is actually reached only if it is in the current frame.
This implies that until can be used to skip over recursive function invocations.

20)How do I examine memory?
Use the x command to examine memory.
The syntax for the x command is x/FMT ADDRESS. The FMT field is a count followed by a format letter and a size letter.
There are many options here, use the help command 'help x' to see them all.
The ADDRESS argument can either be a symbol name, such as a variable, or a memory address.
FMT = stesso per printare testo formattato printf (%d, %s, %c)
If we have char *s = "Hello World\n", some uses of the x command could be:
Examine the variable as a string:
    (gdb) x/s s
    0x8048434 <_IO_stdin_used+4>:    "Hello World\n"
Examine the variable as a character:
    (gdb) x/c s
    0x8048434 <_IO_stdin_used+4>:   72 'H'
Examine the variable as 4 characters:
    (gdb) x/4c s
    0x8048434 <_IO_stdin_used+4>:   72 'H'  101 'e' 108 'l' 108 'l'

21)target record-full
"at this point onwards, record everything that happens: keep track of every change
of every variable, every line we ran, etc.. in order to kind of replay in reverse"

22)reverse-next, reverse-step, reverse-continue=> per ritornare alla riga precedente (con funzion)

(riguardo errori https://stackoverflow.com/questions/3649468/how-to-set-a-breakpoint-in-gdb-where-the-function-returns/3649698#3649698)
DA CHIEDERE AL PROF
1)come monitora una variabile se va fuori dal suo scope? watch -l? incontra anche lei file exit.c? riguardo eval?
2) come gestisce il monitoraggio di una variabile dentro a appena fuori un ciclo for?
3) https://bbs.archlinux.org/viewtopic.php?id=107297 come scorre tra i vari file se sono stati spostati rispetto al momento per compilazione
se cambiano directory a quei file?
4) cosa ne pensa di valgrind
*/
