#! /usr/bin/python3
#which python3, chmod +x main.py
'''
Scrivere uno script o un programma python che corregga l'indentazione di tutti i file .c e .h presenti
nel sottoalbero della directory passata come parametro (la working directory se non vi sono
parametri).
Hint: il comando:
ex -n '+norm!gg=G' +wq prog.c
corrregge l'indentazione del programma sorgente C prog.c
'''
#sudo apt install indent
import sys
import os

if len(sys.argv) == 1: directory = os.getcwd()
else: directory = sys.argv[1]
print(f"Analizzando albero della directory {directory}")

for walkingDirPath, subDirs, files in os.walk(directory):
	for fileName in files:
		fullPath = os.path.join(walkingDirPath, fileName) #path completa
		#https://stackoverflow.com/questions/541390/extracting-extension-from-filename-in-python
		extension = os.path.splitext(fullPath)[1]
		if extension == ".c" or extension == ".h":
			os.system("ex -n \"+norm!gg=G\" +wq \"" + fullPath + "\"")
			print(f"File indentato correttamente: {fullPath}")
print(f"Finito!")