#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>

struct semaphore;
typedef struct semaphore *semaphore;

semaphore semaphore_create(long initval);

void semaphore_destroy(semaphore s);

void semaphore_P(semaphore s);

void semaphore_V(semaphore s);

#endif // SEMAPHORE_H
