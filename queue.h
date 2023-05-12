#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include "linkedlist.h"

/* Queue is a LinkedList wrapper struct that include pthread mutex and condition types */
typedef struct {
    LinkedList* list;
    pthread_mutex_t lock;
    pthread_cond_t not_full;
    pthread_cond_t new_customer;
    int incoming;
} Queue;

Queue* createQueue();
void freeQueue(Queue* q, listFunc func);

#endif
