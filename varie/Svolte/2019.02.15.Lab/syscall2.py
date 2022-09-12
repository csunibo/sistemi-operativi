import os
import sys

#Per compilare il file c usare gcc -o file file.c
#per testare il programma ./file nome_system_call
def main():
    #contiene coppie <nome syscall, numero>
    syscall = []
    file = open("/usr/include/x86_64-linux-gnu/asm/unistd_64.h")
    for arg in file:
        #e' una syscall
        if arg.find("__NR") != -1:
            #cerca nella riga in analisi il numero
            for word in arg:
                if word.isdigit():
                    num = int(word)
            #slice, il nome va da 13 fino al valore della lunghezza riga - la lunghezza del numero - 2
            syscall.append([arg[13:len(arg) - len(str(num)) - 2], num])
        #significa che le abbiamo prese tutte
        if arg.find("endif") != -1:
            break
    
    #creiamo una stringa contenente il programma
    #usiamo una struttura che mantiene un nome e il numero di syscall
    #riempiamo un array di strutture aggiungendoli tutte le syscall
    #che abbiamo trovato precedentemente (usiamo interpolazione di stringhe)
    program = """
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    typedef struct entry
    {
        char *name;
        int num;
    } syscall;
    

    int main(int argc, char const *argv[])
    {
        syscall list[] = {
    """

    for entry in syscall:
        program += "{" + f'"{entry[0]}"' + ", " f"{entry[1]}" + "},"
    program = program[:len(program) - 1] + "};"

    program += f"int length = {len(syscall) - 1};"

    #questa e' la parte di programma che semplicemente fa match
    #la ricerca avviene sull argv[1] passato per parametro
    program += """
    for(int i = 0; i < length; i++)
    {
        if (strcmp(argv[1], list[i].name) == 0)
        {
            printf("%s %d\\n", list[i].name, list[i].num);
            return 0;
        }
    }
    printf("system call does not exist\\n");
    return 1;
    }
    """

    #crea un nuovo file, ci scrive il programma e lo chiude
    newfile = os.open("./file.c", os.O_RDWR|os.O_CREAT)
    
    os.write(newfile, program.encode())

    os.close(newfile)




if __name__ == "__main__":
    main()