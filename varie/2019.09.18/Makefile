CFLAGS=-I. -g -lpthread
LDLIBS=-lpthread
ALL= paran

all: $(ALL)

paran: ex1.o davolib.o monitor.o suspend.o tlist.o
	$(CC) -o paran ex1.o davolib.o monitor.o suspend.o tlist.o $(CFLAGS)



ex1.o: ex1.c davolib.h monitor.h

davolib.o: davolib.c
	$(CC) -c -o davolib.o davolib.c #-c => relocatable .o files => no main() in davolib.c

monitor.o: monitor.c suspend.h tlist.h

suspend.o: suspend.c

tlist.o: tlist.c

clean:
	rm -f *.o $(ALL)
