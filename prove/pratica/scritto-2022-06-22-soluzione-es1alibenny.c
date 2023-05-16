/** Esercizio 1
 * Scrivere due programmi C, tx e rx:
 * - tx deve trasferire a rx stringhe max di 8 caratteri usando i valori assegnati ai segnali [il parametro value della funzione sigqueue].
 *  - ha 2 parametri: pid di rx e il messaggio
 * - rx deve stampare il proprio pid e attendere segnali.
 * 
 * Esempio:
 * tx 22255 sigmsg
 * [posto che rx sia in esecuzione con pid 22255, rx deve stampare sigmsg]
 * 
 * testo esame https://github.com/csunibo/sistemi-operativi/blob/main/prove/pratica/scritto-2022-06-22-testo.pdf
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // sigqueue
#include <unistd.h> // wait 

void tx(int pid, char *sigmsg) {
	if (lenght(sigmsg)<= 8) {
		union sigval value;
		value.sival_ptr = (void*)sigmsg;
		int queue_return = sigqueue(pid, sigmsg, value);
		if (queue_return == -1) {
			printf("Errore nell'invio del segnale");
			exit(-1);
		}
	}
	else printf("Sigmsg too long\n");
}

void sig_handler(int sig, siginfo_t *info, void *ucontext){
	union sigval value = info->si_value;
	int64_t tmp = (int64_t)value.sival_ptr;
	char converted[8];
	for(int i=0; i<8; i++){
		converted[i]=tmp%256;
		tmp/=256;
	}
	printf("RECEIVED: %s\n", converted);
}
void rx() {
	// int signal = pause();
	// if (signal == -1) {
    //    printf("Segnali ricevuti");
    // }
	// printf("pid: %d\n", getpid());
	pid_t pid = getpid();
	printf("My pid: %d\n", (int)pid);
	
	
	struct sigaction sigact;
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = sig_handler;
	while(1){
		int ret = sigaction(SIGUSR1, &sigact, NULL);
	}
	exit(0);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <pid> <sigmsg>\n", argv[0]);
		return 1;
	}
	// salvo i parametri
	int pid = atoi(argv[1]);
	char *sigmsg = argv[2];
	// invio i segnali
	tx(pid, sigmsg);
	// attendo i segnali
	rx();
	return 0;
