#!/usr/bin/env python3

import os, sys

data = {}

for dirpath, _, filenames in os.path.walk(sys.argv[1]):
    for filename in filenames:
        if data[filenames] is None:
            data[filenames] = [dirpath]
        else:
            data[filenames].append(dirpath)

for k, v in sorted(data.items()):
    print(k, ":", *v)
