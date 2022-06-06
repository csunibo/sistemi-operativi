#!/usr/bin/env python3

import os, sys

argvs = []
is_new_argv = True
current_argv = None

for arg in sys.argv[1:]:
    if arg == "//":
        is_new_argv = True
    elif is_new_argv:
        current_argv = [arg]
        argvs.append(current_argv)
        is_new_argv = False
    else:
        current_argv.append(arg)

for argv in argvs:
    if os.fork() == 0:
        os.execlp(argv[0], *argv)

for _ in argvs:
    os.wait()
