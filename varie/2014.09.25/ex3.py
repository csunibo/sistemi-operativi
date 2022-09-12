# Scrivere un programma python o uno scrip bash che faccia un backup storico di un file.
#  backn file n
# deve mantenere n versioni del file specificato. n>2
#  Esempio:
# backn miofile 10
# se esiste miofile.9 deve essere rinominato in miofile.10
# se esiste miofile 8 deve essere rinominato in miofile 9
# e cosi' via fino a miofile.2, rinominato miofile 3.
# ora se miofile.1 ha lo stesso contenuto di miofile.3, miofile diventa un link fisico a miofile.2
# miofile viene copiato in miofile.1
import sys, os, shutil

if len(sys.argv) < 3:
    print("Expected 2 parameters, first is the file, second is the number of version ")
    exit()
elif sys.argv[2] < 2:
    print("The second parameter must be greater than 2")
    exit()

for i in range(int(sys.argv[2]), 2, -1):  # da n a 2+1 inclusi
    pathName = sys.argv[1] + "." + i - 1
    newPathName = sys.argv[1] + "." + i
    if os.path.exists(pathName):
        os.rename(pathName, newPathName)
    else:
        shutil.copy(sys.argv[2], newPathName)
