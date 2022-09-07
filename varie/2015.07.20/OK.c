/* IL programma weakripto deve “criptare” un file. */
/* weakripto filename */
/* sostituisce il contenuto del file indicato come parametri (non fa copie! Il file deve essere riscritto sul posto) */
/* e ogni byte deve avere I bit scambiati rispetto al byte corrispondente nel contenuto originale. */
/* Se il primo byte contiene il carattere ascii A (binario 0x41 binario 01000001) il primo byte del file dopo la */
/* trasformazione sara' 0x82 (10000010). La lettera ascii B (0x42) rimane invariata. */
/* Si chiede di non usare file temporanei e di non caricare l'intero file in memoria. */
/* Come prova di fuunzionamento, eseguendo due volte lo stesso comando il contenuto del file deve tornare quello */
/* originale. */
/* http://sticksandstones.kstrom.com/appen.html */
#include <stddef.h>
#include <strings.h>
#include <stdio.h>
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include <stdlib.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define TRUE 1
#define FALSE 0

/* https://stackoverflow.com/questions/16254497/c-read-and-replace-char */ //IMPORTANTISSIMO !!!
/* https://www.youtube.com/watch?v=Kj3iboADvUc */
char reverse_bin(char c){
    const size_t size = sizeof(char);
    const int BITS = size*8;
    char bin[BITS+1]; bzero(bin, BITS + 1); bin[BITS] = '\0';
    int k = 0;
    for (int i=0; i < size; i++){ //for each byte
        char byte = ((char *)&c)[i]; //((char *)c)[i];//*( (char *)c + i ); // 13 => 1011 = 1 + 4 + 8
        //il linguaggio C in sè non è nè BIG ne little ENDIAN. Tutto dipende da cosa noi prendiamo (il bit meno significativo) ;)
        for (int j = 0; j < 8; j++){//for each bit. Starting from 0 in order to get the least significant bit. Si parte a leggere l'iesimo byte DA DESTRA. IN MODO DA PRINTARE LA CORRETTA RAPPRESENTAZIONE => NON 1101 ALLA BONONI MA 1011. SE BIG ENDIAN => j = 7. Big ENDIAN per assurdo è già la versione reversata del carattere. Quindi una volta invertita ritorna il carattere passato come input
            /* int bit = (byte >> j) & 1; */
            char bit = (byte >> j) & 1;
            /* printf("%d", bit); //voglio printare il numero, non il carattere ASCII //LITTLE ENDIAN */
            sprintf(&bin[k], "%hhd", bit); //"half of half"
            k++;
        }
    }
    for (int i=0; i < size/2; i++){
        char tmp = bin[i]; //it's going to be modified
        bin[i] = bin[size-1-i];
        bin[size-1-i] = tmp;
    }
    printf("%s\n", bin);
    return (char) strtol(bin, (char **)NULL, 2);
}

int main(int argc, char **argv){
    printf("%c\n", reverse_bin('a'));
    printf("%c\n", reverse_bin(reverse_bin('a')));
    /* char c = 'P'; */
    /* reverse_bin((void *)&c, sizeof(char)); */
    /* int n = 13; */
    /* reverse_bin((void *)&n, sizeof(n)); */
    if (argc != 2){
        fprintf(stderr, "BAD USAGE\n");
        exit(EXIT_FAILURE);
    }
    FILE *stream = fopen(argv[1], "rb+");
    if (stream == NULL) HANDLE_ERROR("fopen\n");
    char c; //anzichè gioco fget/setpos
    while ( (c = fgetc(stream)) != EOF ) { //+-+
        fseek(stream, -1, SEEK_CUR);
        if (fputc(reverse_bin(c), stream) == EOF) HANDLE_ERROR("fputc");
        fprintf(stderr, "ho printato %c\n", reverse_bin(c));
    }
    fclose(stream);
    exit(EXIT_SUCCESS);
}
