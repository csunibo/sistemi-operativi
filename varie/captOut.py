import subprocess
cmd = "echo ciao"
p = subprocess.run(cmd, shell=True, capture_output=True)  #p.stdout/stderr
print(p.stdout.decode("utf-8") )
