#make with no arguments executes the first rule in the file
CFLAGS = -I../davolib/ -ldl
dummy: lancia clean
lancia: hw.so davolib.o ex1.c hw #By putting the object files in the dependency list and in the rule, make knows it must first compile the .c versions individually, and then build the executable 
	$(CC) -o lancia ex1.c davolib.o $(CFLAGS) #linkaggio sempre in fondo
	#dependency list => chiama etichette e se modificati ricompila solo quel file modificato
hw.so: hw.c #dependencies 
	$(CC) --shared -o hw.so hw.c
hw: hw.c
	$(CC) -o hw hw.c
davolib.o: ../davolib/davolib.c
	$(CC) -c -o davolib.o ../davolib/davolib.c #-c => relocatable .o files => no main() in davolib.c
clean:
	rm -f *.out *.o
fine:
	rm -f *.out *.o *.so lancia hw