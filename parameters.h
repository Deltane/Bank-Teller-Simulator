#ifndef PARAMS_H
#define PARAMS_H
#include <stdio.h>
#include "queue.h"

/* Contains teller total data and a mutex lock */
typedef struct {
	int num_tellers;
    int tallies[4];
    pthread_mutex_t lock;
} TellerTotals;

/* Constructor and destructor functions */
TellerTotals* initTellerTotals(void);
void freeTellerTotals(TellerTotals* totals);

/* Wrapper for file stream with a mutex lock*/
typedef struct {
    FILE* fd;
    pthread_mutex_t lock;
} LogFile;

/* Constructor and destructor functions */
LogFile* openLogFile(char* filename);
void closeLogFile(LogFile* log);

/* Parameter data and pointed shared memory that is passed to all threads */
typedef struct {
    const int m;
    const int tc;
    const int td;
    const int tw;
    const int ti;
    Queue* queue;
    TellerTotals* totals;
    LogFile* logfile;
} Parameters;

#endif
