# Svolgere l'esercizio 2 in bash o in python (ovviamente scrivendo la logica risolutiva in bash o in python, senza
# eseguire un programma scritto C, per esempio quello dell'esercizio 2)
import threading, subprocess, os, pathlib, time, sys

folder = input("type in folder path: ")
files = [f.path for f in os.scandir(folder) if f.is_file()]
counted_files = []
timers = []


for f in files:
    filename, fileext = os.path.splitext(f)
    if fileext[1:].isdigit():  # rimuoviamo il punto iniziale
        counted_files.append(f)
        timers.append(int(fileext[1:]))

current_time = 0
time_to_wait = 0

timers.sort()

for t in timers:
    time_to_wait = t - current_time
    for f in counted_files:
        filename, fileext = os.path.splitext(f)
        if fileext[1:] == str(t):
            # thread = threading.Thread(target=execute, args=[f, time_to_wait])
            # # thread.daemon = True
            # thread.start()
            pid = os.fork()
            if pid == 0:
                time.sleep(time_to_wait / 1000)
                subprocess.run(f, shell=True, capture_output=False)
                exit()

    if t == timers[len(timers) - 1]:
        print("Ora esco ciao")
        sys.exit()
        print("STILL ALIVE!")
    time.sleep(time_to_wait / 1000)
    current_time += time_to_wait
