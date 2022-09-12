import os
import sys




def main():
    list = []
    #scandisce l'albero e ne estrae directory e files
    for (root,dirs,files) in os.walk(sys.argv[1], topdown = True):
        #per ogni file trovato
        for file in files:
            #prendiamo le informazioni (non segue symlinks)
            stat = os.lstat(root + "/" + file)
            #aggiungiamo alla lista una entry contenente l'inode e il path
            entry = [stat.st_ino, root + "/" + file]
            list.append(entry)
    
    while len(list) > 0:
        found = False
        toPrint = ""
        #prendiamo il primo dalla lista
        entry = list.pop(0)
        inode = entry[0]
        todelete = []
        #vediamo se esiste un file con lo stesso inode
        for elem in list:
            if elem[0] == inode:
                found = True
                toPrint += " " + elem[1]
                todelete.append(elem)
        #se ne abbiamo trovato uno li stampiamo
        if found == True:
            print(entry[1] + toPrint)
            #ed eliminiamo tutte le entry dalla lista
            for item in todelete:
                list.remove(item)
            






if __name__ == "__main__":
    main()