/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *      
 * implementation of (fair) semaphores
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
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>
#include<tlist.h>
#include<semaphore.h>

#define mutex_in(X) pthread_mutex_lock(X)
#define mutex_out(X) pthread_mutex_unlock(X)

struct semaphore {
	volatile long value;
	pthread_mutex_t lock;
	struct tlist *q;
};

semaphore semaphore_create(long initval) {
	semaphore s = malloc(sizeof(*s));
	if (s) {
		s->value = initval;
		s->q = NULL;
		pthread_mutex_init(&s->lock, NULL);
	}
	return s;
}

void semaphore_destroy(semaphore s) {
	pthread_mutex_destroy(&s->lock);
	free(s);
}

void semaphore_P(semaphore s) {
	mutex_in(&s->lock);
	if (s->value <= 0) {
		tlist_enqueue(&s->q, pthread_self());
		mutex_out(&s->lock);
		suspend();
	} else {
		s->value--;
		mutex_out(&s->lock);
	}
}

void semaphore_V(semaphore s) {
	mutex_in(&s->lock);
	if (tlist_empty(s->q)) 
		s->value++;
	else
		wakeup(tlist_dequeue(&s->q));
	mutex_out(&s->lock);
}
