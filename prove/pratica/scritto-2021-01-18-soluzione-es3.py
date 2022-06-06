#!/usr/bin/env python3
import os, sys

data = {}

for dirpath, _, filenames in os.walk("."):
    for filename in filenames:
        filepath = os.path.join(dirpath, filename)
        with open(filepath) as f:
            try:
                if sys.argv[1] in f.read():
                    data[os.path.getmtime(filepath)] = filepath
            except:
                pass

print(*[filepath for _, filepath in sorted(data.items(), reverse=True)], sep="\n")
