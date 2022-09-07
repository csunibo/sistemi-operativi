/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *      
 * bounded buffer (using monitors)
 *        
 * sm is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *             
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 *                   
 */                   

#include<stdio.h>
#include<pthread.h>
#include<monitor.h>

#define SIZE 3
monitor bb;
condition ok2write;
condition ok2read;

volatile int buffer[SIZE];
volatile int front;
volatile int rear;
volatile int buflen;

void bb_create(void) {
	bb = monitor_create();
	ok2write = condition_create(bb);
	ok2read = condition_create(bb);
}

void bb_put(int value) {
	monitor_enter(bb);
	if (buflen >= SIZE)
		condition_wait(ok2write);
	buffer[front] = value;
	front = (front + 1) % SIZE;
	buflen++;
	condition_signal(ok2read);
	monitor_exit(bb);
}

int bb_get(void) {
	int rval;
	monitor_enter(bb);
	if (buflen <= 0)
		condition_wait(ok2read);
	rval = buffer[rear];
	rear = (rear + 1) % SIZE;
	buflen--;
	condition_signal(ok2write);
	monitor_exit(bb);
	return rval;
}

void *producer(void *arg) {
	while (1) {
		int value;
		usleep(random() % 2000000);
		value = random() % 32768;
		printf("produced: %d\n",value);
		bb_put(value);
		printf("sent: %d\n",value);
	}
}

void *consumer(void *arg) {
	while (1) {
		int value;
		printf("\t\tconsumer ready\n");
		value = bb_get();
		printf("\t\tconsume %d\n", value);
		usleep(random() % 2000000);
	}
}

int main(int argc, char *argv[]) {
	pthread_t prod_t;
	pthread_t cons_t;
	bb_create();
	srandom(time(NULL));
	pthread_create(&prod_t, NULL, producer, NULL);
	pthread_create(&cons_t, NULL, consumer, NULL);
	pthread_join(prod_t, NULL);
	pthread_join(cons_t, NULL);
}

