#ifndef TELLER_H
#define TELLER_H

#include "parameters.h"
#include "customer.h"

/* Thread function */
void* Teller (void* arg);
void teller_serve(Parameters* params, int teller_id, customer_t* customer);
void increment_tallies(TellerTotals* totals, int teller_id);

#endif
