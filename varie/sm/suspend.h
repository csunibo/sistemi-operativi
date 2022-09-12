#ifndef SUSPEND_H
#define SUSPEND_H
#include<pthread.h>

void suspend(void);

void wakeup(pthread_t thread);

__attribute__((constructor)) void _suspend_init();

#endif // SUSPEND_H
