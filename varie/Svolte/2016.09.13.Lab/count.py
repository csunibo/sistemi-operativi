import os
import sys

#conta separatamente file e directory
def scandirRec(dir):
    countDir = 0
    countFile = 0
    for entry in os.scandir(dir):
        if entry.is_dir() and entry.name != "." and entry.name != "..":
            recCount = scandirRec(entry.path)
            countFile += recCount[0]
            countDir += 1 + recCount[1]
        elif not entry.is_symlink():
            countFile += 1
    return [countFile, countDir]

def main():
    total = scandirRec(sys.argv[1])
    print("File totali: " + str(total[0]))
    print("Directory totali: " + str(total[1]))


if __name__ == "__main__":
    main()