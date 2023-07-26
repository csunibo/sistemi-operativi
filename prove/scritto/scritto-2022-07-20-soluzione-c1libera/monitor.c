/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *      
 * implementation of monitors
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
#include<monitor.h>
#include<tlist.h>

#define mutex_in(X) pthread_mutex_lock(X)
#define mutex_out(X) pthread_mutex_unlock(X)

struct monitor {
	pthread_mutex_t lock;
	struct tlist *urgent;
};

typedef struct monitor *monitor;

struct condition {
	monitor m;
	struct tlist *q;
};

typedef struct condition *condition;

monitor monitor_create(void) {
	monitor m = malloc(sizeof(*m));
	if (m) {
		pthread_mutex_init(&m->lock, NULL);
		m->urgent = NULL;
	}
	return m;
}

void monitor_destroy(monitor m) {
	pthread_mutex_destroy(&m->lock);
	free(m);
}

void monitor_enter(monitor m) {
	//printf("enter %p\n",m);
	mutex_in(&m->lock);
}

void monitor_exit(monitor m) {
	//printf("exit %p\n",m);
	if (!tlist_empty(m->urgent)) {
		pthread_t t = tlist_pop(&m->urgent);
		wakeup(t);
	} else
		mutex_out(&m->lock);
}

condition condition_create(monitor m) {
	condition c = malloc(sizeof(*c));
	if (c) {
		c->m = m;
		c->q = NULL;
	}
	return c;
}

void condition_destroy(condition c) {
	free(c);
}

void condition_wait(condition c) {
	//printf("wait %p\n",c);
	tlist_enqueue(&c->q, pthread_self());
	monitor_exit(c->m);
	suspend();
}

void condition_signal(condition c) {
	//printf("signal %p\n",c);
	if (!tlist_empty(c->q)) {
		monitor m = c->m;
		pthread_t t = tlist_dequeue(&c->q);
		tlist_push(&m->urgent, pthread_self());
		wakeup(t);
		suspend();
	}
}


