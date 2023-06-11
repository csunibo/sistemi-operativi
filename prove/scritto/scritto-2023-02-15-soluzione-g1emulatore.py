#!/usr/bin/env python3

# emulatore per l'esercizio c1 dello scritto 2023 02 15 (febbraio)
# scritto da Renzo Davoli in data 2023 06 10 (giugno)
# messaggio telegram: https://t.me/c/1449061520/1282
# (gruppo telegram di questo AA: https://t.me/+b8Txn35l8GY1NzAy )

import sys

N = 256
T3 = 16
s3 = 3
if len(sys.argv) > 1:
	T3 = int(sys.argv[1])
if len(sys.argv) > 2:
	s3 = int(sys.argv[2])
cpu = (N+T3) * [' ']
io = (N+T3) * [' ']
err = (N+T3) * [' ']

for i in range(0,N,2):
	cpu[i] = '1'
	io[i+1] = '1'
for i in range(1,N,4):
	cpu[i] = '2'
	io[i+1] = '2'

status = 6
for i in range(s3,N,T3):
	if status != 6:
#		print(f"ERR P3 t = {i}")
		err[i] = '*'
	else:
		err[i] = '^'
	status = 0
	for j in range(T3):
		if i + j > N:
			break
#		print(f'{i}-{j} {status} {cpu[i+j]}')
		if status == 0 and cpu[i+j] == ' ':
			status = 1
			cpu[i+j] = '3'
		elif status == 1 and cpu[i+j] == ' ':
			status = 2
			cpu[i+j] = '3'
		elif status == 2 and io[i+j] == ' ':
			status = 3
			io[i+j] = '3'
		elif status == 3 and cpu[i+j] == ' ':
			status = 4
			cpu[i+j] = '3'
		elif status == 4 and cpu[i+j] == ' ':
			status = 5
			cpu[i+j] = '3'
		elif status == 5 and io[i+j] == ' ':
			status = 6
			io[i+j] = '3'
	
print(''.join(cpu))
print(''.join(io))
print(''.join(err))
