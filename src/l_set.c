 /*****************************************************************
 * file: l_set.c
 * author: libaoyu
 * date: 2020-11-12 14:01:32
 * desc: 
 ******************************************************************/
#include "l_mem.h"
#include "l_lock.h"
#include "l_log.h"
#include "l_mem.h"
#include "l_type.h"
#include "l_set.h"


int l_set_init(l_set_s *list)
{
    return l_list_init(list);
}

int l_set_destroy(l_set_s *list)
{
    return l_list_destroy(list);
}

int l_set_add(l_set_s *list, void *data, size_t size)
{
    if(l_set_elt_exist(list, data, size))
    {
        return L_SUCCESS;
    }
    l_list_rpush(list, data, size);
}

int l_set_remove(l_set_s *list, int n)
{
    return l_list_remove(list, n);
}

int l_set_empty(l_set_s *list)
{
    return l_list_empty(list);
}

int l_set_elt_destroy(l_set_s *list, l_set_elt_s *elt)
{
    return l_list_elt_destroy(list, elt);
}

int l_set_elt_exist(l_set_s *list, void *data, size_t data_len)
{
    return l_list_elt_exist(list, data, data_len);
}

int l_set_elt_equals(l_set_elt_s *elt, void *data, size_t data_len)
{
    return l_list_elt_equals(elt, data, data_len);
}

l_set_elt_s *l_set_get(l_set_s *list, int n)
{
    return l_list_get(list, n);
}

l_set_elt_s *l_set_lpop(l_set_s *list)
{
    return l_list_lpop(list);
}

l_set_elt_s *l_set_rpop(l_set_s *list)
{
    return l_list_rpop(list);
}

l_set_s *l_set_slice(l_set_s *list, int m, int n)
{
    return l_list_slice(list, m, n);
}

l_set_elt_s *l_set_elt_clone(l_set_elt_s *src)
{
    return l_list_elt_clone(src);
}

void l_set_display(l_set_s *list)
{
    l_list_display(list);
}

void l_set_set_elt_free_method(l_set_s *list, int (*free_elt)(l_set_elt_s *elt))
{
    list->free_elt = free_elt;
}

void l_set_set_elt_equals_method(l_set_s *list, int (*equals)(l_set_elt_s *elt, void *data, size_t data_len))
{
    list->equals = equals;
}