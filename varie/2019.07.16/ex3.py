##!/usr/bin/python3
## Scrivere un programma python o uno script bash che cerchi tutti i file con uguale contenuto in una
## directory. I file con lo stesso contenuto devono diventare link fisici a un unico file.
## https://stackoverflow.com/questions/1072569/see-if-two-files-have-the-same-content-in-python
#import filecmp
#import os
#import subprocess
#import time
#import hashlib

#files = [f.name for f in os.scandir() if f.is_file()]
#for i in range(0, len(files)-1):
#    print("sto analizzando " + files[i])
#    with open(files[i],"rb") as f:
#        bytes = f.read() # read entire file as bytes
#        readable_hashi = hashlib.sha256(bytes).hexdigest();
#    with open(files[i +1], "rb") as f1:
#        bytes = f1.read()
#        readable_hashiii = hashlib.sha256(bytes).hexdigest()
#    f1.close()
#    f.close()
#    if readable_hashiii == readable_hashi:
#        os.unlink(os.path.abspath(files[i+1]))
#        os.link(os.path.abspath(files[i]),files[i+1] + "hl")
        # i+=1


import os, sys

data = {}

dir = sys.argv[1]
for dirent in os.listdir(dir):
    path = os.path.join(dir, dirent)
    if os.path.isfile(path):
        removable = False
        with open(path) as f:
            content = f.read()
            if content in data:
                removable = True
            else:
                data[content] = path
        f.close()
        if removable:
            os.remove(path)
            os.link(data[content], path)
