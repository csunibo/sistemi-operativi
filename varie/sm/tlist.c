/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *      
 * lists of thread_id using circular linked lists.
 * (this module implements both abstractions: stack and queue).
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

#include<pthread.h>
#include<stdlib.h>
#include<tlist.h>

struct tlist {
	struct tlist *next;
	pthread_t thread;
};

void tlist_enqueue(struct tlist **tp, pthread_t thread) {
	struct tlist *new = malloc(sizeof(*new));
	if (new) {
		new->thread = thread;
		if (*tp) {
			struct tlist *last = *tp;
			new->next = last->next;
			last->next = new;
		} else 
			new->next = new;
		*tp = new;
	}
}

void tlist_push(struct tlist **tp, pthread_t thread) {
	struct tlist *new = malloc(sizeof(*new));
	if (new) {
		new->thread = thread;
		if (*tp) {
			struct tlist *last = *tp;
			new->next = last->next;
			last->next = new;
		} else {
			new->next = new;
			*tp = new;
		}
	}
}

static pthread_t tlist_get(struct tlist **tp) {
	if (*tp) {
		struct tlist *last = *tp;
		struct tlist *first = last->next;
		pthread_t rval = first->thread;
		if (first == first->next)
			*tp = NULL;
		else
			last->next = first->next;
		free(first);
		return rval;
	} else
		return -1;
}

pthread_t tlist_dequeue(struct tlist **tp) {
	return tlist_get(tp);
}

pthread_t tlist_pop(struct tlist **tp) {
	return tlist_get(tp);
}

