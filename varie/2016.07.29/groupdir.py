#!/usr/bin/python3
# Scrivere la funzione groupdir in python.
import os
import sys
import grp
import stat
if len(sys.argv) != 3:
    print("BAD USAGE")
    exit()
files = [ f.path for f in os.scandir(sys.argv[1]) ] #quindi directory escluse a priori
for file in files:
    mode = os.lstat(file).st_mode
    if stat.S_ISDIR(mode):
        # https://docs.python.org/3/library/stat.html
        print(file + " è una cartella")
    elif os.stat(file).st_gid == grp.getgrnam(sys.argv[2]).gr_gid:
        print(file)
