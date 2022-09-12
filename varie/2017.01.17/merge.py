#!/usr/bin/python3
#to chmod +x!
# Scrivere uno script che faccia il merge dei contenuti due directory:
# merge a b c
# Tutti i file (non le directory) esistenti in a e in b devono essere spostati in c. Nel caso due file con lo stesso nome
# compaiano sia in a sia in b, il file spostato in c deve essere quello piu' recentementente modificato, l’altro deve
# rimanere nella directory di origine.
import os
import sys

dicts = []
n = 2
for i in range(n):
	dicts.append([]) #liste per ogni directory che contengono tutti i loro file

for i in range(n):
	for walkingDirPath, subDirs, files in os.walk(sys.argv[1+i]): #for name in files:, for name in subDirs: lista tutti i file/tutte le cartelle con loro path
		for fileName in files:
			fullPath = os.path.join(walkingDirPath, fileName) #path completa
			flag = True
			for j in range(n-1, -1, -1): #per tutte le altre liste
				if j == i: continue #è la lista della directory che stiamo analizzando
				for elem in dicts[j]:
					if fileName == os.path.basename(elem):
						if os.stat(fullPath).st_mtime <= os.stat(elem).st_mtime:
							flag = False
							break
						else:
							dicts[j].remove(elem)
			#dopo che ho controllato TUTTE le altre liste...
			if flag == True: dicts[i].append(fullPath)

superlist = []
for i in range(n):
	for elem in dicts[i]:
		superlist.append(elem) #quelli che sono rimasti

for elem in superlist:
	os.rename(elem, sys.argv[len(sys.argv) -1] + "/" + os.path.basename(elem))
