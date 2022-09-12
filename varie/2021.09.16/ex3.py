'''
Scrivere un programma/script che faccia la lista riscorsiva dei file in un sottoalbero riportando in
ordine alfabetico per nome di file in quale/quali sottodirectory compare.
e.g.
rlsr mydir
ciao: . ./a
mare: ./a ./b
sole: .
Significato: un file con nome ciao esite nella directory mydir ma anche all'interno della sottodirectory
a (esistono cioe' i file mydir/ciao e mydir/a/ciao).
'''

import os
import sys

dictionary = {} #chiave: nome file; valore: lista delle directory nel quale il nome del file è presente 

#https://stackoverflow.com/questions/1602934/check-if-a-given-key-already-exists-in-a-dictionary
#https://www.w3schools.com/python/gloss_python_dictionary_add_item.asp
for walkingDirPath, subDirs, files in os.walk("."): #for name in files:, for name in subDirs: lista tutti i file/tutte le cartelle con loro path
	for fileName in files:
		#fullPath = os.path.join(walkingDirPath, fileName) #path completa
		if fileName in dictionary: #se è tra le chiavi
			dictionary[fileName].append(walkingDirPath)
		else:
			dictionary[fileName] = [walkingDirPath]
keylist = list(dictionary.keys())
keylist = sorted(keylist, key=str.casefold) #altrimenti maiuscole hanno la precedenza
for key in keylist:
	print(f"{key}: {str(dictionary[key])}")