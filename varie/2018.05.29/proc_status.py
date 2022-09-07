import os
import sys

id_map = {}

def main():
    #scansiona la directory proc/
    for direntry in os.scandir("/proc"):
        #se trova un'altra dicetory la scandisce
        if direntry.is_dir():
            for dirdirentry in os.scandir(direntry.path):
                #se trova un file chiamato status lo apre
                if dirdirentry.name == "status":
                    fd = open(dirdirentry.path, "r")
                    #legge per linea
                    lines = fd.readlines()
                    #indica se il processo e' Running o Sleeping
                    b = False
                    for line in lines:
                        #se e' un nuovo processo da analizzare
                        if line.startswith("State"):
                            try:
                                #se e' running imposta b a true
                                line.index("(running)")
                                b = True
                            except:
                                #altrimenti la imposta a false
                                b = False
                        #se il processo analizzato e' running e siamo alla riga Uid
                        if b == True and line.startswith("Uid"):
                            #divide la riga in una lista per tab come separatore
                            id = line.split("\t")
                            for i in range(1, 5):
                                #serve o conta indici in piu (l'ultimo va a capo nel file)
                                if id[i].endswith('\n'):
                                    id[i] = id[i].replace("\n", "")
                                #se e' un processo appartenente ad un nuovo user
                                if id[i] not in id_map:
                                    #crea una entry
                                    id_map[id[i]] = [0, 0, 0, 0]
                                #incrementa di 1
                                id_map.get(id[i])[i - 1] += 1
                                

    for uid in id_map:
        print(uid + "\t" + str(id_map[uid]))








if __name__ == "__main__":
    main()