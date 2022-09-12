/* Scrivere un programma che preso come parametro a linea comando il path di una directory elenchi solamente I file */
/* che hanno un nome che termina con un numero (es. Prova10). I file devono essere ordinati in ordine numerico. */
/* Esempio se la directory test contiene I file */
/* prova, giovanni, aaa1000, bb2000, ccc500 */
/* l'output del programma deve essere: */
/* ccc500 */
/* aaa1000 */
/* bb2000 */
/* (in quanto 500 numericamente e' minore di 1000). */
#include <linux/limits.h>
#include <stdlib.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#include <string.h>
#include <stdio.h>
#include <limits.h>
#define TRUE 1
#define FALSE 0
//partendo dal fondo, estraiamo il numero. n = strlen(s) -1
char *has_trailing_digits(char *s, int n){
    char *tmp;
    for (tmp = s+n;(  (*tmp <= '9') && ('0' <= *tmp )); tmp--);
    return tmp + 1;
}

int main(){
    char buf[PATH_MAX]; INIT_ARRAY(buf);
    snprintf(buf, PATH_MAX, "%s", "ciao m0ndo2000");
    printf("Its trailing digits are %s\n", has_trailing_digits(buf, strlen(buf) - 1));



    INIT_ARRAY(buf);
    snprintf(buf, PATH_MAX, "%s", "2000");
    printf("Its trailing digits are %s\n", has_trailing_digits(buf, strlen(buf) - 1));

    exit(EXIT_SUCCESS);
}
