#!/usr/bin/env python3

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
        if removable:
            os.remove(path)
            os.link(data[content], path)
