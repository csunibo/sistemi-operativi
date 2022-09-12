'''
Scrivere un programma python o uno script bash che produca il listato del contenuto di una directory
in ordine di suffisso.
'''
import os
import sys

#per ogni estensione
dictionary = {}
#https://stackoverflow.com/questions/3207219/how-do-i-list-all-files-of-a-directory
arr = os.listdir('./')

for element in arr:
	fullPath = os.path.join("./", element)
	if os.path.isdir(fullPath): continue
	extension = os.path.splitext(fullPath)[1]
	if extension in dictionary: #se è tra le chiavi
		dictionary[extension].append(fullPath)
	else:
		dictionary[extension] = [fullPath]
keylist = list(dictionary.keys())
keylist = sorted(keylist, key=str.casefold) #altrimenti maiuscole hanno la precedenza
for key in keylist:
	print(f"{key}: {str(dictionary[key])}")