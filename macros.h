#ifndef MACROS_H
#define MACROS_H

#define TRUE 1
#define FALSE !TRUE

#define C_FILE "c_file.txt"

/* Variadic macros to write to LogFile */
#define LOG(LOGFILE, ...) {                 \
    pthread_mutex_lock(&LOGFILE->lock);     \
    fprintf(LOGFILE->fd, __VA_ARGS__);      \
    fflush(LOGFILE->fd);                    \
    pthread_mutex_unlock(&LOGFILE->lock);   \
}

#endif
