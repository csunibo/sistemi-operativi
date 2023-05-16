# link testo https://csunibo.github.io/sistemi-operativi/prove/pratica/scritto-2021-06-24-testo.pdf

"""	Esercizio 3
Scrivere uno script o un programma python che corregga l'indentazione di tutti i file .c e .h presenti
nel sottoalbero della directory passata come parametro (la working directory se non vi sono
parametri).
Hint: il comando:
	ex -n '+norm!gg=G' +wq prog.c
corregge l'indentazione del programma sorgente C prog.c.
"""

import os
import sys

def correct_indentation(path):
	for root, dirs, files in os.walk(path):	# sys.argv[1] è la directory passata come parametro
		for file in files:	
			if file.endswith(".c") or file.endswith(".h"):
				os.system("ex -n '+norm!gg=G' +wq " + os.path.join(root, file))	# esegue il comando ex per ogni file .c o .h

if __name__ == "__main__":
	if len(sys.argv) > 1:
		correct_indentation(sys.argv[1])
	else:
		correct_indentation(".")