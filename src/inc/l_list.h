#ifndef __L_LIST__
#define __L_LIST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** 用于定义元素释放的函数 */
#define L_LIST_FREE_ELT_FUNC_NAME(FUNC) l_list_free_elt_##FUNC
#define L_LIST_FREE_ELT_FUNC(FUNC) int l_list_free_elt_##FUNC(l_list_elt_s *elt)

typedef struct l_list_elt l_list_elt_s;
typedef struct l_list 
{
    l_list_elt_s *first;
    l_list_elt_s *current;
    l_list_elt_s *last;
    int size;
    pthread_mutex_t lock;
    int (*free_elt)(l_list_elt_s *elt);
    
} l_list_s;

typedef struct l_list_elt
{
   l_list_elt_s *pre;
   l_list_elt_s *next;
   void *data; 
   int data_len;

} l_list_elt_s;

extern int l_list_init(l_list_s *list);
extern int l_list_destroy(l_list_s *list);
extern int l_list_lpush(l_list_s *list, void *data, size_t size);
extern int l_list_rpush(l_list_s *list, void *data, size_t size);
extern int l_list_insert(l_list_s *list, int n, void *data, size_t size);
extern int l_list_remove(l_list_s *list, int n);
extern int l_list_empty(l_list_s *list);
extern int l_list_elt_destroy(l_list_s *list, l_list_elt_s *elt);
extern l_list_elt_s *l_list_get(l_list_s *list, int n);
extern l_list_elt_s *l_list_lpop(l_list_s *list);
extern l_list_elt_s *l_list_rpop(l_list_s *list);
extern l_list_s *l_list_slice(l_list_s *list, int m, int n);
extern l_list_elt_s *l_list_elt_clone(l_list_elt_s *src);
extern void l_list_display(l_list_s *list);
extern void l_list_set_elt_free_method(l_list_s *list, int (*free_elt)(l_list_elt_s *elt));

#endif