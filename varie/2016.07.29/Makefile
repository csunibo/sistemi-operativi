dummy: testpgd clean
testpgd: printgroupdir.o testpgd.c #By putting the object files in the dependency list and in the rule, make knows it must first compile the .c versions individually, and then build the executable
	$(CC) -o testpgd testpgd.c printgroupdir.o  #linkaggio sempre in fondo
	#dependency list => chiama etichette e se modificati ricompila solo quel file modificato
printgroupdir.o: printgroupdir.c
	$(CC) -c -o printgroupdir.o printgroupdir.c #-c => relocatable .o files => no main() in davolib.c
clean:
	rm -f *.out *.o *.so
fine:
	rm -f *.out *.o *.so testpgd
