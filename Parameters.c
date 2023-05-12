#include <stdlib.h>
#include <time.h>
#include "parameters.h"

TellerTotals* initTellerTotals() {
    /* Constructor function */
    TellerTotals* totals = (TellerTotals*) malloc(sizeof(TellerTotals));
    int i;

    totals->num_tellers = 0;

    /* Initialise array of teller totals to [0, 0, 0, 0] */
    for (i=0; i<4; i++) {
        totals->tallies[i] = 0;
    }

    pthread_mutex_init(&totals->lock, NULL);

    return totals;
}

void freeTellerTotals(TellerTotals* totals) {
    /* Destructor function */
    pthread_mutex_destroy(&totals->lock);
    free(totals);
}

LogFile* openLogFile(char* filename) {
    /* LogFile constructor function */
    LogFile* logfile = (LogFile*) malloc(sizeof(LogFile));
    
    /* Opens logfile in write mode without buffering */
    logfile->fd = fopen(filename, "w+");
    setbuf(logfile->fd, NULL);
    
    pthread_mutex_init(&logfile->lock, NULL);

    return logfile;
}

void closeLogFile(LogFile* logfile) {
    /* Destructor function */
    fclose(logfile->fd);

    pthread_mutex_destroy(&logfile->lock);

    free(logfile);
}
