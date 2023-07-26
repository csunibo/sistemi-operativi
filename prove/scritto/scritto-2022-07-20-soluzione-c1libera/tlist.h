#ifndef TLIST_H
#define TLIST_H
#include<pthread.h>

struct tlist;

void tlist_enqueue(struct tlist **tp, pthread_t thread);

void tlist_push(struct tlist **tp, pthread_t thread);

pthread_t tlist_dequeue(struct tlist **tp);

pthread_t tlist_pop(struct tlist **tp);

static inline int tlist_empty(struct tlist *tp) {
	return tp == NULL;
}
 
#endif // TLIST_H
