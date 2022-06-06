
long mystrlen(char *s) {   // lunghezza di una stringa
        long len;
        for (len = 0; *s != 0; len++)
                s++;
        return len;
}

long mywrite(char *s) { //stampa una stringa
        long addr = (long) s; //addr e' l'indirizzo della stringa
        long len = mystrlen(s); // e len e' la lunghezza
        register long r_syscallno asm("rax") = 1; // mette nel registro rax il numero della syscall 1=write
                                                  // l'elenco completo in /usr/include/x86_64-linux-gnu/asm/unistd_64.h
                                                  // mutatis mutandis se usare un'altra architetttura di processore
        register long r_arg1 asm("rdi") = 1;      // da "man 2 write" si vede che il 1' paramentro e' il fd. 1 e' stdout
        register long r_arg2 asm("rsi") = addr;   // secondo parametro l'indirizzo del buffer da mettere in output
        register long r_arg3 asm("rdx") = len;    // terzo parametro e' la lunghezza
        register long r_retvalue asm("rax");      // chiamo r_retvalue il registro rax, dove le syscall mettono il valore di rit.
        asm("syscall");                           // genera la trap per "svegliare" il kernel
        return r_retvalue;                        // restituisco il valore di ritorno
}

void myexit(long value) {                   // termina il chiamante
        register long r_syscallno asm("rax") = 60;  // la ssytem call 60 e' _exit
        register long r_arg1 asm("rdi") = value;   // ed ha un solo parametro: il valore di ritorno
        asm("syscall");
}

void _start(void) {
        long retvalue;
        retvalue = mywrite("hello world\n"); // stampa la famosa stringa
        myexit(retvalue);                    // e resituisci come exit status il valore di ritorno della write
                                             // e' il numero dei caratteri stampati (o il codice di erroe in negativo).
}
