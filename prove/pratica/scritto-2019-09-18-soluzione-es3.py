#!/bin/env python3
import sys
import os

f = {}

for root, _, files in os.walk(sys.argv[1]):
    for file in files:
        fp = os.path.abspath(os.path.join(root, file))
        key = fp if not os.path.islink(fp) else os.path.realpath(fp)
        if key in f:
            f[key].append(fp)
        else:
            f[key] = [fp]

for key in f:
    if len(f[key]) > 1:
        for file in f[key]:
            print(os.path.relpath(file, os.path.join(sys.argv[1],'..'))+' ', end='')
        print('\n', end='')

