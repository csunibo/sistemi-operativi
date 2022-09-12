#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	char *buf; //We're going to check the value of buf before the malloc call. Since buf wasn't initialized, the value should be garbage. Indeed
	//ALWAYS CHECK THE RETURN VALUE OF MALLOC!
	/* buf = malloc(1<<31); //Now step over the malloc call and examine buf again */
	buf = malloc(1024);
	//After the call to malloc, buf is NULL. malloc returns NULL when it cannot allocate the amount of memory requested
	fgets(buf, 1024, stdin); //to, max size readed, from. step appena vedo il comando => _IO_fgets (-> buf=0x5555555592a0 <- "", n=1024..
	//con step accedo alla funzione => vedo con quali parametri è stata chiamata
	printf("%s\n", buf);
	return 1;
}
//https://www.cprogramming.com/debugging/segfaults.html
//https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Debugging.html valgrind
/*
Program received signal SIGSEGV, Segmentation fault.
So we received the SIGSEGV signal from the operating system. This means that we tried to access an invalid memory address => printare indirizzi
di memoria delle variabili utilizzate del comando nel quale siamo sopra
In general, we can assume that library functions such as fgets work properly. So the problem must be one of our arguments.
The value of buf is 0x0, which is the NULL pointer.  buf should point to the memory we allocated on line 9. So we're going to have to find out what happened there.
=> settiamo un breakpoint a quella riga (9)
*/
