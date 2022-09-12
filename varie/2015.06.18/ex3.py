#!/usr/bin/python3
# Scrivere un programma python o uno script bash che conti il numero dei file eseguibili presenti nella directory
# passata come parametro catalogandoli per tipo di interprete. (se il file e' di testo si guarda cio' che segue #! nella
# prima riga, tutti quelli che non iniziano con #! contateli come generici “exec”)
# $ contaexec /usr/bin
# /bin/bash: 20
# /usr/bin/env: 10
# exec: 12
import os, sys

dicti = {}


# https://stackoverflow.com/questions/898669/how-can-i-detect-if-a-file-is-binary-non-text-in-python
def is_text_file(path):
    try:
        with open(path, "r", encoding="utf-8") as f:
            first_line = f.readline()
            f.close()
        return first_line, True
    except UnicodeDecodeError:
        return "", False


files = [f.path for f in os.scandir(sys.argv[1]) if f.is_file()]
for f in files:
    if os.access(f, os.X_OK):
        first_line, boolean = is_text_file(f)
        if boolean == False:
            if "exec" in dicti:
                dicti["exec"].append(f)
            else:
                dicti["exec"] = [f]
        else:
            if first_line in dicti:
                dicti[first_line].append(f)
            else:
                dicti[first_line] = [f]

for k in dicti:
    print(k, len(dicti[k]))
