#include <stdlib.h>
#include "queue.h"
#include "macros.h"

Queue* createQueue() {
    /* Initialises Queue on heap */
    Queue* q = (Queue*) malloc(sizeof(Queue));

    /* Create linked list */
    q->list = createLinkedList();
    
    /* Initailise synchronisation fields */
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->new_customer, NULL);
    q->incoming = TRUE;

    return q;
}

void freeQueue(Queue* q, listFunc func) {
    /* Queue destructor function */

    /* Free LinkedList */
    freeLinkedList(q->list, func);

    /* Destroy synchronisation variables */
    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->not_full);
    pthread_cond_destroy(&q->new_customer);
    
    free(q);
}
