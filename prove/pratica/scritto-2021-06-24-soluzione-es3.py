#!/bin/env python3
import os
import sys
import re
from os import path

r = re.compile('\w+\.(c|py)$')

def main():
	matches = []
	for root, dirnames, filenames in os.walk(sys.argv[1] if len(sys.argv) > 1 else os.getcwd()):
		for filename in filter(r.match, filenames):
			print('formatting ' + os.path.join(root, filename))
			os.system('ex -n \'+norm!gg=G\' +wq ' + os.path.join(root, filename))

if __name__ == '__main__':
    main()
