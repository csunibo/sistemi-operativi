import os
import sys

def list_to_string(argv):
    str = ""
    first = True
    for arg in argv:
        if first:
            str += arg
            first = False
        else:
            str += " " + arg
        if arg == "//":
            first = True
    return str

def main():
    #creiamo una stringa di argomenti
    arg = list_to_string(sys.argv[1:])
    #la trasformiamo in una lista usando // come separatore
    commands = arg.split("//")
    for i in commands:
        #per ogni comando crea un subprocess
        pid = os.fork()
        #se e' il figlio
        if pid == 0:
            #spezza il comando e le opzioni in una lista
            command = i.split(" ")
            #se c'e' uno spazio finale lo elimina
            if command[-1] == '':
                command = command[:-1]
            #esegue il comando passandogli le opzioni come argomenti
            os.execlp(command[0], command[0], list_to_string(command[1:]))
    
    #aspetta tante volte quanti i figli che ha generato
    for i in commands:
        os.wait()


if __name__ == "__main__":
    main()