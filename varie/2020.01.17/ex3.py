# Scrivere un programma python o uno script bash che consenta di lanciare parallelamente comandi
# para.py sleep 20 // ls -l // ps // sleep 3
# para.sh sleep 20 // ls -l // ps // sleep 3
# deve eseguire concorrentemente i vari comandi separati da // e aspettare la terminazione di tutti
import subprocess
import threading
import sys

def run(cmd):
    subproc = subprocess.run(cmd, shell=True, capture_output=True)
    print(str(subproc.stdout.decode("utf-8"))) #per evitare che ci sia b di byte
    #https://stackoverflow.com/questions/606191/convert-bytes-to-a-string

strConcat = ""
for i in range(1, len(sys.argv)): #nome file escluso
    strConcat += sys.argv[i] + " "
print(strConcat)
print(len(sys.argv))

threads = []
#metodo split ritorna lista
comandi = strConcat.split("//")
print(comandi, "\n")
for cmd in comandi:
   thread = threading.Thread(target=run, args=[cmd])
   thread.start()
   threads.append(thread)

for thread in threads:
    thread.join()

print("finito")
