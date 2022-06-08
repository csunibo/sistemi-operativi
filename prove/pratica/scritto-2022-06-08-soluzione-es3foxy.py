#!/usr/bin/env python3

import os, pwd

proc = "/proc/"
outputformat = "{:<20} | {}"

def ismine(path):
    return pwd.getpwuid(os.stat(path).st_uid).pw_name == os.getlogin()

data = {}

for dirent in os.listdir(proc):
    if dirent not in ["net", "mounts", "self", "thread", "thread-self"]:
        path = proc + dirent + "/status"
        if ismine(proc + dirent):
            name, vmsize = '', ''
            with open(path) as f:
                for line in f:
                    tokens = line.split()
                    if tokens[0] == 'Name:':
                        name =  ' '.join(tokens[1:])
                    elif tokens[0] == 'VmSize:':
                        vmsize = ' '.join(tokens[1:])
            data[name] = vmsize

print(outputformat.format('Nome','Memoria'))
for k, v in data.items():
    print(outputformat.format(k, v))

