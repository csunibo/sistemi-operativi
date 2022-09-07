import subprocess

def main():
    while True:
        try:
            cmd = (input("> "))
        except:
            #se riceve ^D termina
            print("\n")
            break 
        #se fa logout termina
        if (cmd == "logout"):
            break
        #separa gli argomenti passati per parametro
        argv = cmd.split(" ")
        try:
            subprocess.run(argv, shell=False)
        except:
            #se non puo' essere eseguito avverte (ad esempio cd)
            print("Not permitted\n")

if __name__ == "__main__":
    main()