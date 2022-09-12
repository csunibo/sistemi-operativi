# Scrivere un programma python o uno script bash che scandisca il sottoalbero relativo alle directory passate come
# parametri (o alla direcotry corrente se non ci sono parametri) e fornisca in output l'elenco dei file che hanno la
# stessa somma MD5 (i.e. l'output del comando md5sum).
# In output ogni riga deve mostrare un elenco di pathname realtivi a file che hanno la stessa somma MD5 (che quindi
# sono molto molto probabilmente uguali).
import hashlib, sys, os

if len(sys.argv) != 2:
    folder = "."
else:
    folder = sys.argv[1]
dicti = {}
for walkingDirPath, subDirs, files in os.walk(folder):
    for f in files:
        file = os.path.join(walkingDirPath, f)
        # https://stackoverflow.com/questions/16874598/how-do-i-calculate-the-md5-checksum-of-a-file-in-python
        checksum = hashlib.md5(open(file, "rb").read()).hexdigest()
        if checksum in dicti:
            dicti[checksum].append(file)
        else:
            dicti[checksum] = [file]

for k in dicti:
    print(k)
    for v in dicti[k]:
        print(v[len(folder) :])  # così facendo la path è relativa ;)
