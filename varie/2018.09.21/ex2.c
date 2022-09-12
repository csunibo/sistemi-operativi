/* Dato un estratto di un file di log come questo: */
/* Sep 20 19:58:32 eipi ntpd[2051]: Listen normally on 99 eth0 [fe80::8456:a70]:123 */
/* Sep 20 19:58:32 eipi ntpd[2051]: new interface(s) found: waking up resolver */
/* Sep 20 19:58:34 eipi ntpd[2051]: Soliciting pool server 151.3.106.211 */
/* Sep 20 19:58:35 eipi ntpd[2051]: Soliciting pool server 94.177.187.22 */
/* Sep 20 19:58:48 eipi ntpd[2051]: Soliciting pool server 80.211.155.206 */
/* Sep 20 19:58:49 eipi ntpd[2051]: Soliciting pool server 212.45.144.206 */
/* Scrivere un programma che lo mostri alla stessa velocita' allla quale e' stato generato. */
/* In questo caso dovrebbe scrivere le prime dur righe, attendere 2 secondi, mostrare la terza, dopo un secondo la */
/* quarta quindi dopo una attesa di 13 secondi la quinta e la sesta dopo un ulteriore secondo. */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

int main(){
    int timeouts[6] = {0, 2, 1, 13, 1, 0};
    FILE *stream = fopen("./logfile.txt", "r");
    if (stream == NULL){
        perror("\nfopen()");
        exit(EXIT_FAILURE);
    }
    char buf[512]; bzero(buf, sizeof(buf));
    for (int i = 0; i < sizeof(timeouts)/sizeof(int); i++){
        //reset
        bzero(buf, sizeof(buf));
        if (fgets(buf, sizeof(buf), stream) == NULL){
            perror("\nfgets()");
            exit(EXIT_FAILURE);
        }
        printf("%s", buf);
        sleep(timeouts[i]);
    }
    fclose(stream);
    exit(EXIT_SUCCESS);
}
