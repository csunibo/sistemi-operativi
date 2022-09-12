#!/usr/bin/python3
import os, sys
def forEachDir(pathdir):
	counter = 0 #numero bytes totali
	files = [f.path for f in os.scandir(pathdir) if f.is_file()]
	for file in files:
		if os.access(file, os.X_OK):
			with open(file, "rb") as f:
				bytes = f.read(7)
			decoded = bytes.decode("utf-8")
#https:/	/stackoverflow.com/questions/12822570/how-do-i-check-if-an-object-file-is-in-coff-or-elf-format-from-within-c
			print(decoded)
			if "ELF" in decoded:
				counter+= os.stat(file).st_size
	return counter

folders = []
if len(sys.argv) < 2: folders.append(".")
else:
	for i in range(1, len(sys.argv)):folders.append(sys.argv[i])
tot = 0
for folder in folders: tot+=forEachDir(folder)
print(str(tot))
