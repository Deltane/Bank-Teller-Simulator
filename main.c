/*
Serena McPherson
19765644
Operating Systems Assignment
10/05/2023
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "parameters.h"
#include "customer.h"
#include "teller.h"

int main (int argc, char** argv) 
{
	if (argc != 6) {
		perror("Wrong number of arguments");
		return 1;
	}

	/* Create a queue, a teller-totals struct, a logfile */
Queue* c_queue = createQueue(); // create queue for customers
TellerTotals* totals = initTellerTotals(); // create struct to keep track of tellers and their statistics
LogFile* logfile = openLogFile("r_log"); // open log file to store messages

/* Get parameters */
Parameters parameters = {
	.m = atoi(argv[1]), // max number of customers in the queue
	.tc = atoi(argv[2]), // time for a customer to be served by a teller
	.tw = atoi(argv[3]), // time for a teller to rest after serving a customer
	.td = atoi(argv[4]), // duration of a transaction between a customer and a teller
	.ti = atoi(argv[5]), // interval at which customers arrive
	.queue = c_queue, // pointer to the queue of customers
	.totals = totals, // pointer to the teller-totals struct
	.logfile = logfile // pointer to the log file
};

pthread_t customer_th; // thread for customer
pthread_t teller_th[4]; // array of threads for tellers, with 4 tellers in total
int i;

/* Create threads */
printf("Starting threads...\n");
pthread_create(&customer_th, NULL, &customer, &parameters); // create thread for customer
for (i=0; i<4; i++) { // create threads for each teller
	pthread_create(&teller_th[i], NULL, &teller, &parameters);
}

/* Join threads */
pthread_join(customer_th, NULL); // wait for customer thread to finish
for (i=0; i<4; i++) { // wait for each teller thread to finish
	pthread_join(teller_th[i], NULL);
}
printf("Threads joined...\n");

/* Free memory */
freeQueue(c_queue, &free); // deallocate memory used by the queue of customers
freeTellerTotals(parameters.totals); // deallocate memory used by the teller-totals struct
closeLogFile(parameters.logfile); // close the log file
return 0;
}
