#! /usr/bin/python3
#which python3, chmod +x main.py
#Scrivere uno script in grado si cercare all'interno di un sottoalbero del file system il file modificato più
#di recente e quello la cui ultima modifca è avvenuta più anticamente.
#https://stackoverflow.com/questions/2731014/finding-most-recently-edited-file-in-python
import os
import sys

print(f"root del filesystem: {os.getcwd()}")
#https://stackoverflow.com/questions/595305/how-do-i-get-the-path-of-the-python-script-i-am-running-in
print(f"dove si trova l'eseguibile: {os.path.abspath(os.path.dirname(sys.argv[0]))}")
maxEditTime = float('-inf')
minEditTime = float('inf')
#per cambiare directory analizzata => cambiare argomento (non sarebbe l'unico) di os.walk
for walkingDirPath, subDirs, files in os.walk("."): #for name in files:, for name in subDirs: lista tutti i file/tutte le cartelle con loro path
	for fileName in files:
		fullPath = os.path.join(walkingDirPath, fileName) #path completa
		editTime = os.stat(fullPath).st_mtime
		if editTime > maxEditTime:
			maxEditTime = editTime
			maxDir = walkingDirPath
			maxFile = fileName
		if editTime < minEditTime:
			minEditTime = editTime
			minDir = walkingDirPath #directory con path che sto correntemente analizzando, NON quella da cui sono partito 
			minFile = fileName

print(f"File modificato + recentemente: {maxDir}/{maxFile} at {maxEditTime}")
print(f"File modificato + anticamente: {minDir}/{minFile} at {minEditTime}")