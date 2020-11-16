#ifndef __L_LOG_H__
#define __L_LOG_H__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#define L_LOG_DEBUG(format, ...) \
{ \
    time_t t = time(0); \
    struct tm ptm; \
    memset(&ptm,0, sizeof(ptm)); \
    localtime_r(&t, &ptm); \
    fprintf(stdout, "[ DEBUG ] [%4d-%02d-%02d %02d:%02d:%02d ] [ %s:%s:%d ] " format "", \
    ptm.tm_year + 1900, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour, ptm.tm_min, ptm.tm_sec,\
    __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
}

#define L_LOG_INFO(format, ...) \
{ \
    time_t t = time(0); \
    struct tm ptm; \
    memset(&ptm,0, sizeof(ptm)); \
    localtime_r(&t, &ptm); \
    fprintf(stdout, "[ INFO ] [%4d-%02d-%02d %02d:%02d:%02d ] [ %s:%s:%d ] " format "", \
    ptm.tm_year + 1900, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour, ptm.tm_min, ptm.tm_sec, \
    __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
}

#define L_LOG_ERROR(format, ...) \
{ \
    time_t t = time(0); \
    struct tm ptm; \
    memset(&ptm,0, sizeof(ptm)); \
    localtime_r(&t, &ptm); \
    fprintf(stderr, "[ ERROR ] [%4d-%02d-%02d %02d:%02d:%02d ] [ %s:%s:%d ] " format "", \
    ptm.tm_year + 1900, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour, ptm.tm_min, ptm.tm_sec, \
    __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
}


#define L_ASSERT(P) if(!(P)){L_LOG_DEBUG("Assert fail\n"); exit(-1);}

#endif