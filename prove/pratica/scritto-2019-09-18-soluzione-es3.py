#!/usr/bin/env python3

import os, sys

data = {}

for dirpath, _, filenames in os.walk(sys.argv[1]):
    for filename in filenames:
        path = os.path.join(dirpath, filename)
        ino = os.lstat(path).st_ino
        if ino in data:
            data[ino].append(path)
        else:
            data[ino] = [path]

for value in data.values():
    if len(value) > 1:
        print(*value)
