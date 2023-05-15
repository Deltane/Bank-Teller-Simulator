#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "macros.h"
#include "parameters.h"
#include "teller.h"
#include "customer.h"

void* teller(void* arg) {
    /* Thread routine to serve customers in the queue */
    Parameters* params = (Parameters*) arg;
    Queue* queue = params->queue;
    int teller_id, total_served, i;
    struct tm start_time;
    struct tm finish_time;
    time_t ltime; /* Temp time value holder */

    pthread_mutex_lock(&params->totals->lock);
    if (params->totals->num_tellers < 4) {
        /* Set teller_id and update number of tellers*/
        params->totals->num_tellers++;
        teller_id = params->totals->num_tellers;
        pthread_mutex_unlock(&params->totals->lock);

        /* Store start time */
        ltime = time(NULL);
        localtime_r(&ltime, &start_time);
    } else {
        /* Terminate if there are already four tellers */
        pthread_mutex_unlock(&params->totals->lock);
        printf("Too many tellers already.\n");
        return NULL;
    }
    
    pthread_mutex_lock(&queue->lock);
    while (TRUE) {
        /* If customers still incoming, wait for signal and then recheck length of queue */
        if (queue->incoming) {
            pthread_cond_wait(&queue->new_customer, &queue->lock);
            if (queue->list->length == 0) break;
        } else { /* Else keep serving customers until the queue it empty */
            if (queue->list->length == 0) break;
        }

        /* Pop last customer from queue and signal not_full */
        customer_t* customer = (customer_t*) removeLast(queue->list);
        ltime = time(NULL);
        localtime_r(&ltime, &customer->response);
        pthread_cond_signal(&queue->not_full);
        pthread_mutex_unlock(&queue->lock);

        /* Log repsonse time of customer */
        LOG(params->logfile, "Teller: %i\nCustomer: %i\nArrival time: %02d:%02d:%02d\nResponse Time: %02d:%02d:%02d\n",
            teller_id,
            customer->n,
            customer->arrival.tm_hour, customer->arrival.tm_min, customer->arrival.tm_sec,
            customer->response.tm_hour, customer->response.tm_min, customer->response.tm_sec);

        /* Serve customer */
        teller_serve(params, teller_id, customer);
        
        /* Store finish time */
        ltime = time(NULL);
        localtime_r(&ltime, &customer->finish);

        /* Log finish time of customer */
        LOG(params->logfile, "Teller: %i\nCustomer: %i\nArrival time: %02d:%02d:%02d\nFinish Time: %02d:%02d:%02d\n",
            teller_id,
            customer->n,
            customer->arrival.tm_hour, customer->arrival.tm_min, customer->arrival.tm_sec,
            customer->finish.tm_hour, customer->finish.tm_min, customer->finish.tm_sec);

        free(customer);

        /* Update total customers served */
        increment_tallies(params->totals, teller_id);

        pthread_mutex_lock(&queue->lock);
    }
    /* Terminate if there are no more incoming customers */
    pthread_mutex_unlock(&queue->lock);
    pthread_cond_signal(&queue->new_customer); /* Unblock other tellers */

    /* Store finish time */
    ltime = time(NULL);
    localtime_r(&ltime, &finish_time);

    pthread_mutex_lock(&params->totals->lock);

    /* Log final stats of teller */
    LOG(params->logfile, "Termination: teller-%i\n#served customers: %i\nStart time: %02d:%02d:%02d\nFinish Time: %02d:%02d:%02d\n",
        teller_id,
        params->totals->tallies[teller_id-1],
        start_time.tm_hour, start_time.tm_min, start_time.tm_sec,
        finish_time.tm_hour, finish_time.tm_min, finish_time.tm_sec);

    params->totals->num_tellers -= 1;

    /* If last teller, log final stats of all tellers */
    if (params->totals->num_tellers == 0) {
        LOG(params->logfile, "\nTeller Statistics\n");
        total_served = 0;
        for (i = 0; i<4; i++) {
            LOG(params->logfile, "Teller-%i served %i customers.\n", i+1, params->totals->tallies[i]);
            total_served += params->totals->tallies[i];
        }
        LOG(params->logfile, "\nTotal number of customers: %i customers.\n", total_served);
    }

    pthread_mutex_unlock(&params->totals->lock);

    return NULL;
}

void teller_serve(Parameters* params, int teller_id, customer_t* customer) {
    /* Sleeps for the desired duration depending on customer type */
    switch (customer->type) {
        case 'W':
            sleep(params->tw);
            break;
        case 'D':
            sleep(params->td);
            break;
        case 'I':
            sleep(params->ti);
            break;
        default:
            break;
    }
}

void increment_tallies(TellerTotals* totals, int teller_id) {
    /* Increments the teller's total customers served */
    pthread_mutex_lock(&totals->lock);

    totals->tallies[teller_id - 1] += 1;

    pthread_mutex_unlock(&totals->lock);
}
