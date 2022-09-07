#!/usr/bin/python3
# I file /proc/*/status contengono importanti informazioni relative ai processi in esecuzione.
# In particolare una riga contiene la costante "Uid:" seguita da quattro numeri (sono l'uid reale, effettivo, salvato e del
# file system per il processo in considerazione).
# Scrivere un programma python o uno script bash che per ogni utente che ha un ruolo in almeno un processo
# indichi quanti processi sono attivi che hanno quell'utente come reale, effettivo, salvato o del file system.
# esempio di output:
# 0 10 12 12 12
# 1000 5 3 3 3
# (l'utente 0 ha 10 processi come utente reale, 12 come effettivo, salvato e del file system. l'utente 1000 ha 5
# processi come utente reale e 3 come effettivo, salvato e del file system. Per esempio questo caso si riferirebbe a
# 10 processi di root, 5 dell'utente 1000 dei quali 2 sono setuid root).

# if fileName in dictionary: #se è tra le chiavi
# 	dictionary[fileName].append(walkingDirPath)
# else:
# 	dictionary[fileName] = [walkingDirPath]
# keylist = list(dictionary.keys())
# keylist = sorted(keylist, key=str.casefold) #altrimenti maiuscole hanno la precedenza
# for key in keylist:
# print(f"{key}: {str(dictionary[key])}")


import os

dicts = [
    {},
    {},
    {},
    {},
]  # un dizionario che per ogni processo tiene gli effettivi; un dizionario che per ogni processo tiene i reali, etc...
procs = []
folder = "/proc/"
# https://stackoverflow.com/questions/973473/getting-a-list-of-all-subdirectories-in-the-current-directory
subfolders = [f.name for f in os.scandir(folder) if f.is_dir()]
for subdir in subfolders:
    if subdir.isnumeric():
        # https://stackoverflow.com/questions/1830618/how-to-find-the-owner-of-a-file-or-directory-in-python
        # https://www.w3schools.com/python/python_file_open.asp
        f = open(folder + subdir + "/status", "r")
        for line in f:
            if "Uid" in line:
                numbers = [int(s) for s in line.split() if s.isdigit()]  # IDs
                for i in range(len(numbers)):
                    if numbers[i] in dicts[i]:
                        dicts[i][numbers[i]] += 1
                    else:
                        dicts[i][numbers[i]] = 1
                    if numbers[i] not in procs:  # I NUMERI SONO I PROCESSI!!!
                        procs.append(numbers[i])

                    # if numbers[i] in dicts[4]:
                    #     path = folder + subdir
                    #     if path not in dicts[4][numbers[i]]: dicts[4][numbers[i]].append(folder + subdir)
                    # else: dicts[4][numbers[i]] = [(folder + subdir)]
        f.close()
        # https://www.linuxquestions.org/questions/linux-enterprise-46/uid-and-gid-fileds-from-proc-pid-status-595383/
print(procs)
for p in procs:
    print(p)
    for i in range(len(dicts)):
        if p in dicts[i]:
            print(dicts[i][p])
        else:
            print("0")
    print("--------")
