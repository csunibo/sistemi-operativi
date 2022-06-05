#!/usr/bin/env python3

import subprocess
import string

dict = {}

def main():
  out = subprocess.check_output(['getent', 'passwd'])
  for line in out.splitlines():
    l = line.decode('utf-8')
    parts = l.split(':')
    home = parts[5]
    if '/home/' in home:
      # key = '/'.join(home.split('/')[:-1])
      key = home[:home.rfind('/')]
    else:
      key = home
    if key in dict:
      dict[key] += 1
    else:
      dict[key] = 1

  for key in dict:
    print(key, " ", dict[key])

if __name__ == "__main__":
  main()
