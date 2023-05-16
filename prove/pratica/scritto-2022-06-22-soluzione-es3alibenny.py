#!usr/bin/env python3
'''
Esercizio 3
Scrivere uno script bash o un programma python che stampi l'elenco dei file di una directory catalogati per output del comando `file`

Esempio:
ASCII text: file.txt
directory: mydir
empty: nullfile
PDF document, version 1.6: mydoc.pdf
'''

import os
import sys
import subprocess

def print_file_info():
	if len(sys.argv) < 2:
		print('Usage: python3 file.py <directory>')
		sys.exit(1)

	directory = sys.argv[1]

	for filename in os.listdir(directory):
		filepath = os.path.join(directory, filename)
		info = subprocess.run(['file', filepath], capture_output=True, text=True)
		print(f"{info.stdout.strip()}: {filename}")

if __name__ == '__main__':
	print_file_info()