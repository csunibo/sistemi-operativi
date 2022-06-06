#!/usr/bin/env python3

import time, os, sys

def announce(result):
    print(time.strftime("%m/%d/%Y %H:%M:%S", time.localtime(result[0])),
          result[1])

newer = older = None, None

for dirpath, _, filenames in os.walk(sys.argv[1]):
    for filename in filenames:
        path = os.path.join(dirpath, filename)
        mtime = os.path.getmtime(path)
        if newer[0] is None:
            newer = older = mtime, path
        elif mtime > newer[0]:
            newer = mtime, path
        elif mtime < older[0]:
            older = mtime, path

announce(newer)
announce(older)
