/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *      
 * bounded buffer (using semaphores)
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
#include<semaphore.h>

#define SIZE 10
semaphore full;
semaphore empty;
volatile int buffer[SIZE];
volatile int front;
volatile int rear;

void *producer(void *arg) {
	while (1) {
		int value;
		usleep(random() % 2000000);
		value = random() % 32768;
		printf("produced: %d\n",value);
		semaphore_P(empty);
		buffer[front] = value;
		front = (front + 1) % SIZE;
		semaphore_V(full);
		printf("sent: %d\n",value);
	}
}

void *consumer(void *arg) {
	while (1) {
		int value;
		printf("\t\tconsumer ready\n");
		semaphore_P(full);
		value = buffer[rear];
		rear = (rear + 1) % SIZE;
		semaphore_V(empty);
		printf("\t\tconsume %d\n", value);
		usleep(random() % 2000000);
	}
}

int main(int argc, char *argv[]) {
	pthread_t prod_t;
	pthread_t cons_t;
	full=semaphore_create(0);
	empty=semaphore_create(SIZE);
	srandom(time(NULL));
	pthread_create(&prod_t, NULL, producer, NULL);
	pthread_create(&cons_t, NULL, consumer, NULL);
	pthread_join(prod_t, NULL);
	pthread_join(cons_t, NULL);
}

