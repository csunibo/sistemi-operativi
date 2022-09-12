import os
import sys



name = []

def main():
    #riempiamo prima con i file della prima dir
    for entry in os.scandir(sys.argv[1]):
        if entry.is_dir() == False:
            name.append([entry.name, entry.path])
    #poi quelli della seconda
    for entry in os.scandir(sys.argv[2]):
        if entry.is_dir() == False:
            found = False
            for names in name:
                #se esiste gia un file con quel nome
                if entry.name == names[0]:
                    found = True
                    info1 = os.stat(entry.path)
                    info2 = os.stat(names[1])
                    #confronta la data di ultima modifica
                    date1 = info1.st_mtime
                    date2 = info2.st_mtime
                    if date1 > date2:
                        name.remove(names)
                        name.append([entry.name, entry.path])
                    break
            if found == False:
                name.append([entry.name, entry.path])
    #sposta tutto quello che deve spostare nella terza dir
    for entry in name:
        os.rename(entry[1], sys.argv[3] + "/" + entry[0])


if __name__ == "__main__":
    main()