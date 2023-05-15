#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "macros.h"
#include "parameters.h"
#include "customer.h"

void* customer(void* arg) {
	/* Thread routine to add customers to queue */
	Parameters* parameters = (Parameters*) arg;
	Queue* queue = parameters->queue;
	customer_t* customer = NULL;
	int push_customer = FALSE;
	const char* linebreak = "-----------------------------------------------------------------------\n";
	time_t ltime;

	/* Open customer file */
	FILE* file = fopen(C_FILE, "r");

	if (file == NULL ) {
		perror("Error reading file.\n");
	} else {
		while(!feof(file)) {
			/* For every line in the file, read into a Customer struct */
			customer = create_customer();
			fscanf(file, "%i %c ", &customer->n, &customer->type);

			/* Sleep for specified time interval */
			sleep(parameters->tc);

			/* Push customer to Queue */
			push_customer = FALSE;
			pthread_mutex_lock(&queue->lock);
			while (!push_customer) {
				if (queue->list->length < parameters->m) { /* Check Queue has room for one more */
					insertStart(queue->list, customer);
					ltime = time(NULL);
					localtime_r(&ltime, &customer->arrival);
					push_customer = TRUE;

					/* Print arrival time string to logfile */
					LOG(parameters->logfile, "%s%i: %c\nArrival time: %02d:%02d:%02d\n%s",
						linebreak,
						customer->n, customer->type,
						customer->arrival.tm_hour, customer->arrival.tm_min, customer->arrival.tm_sec,
						linebreak);

				} else {
					/* Block thread if there is no space */
					pthread_cond_wait(&queue->not_full, &queue->lock);
				}
			}
			pthread_mutex_unlock(&queue->lock);

			/* Unblock a teller to serve the customer by signalling */
			pthread_cond_signal(&queue->new_customer);

		}
		/* End of file, no more incoming customer */
		pthread_mutex_lock(&queue->lock);
		queue->incoming = FALSE; /* Set incoming flag to false so tellers terminate */
		pthread_mutex_unlock(&queue->lock);
	
		fclose(file);
	}

	return NULL;
}

customer_t* create_customer() {
	/* Constructor function */
	customer_t* customer = (customer_t*) malloc(sizeof(customer_t));
	
	/* Set default values */
	customer->n = 0;
	customer->type = ' ';

	return customer;
}

void print_customer(void* customer) {
	/* Standard printer function used by printLinkedList */
	customer_t* c = (customer_t*)customer;
	printf("#%i: '%c'\n", c->n, c->type);

	return;
}
