#!/usr/bin/env python3

# Prima lancia:
# echo "#include<sys/syscall.h>" | gcc -dD -E - >> syscall

import sys


def isnumber(str):
    str = str.lstrip("(-")
    if str.startswith("double"):
        return True
    return str[0].isnumeric()


data = {}

with open("syscall") as f:
    for line in f:
        line = line[0:-1]
        tokens = line.split(" ")
        if (
            tokens[0] == "#define"
            and len(tokens) == 3
            and tokens[2] not in ("", "int", "c")
        ):
            while not isnumber(tokens[2]):
                if tokens[2] in data:
                    tokens[2] = data[tokens[2]]
                else:
                    print("Simbolo non definito precedentemente: '" + tokens[2] + "'")
            data[tokens[1]] = tokens[2]

print(data["SYS_" + sys.argv[1]])
