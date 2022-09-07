#!/usr/bin/python3
# Scrivere un programma python o uno script bash che esaminando i dati in /proc/*/status fornisca in
# output una tabella che indichi per ogni processo di proprietà dell'utente, il nome dell'eseguibile e
# l'attuale occupazione di memoria (campo vmSize). //* = numeri = id processi

import os

folder = "/proc/"
# https://stackoverflow.com/questions/973473/getting-a-list-of-all-subdirectories-in-the-current-directory
subfolders = [ f.name for f in os.scandir(folder) if f.is_dir() ]
for subdir in subfolders:
    if subdir.isnumeric():
        #https://stackoverflow.com/questions/1830618/how-to-find-the-owner-of-a-file-or-directory-in-python
        if os.stat(folder + subdir + "/status").st_uid != os.getuid(): continue
        #https://www.w3schools.com/python/python_file_open.asp
        f = open(folder + subdir + "/status", "r")
        for line in f:
            if "Name" in line or "VmSize" in line: print(line)
        print("----------------")
        f.close()
        #https://www.linuxquestions.org/questions/linux-enterprise-47/uid-and-gid-fileds-from-proc-pid-status-595383/
