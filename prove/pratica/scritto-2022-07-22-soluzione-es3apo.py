import sys
import os
import magic
import subprocess

path = '.'

if (len(sys.argv) > 1):
	path = sys.argv[1]


for (dirpath, dirnames, filenames) in os.walk(path):
	if(dirpath == path):
		for file in filenames:
			filep = (dirpath + '/' + file)
			if(os.access(filep, os.X_OK)): #check if a file is executable
				filetype = (magic.from_file(filep))
				if not ("ELF" in filetipe): #check if a file is NOT an ELF 
					subprocess.call(filep, shell=True) #exec the script
