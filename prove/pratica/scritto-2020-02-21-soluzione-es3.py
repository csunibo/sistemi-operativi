#!/usr/bin/env python

import os, sys

data = {}

for filename in os.listdir(sys.argv[1]):
    key = filename.split(".")[-1] if "." in filename else ""
    if key in data:
        data[key].append(filename)
    else:
        data[key] = [filename]
for _, value in sorted(data.items()):
    print(*value, sep="\n")
