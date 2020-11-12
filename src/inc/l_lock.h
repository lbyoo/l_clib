#ifndef __L_LOCK__
#define __L_LOCK__
#include <pthread.h>

#define L_MUTEX pthread_mutex_t
#define L_LOCK_INIT(L) pthread_mutex_init((L), NULL);
#define L_LOCK_DESTROY(L) pthread_mutex_destroy((L));
#define L_LOCK(L) pthread_mutex_lock((L));
#define L_UNLOCK(L) pthread_mutex_unlock((L));
#endif