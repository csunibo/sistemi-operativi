#!/usr/bin/env python3

#soluzione scritta dal prof

import os
import sys

def copyapp(s, d):
  with open(s, 'r') as sf:
    with open(d, 'a') as df:
      df.write(sf.read())

def copyappdir(sd, dd):
  for d,_,fx in os.walk(sd):
    dstdir = dd + '/' + d[len(sd):]
    os.makedirs(dstdir, exist_ok=True)
    for f in fx:
      copyapp(sd + '/' + f, dstdir + '/' + f)

if name == "main":
  copyappdir(sys.argv[1], sys.argv[3])
  copyappdir(sys.argv[2], sys.argv[3])
