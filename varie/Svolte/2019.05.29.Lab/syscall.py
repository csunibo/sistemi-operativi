import os
import sys

#usare python3 syscall.py `echo "#include<sys/syscall.h>" | gcc -dD -E -`
#successivamente gcc -o file file.c
#per testare il programma ./file nome_system_call
def main():
    #contiene coppie <nome syscall, numero>
    syscall = []
    next = False
    for arg in sys.argv:
        #e' il numero di una syscall matchata al passo precedente
        if next == True:
            syscall[len(syscall) - 1].append(int(arg))
            next = False
        #e' una syscall
        if arg.find("__NR") != -1:
            #taglia per prendere solo il nome
            syscall.append([arg[5:]])
            next = True
        #significa che le abbiamo prese tutte
        if arg == "435":
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