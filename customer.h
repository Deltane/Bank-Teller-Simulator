#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "queue.h"
#include <time.h>

/* Customer data */
typedef struct {
    int n;
    char type;
    struct tm arrival;
    struct tm response;
    struct tm finish;
} customer_t;

/* Customer methods */
customer_t* create_customer();
void print_Customer(void* customer);

/* Thread function */
void* customer(void* queue);

#endif
