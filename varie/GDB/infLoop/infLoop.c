#include <stdio.h> //Infinite Loop Example
#include <ctype.h>

int main(int argc, char **argv)
{
	char c;

	c = fgetc(stdin); //La funzione fgetc in C legge un carattere da un file
	while((c != '\n') && (c != EOF )){ //La funzione fgetc restituisce un intero, che in caso di successo è il carattere letto, altrimenti, se ad esempio
					 //raggiunge la fine del file o si ha un errore di lettura, restituisce la costante EOF
		if(isalnum(c))
			printf("%c", c); //The isalnum() function checks whether the argument passed is an alphanumeric character (alphabet or number) or not.
		/* else */ //da commentare altrimenti non consuma caratteri
		c = fgetc(stdin);
	}

	return 1;
}
/*
   To find the problem, we'll set off the infinite loop, and then press Ctrl-C to send the program a SIGINT.
   Gdb will trap this signal and stop program execution. (Program received signal SIGINT, Interrupt.)
   Now we have to find the loop. We use several iterations of the 'next' command to watch what is happening.
   We run the next command several more times to watch the program execute.
   Notice a pattern? The same two lines of code are executing over and over. This means we are looping. => controllare qual è quella condizione
   che non cambia mai
   */
