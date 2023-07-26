CFLAGS=-I.
LDLIBS=-lpthread
ALL= esercizio

all: $(ALL)

esercizio: esercizio.o monitor.o suspend.o tlist.o

monitor.o: monitor.c suspend.h tlist.h

semaphore.o: semaphore.c suspend.h tlist.h

suspend.o: suspend.c

tlist.o: tlist.c

clean:
	rm -f *.o $(ALL)
