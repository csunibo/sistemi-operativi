import os
import sys


filemap = {}

def recscan(path):
    for entry in os.scandir(path):
        #se e' una directory la scansiona ricorsivamente
        if entry.is_dir() == True:
            recscan(entry.path)
        #altrimenti se e' un file
        else:
            #apre il file e lo legge (l'encoding serve o puo' dare problemi strani)
            file = open(entry.path, "r", encoding='cp437')
            str = file.read()
            #vede se inizia con un shabang
            if str.startswith("#!"):
                #divide tutto in liste separate da spazi, quindi list[0] e' cio' che ci interessa
                list = str.split("\n")
                #se esiste gia' questa key la appende alla sua lista, altrimenti ne crea una nuova
                if list[0] in filemap:
                    filemap[list[0]].append(entry.path)
                else:
                    filemap[list[0]] = [entry.path]


def main():
    recscan(sys.argv[1])
    for shabang in filemap:
        name = ""
        for entry in filemap[shabang]:
            name = name + " " + entry
        print(shabang + name)

if __name__ == "__main__":
    main()