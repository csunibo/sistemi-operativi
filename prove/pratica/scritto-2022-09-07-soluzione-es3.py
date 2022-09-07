#!/usr/bin/env python3 
import sys,os,magic

paths=["."]
if(len(sys.argv)>1):
    print("arguments found \n")
    print(sys.argv)
    paths=sys.argv
    paths.pop(0)

sum=0

for path in paths:
    print("checking path: " + path)
    for filename in os.listdir(path):
        print("checking file:" + filename)
        fullpath=path+'/'+filename 
        if(not (os.path.isdir(fullpath))):
            filetype=magic.from_file(fullpath)
            if("ELF" in filetype):
                print("ELF file found!\n")
                sum+=os.path.getsize(fullpath)

print('total size: ' + str(sum) + ' bytes')
        
