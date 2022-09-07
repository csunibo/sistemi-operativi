# Le directory di tipo /proc/nnn/fd (dove nnn e' un numero) mostrano i descrittori di file aperti dai processi.
# Scrivere uno script bash o un programma python che mostri quali processi stanno usando piu' descrittori,
# (ad alcune directory /proc/nnn e' vietato l'accesso perche' sono di altri utenti, saltatele nella scansione).

import os
dicti = {}
tuples = []
folder = "/proc/"
# https://stackoverflow.com/questions/973473/getting-a-list-of-all-subdirectories-in-the-current-directory
subfolders = [ f.name for f in os.scandir(folder) if f.is_dir() ]
for subdir in subfolders:
    if subdir.isnumeric():
        descs = []
        try: descs =  [ f.name for f in os.scandir(folder + subdir + "/fd/") ]
        except: pass
        else:
            dicti[subdir] = len(descs)
            tuples.append(( subdir, len(descs) )) #dava problemi di EOF => controllare parentesi

tuples.sort(reverse = True, key=lambda y: y[1])
print(tuples)
