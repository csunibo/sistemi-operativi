CFLAGS=-I.
LDLIBS=-lpthread
ALL= prod_cons bounded_buf philo bounded_buf_mon

all: $(ALL)

bounded_buf_mon: bounded_buf_mon.o monitor.o suspend.o tlist.o

bounded_buf: bounded_buf.o semaphore.o suspend.o tlist.o

bouded_buf_mon.o: bounded_buf_mon.c monitor.h

bouded_buf.o: bounded_buf.c semaphore.h

monitor.o: monitor.c suspend.h tlist.h

semaphore.o: semaphore.c suspend.h tlist.h

suspend.o: suspend.c

tlist.o: tlist.c

clean:
	rm -f *.o $(ALL)
