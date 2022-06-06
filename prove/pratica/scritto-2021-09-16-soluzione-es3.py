#!/usr/bin/env python3

import os, sys

data = {}

for dirpath, _, filenames in os.walk(sys.argv[1]):
    for filename in filenames:
        if filename in data:
            data[filename].append(dirpath)
        else:
            data[filename] = [dirpath]

for k, v in sorted(data.items()):
    print(k + ":", *v)
