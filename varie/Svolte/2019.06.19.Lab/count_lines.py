import os
import sys

#dizionario di liste per ogni estensione
filemap = {".c": [], ".h": [], "makefile": []}

def recscan(path, match):
    #per ogni entry nella dir
    for entry in os.scandir(path):
        #se e' una directory la scansiona ricorsivamente
        if entry.is_dir() == True:
            recscan(entry.path, match)
        #altrimenti se e' un file che fa match con l'estensione
        elif entry.name.endswith(match):
            #conta il numero di linee
            num_lines = sum(1 for line in open(path + "/" + entry.name))
            entry = [path + "/" + entry.name, num_lines]
            #aggiunge una coppia <path, numero linee>
            filemap[match].append(entry)



def main():
    #effettua una scansione per ogni estensione della dir passata come argument
    recscan(sys.argv[1], ".c")
    recscan(sys.argv[1], ".h")
    recscan(sys.argv[1], "makefile")

    #conta le linee totali e stampa filepath: numero linee del file
    totalc = 0
    for entry in filemap[".c"]:
        print(entry[0] + ": " + str(entry[1]))
        totalc += entry[1]
    #stampa il totale delle line per i file c
    print("Total c: " + str(totalc))
    
    #idem per .h
    totalh = 0
    for entry in filemap[".h"]:
        print(entry[0] + ": " + str(entry[1]))
        totalh += entry[1]

    print("Total h: " + str(totalh))

    #idem per makefile
    totalm = 0
    for entry in filemap["makefile"]:
        print(entry[0] + ": " + str(entry[1]))
        totalm += entry[1]
    
    print("Total makefile: " + str(totalm))














if __name__ == "__main__":
    main()