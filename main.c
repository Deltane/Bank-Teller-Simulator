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
	Queue* c_queue = createQueue();
	TellerTotals* totals = initTellerTotals();
	LogFile* logfile = openLogFile("r_log");
	/* Get parameters */
	Parameters params = {
		.m = atoi(argv[1]), 
		.tc = atoi(argv[2]),
		.tw = atoi(argv[3]),
		.td = atoi(argv[4]),
		.ti = atoi(argv[5]),
		.queue = c_queue, 
		.totals = totals,
		.logfile = logfile
	};

	pthread_t Customer_th;
	pthread_t teller_th[4];
	int i;

	/* Create threads */
	printf("Starting threads...\n");
	pthread_create(&Customer_th, NULL, &customer, &params);
	for (i=0; i<4; i++) {
		pthread_create(&teller_th[i], NULL, &Teller, &params);
	}
	
	/* Join threads */
	pthread_join(Customer_th, NULL);
	for (i=0; i<4; i++) {
		pthread_join(teller_th[i], NULL);
	}
	printf("Threads joined...\n");

	/* Free memory */
	freeQueue(c_queue, &free);
	freeTellerTotals(params.totals);
	closeLogFile(params.logfile);
	return 0;
}
