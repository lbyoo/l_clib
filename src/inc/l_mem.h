#ifndef __L_MEM_H__
#define __L_MEM_H__
#include <stdio.h>

#define L_MEMLEAK_CHECK 0

#define L_MALLOC(P,T,N) (P) = malloc(sizeof(T) * N); if(L_MEMLEAK_CHECK){L_LOG_DEBUG("malloc:%p\n",P);}
#define L_FREE(P) if((P)){if(L_MEMLEAK_CHECK){L_LOG_DEBUG("free: %p\n", P);}free(P);P=0;}

#endif