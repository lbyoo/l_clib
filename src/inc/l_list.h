#ifndef __L_LIST__
#define __L_LIST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "l_lock.h"

/** 用于定义元素释放的函数,函数中不要释放 elt本身，只需要处理elt->data对应的数据 */
#define L_LIST_FREE_ELT_FUNC_NAME(FUNC) l_list_free_elt_##FUNC
#define L_LIST_FREE_ELT_FUNC(FUNC) int l_list_free_elt_##FUNC(l_list_elt_s *elt)

#define L_LIST_ELT_EQUALS_FUNC_NAME(FUNC) l_list_elt_equals_##FUNC
#define L_LIST_ELT_EQUALS_FUNC(FUNC) int l_list_elt_equals_##FUNC(l_list_elt_s *elt, void *data, size_t data_len)

typedef struct l_list_elt l_list_elt_s;
typedef struct l_list 
{
    l_list_elt_s *first;
    l_list_elt_s *current;
    l_list_elt_s *last;
    size_t size;
    L_MUTEX lock;
    int (*free_elt)(l_list_elt_s *elt);
    int (*equals)(l_list_elt_s *elt, void *data, size_t data_len);
    
} l_list_s;

typedef struct l_list_elt
{
   l_list_elt_s *prev;
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
extern L_BOOL l_list_elt_exist(l_list_s *list, void *data, size_t data_len);
extern L_BOOL l_list_elt_equals(l_list_elt_s *elt, void *data, size_t data_len);
extern l_list_elt_s *l_list_get(l_list_s *list, int n);
extern l_list_elt_s *l_list_lpop(l_list_s *list);
extern l_list_elt_s *l_list_rpop(l_list_s *list);
extern l_list_s *l_list_slice(l_list_s *list, int m, int n);
extern l_list_elt_s *l_list_elt_clone(l_list_elt_s *src);
extern void l_list_display(l_list_s *list);
extern void l_list_set_elt_free_method(l_list_s *list, int (*free_elt)(l_list_elt_s *elt));
extern void l_list_set_elt_equals_method(l_list_s *list, int (*equals)(l_list_elt_s *elt, void *data, size_t data_len));

#endif