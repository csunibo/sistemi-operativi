#!/usr/bin/env python3 
'''
/** Esercizio 3
 * Scrivere un programma python o uno script bash che calcoli l'ampiezza totale in byte dei file eseguibili presenti in tutte le directory passate come parametri o nella directory corrente se non vengono passati parametri.
 * 
 * esempio:
 * $ elfsize /bin /usr/bin
 * 1682573547
 */
'''
import os
import sys
import subprocess

def elfsize(path):
	'''
	Calcola l'ampiezza totale in byte dei file eseguibili presenti in tutte le directory passate come parametri o nella directory corrente se non vengono passati parametri.
	'''
	size = 0
	for dir in path:
		for root, dirs, files in os.walk(dir):
			for file in files:
				file_path = os.path.join(root, file)
				result = subprocess.run(['file', '--brief', '--mime-type', file_path], stdout=subprocess.PIPE)
				output = result.stdout.decode('utf-8').strip()
				if output == 'application/x-executable':
					size += os.path.getsize(file_path)
	return size
	
if __name__ == '__main__':
	if len(sys.argv) > 1:
		for path in sys.argv[1:]:
			print(elfsize(path))
	else:
		print(elfsize(os.getcwd()))