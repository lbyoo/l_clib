#include "l_list.h"
#include "l_log.h"
#include "l_type.h"
#include "l_mem.h"
#include <time.h>

int test_remove(l_list_s *list)
{

    l_list_lpush(list, "hello1", 6);
    l_list_lpush(list, "hello2", 6);
    l_list_lpush(list, "hello3", 6);
    L_LOG_DEBUG("before remove\n");
    l_list_display(list);
    l_list_remove(list, 1);
    L_LOG_DEBUG("after remove\n");
    l_list_display(list);

    return 1;

}
int test_push_insert(l_list_s *list)
{

    l_list_lpush(list, "hello1", 6);
    l_list_lpush(list, "hello2", 6);
    l_list_lpush(list, "hello3", 6);
    l_list_insert(list, 3, "hello4", 6);
    l_list_display(list);

    return 1;
}
int test_get(l_list_s *list)
{

    l_list_lpush(list, "hello1", 6);
    
    // l_list_lpush(list, "hello3", 6);
    // l_list_insert(list, 3, "hello4", 6);
    l_list_elt_s *elt = l_list_get(list, 0);
    L_LOG_DEBUG("%s\n",(char *)elt->data);
    L_ASSERT(strcmp(elt->data, "hello1") == 0);
    elt = l_list_get(list, 1);
    l_list_lpush(list, "hello2", 6);
    elt = l_list_get(list, 0);
    L_LOG_DEBUG("%s\n",(char *)elt->data);
    elt = l_list_get(list, 1);
    L_LOG_DEBUG("%s\n",(char *)elt->data);

    return 1;
}
void test_pop(l_list_s *list)
{

    l_list_lpush(list, "hello1", 6);
    L_LOG_INFO("before pop\n");
    l_list_display(list);
    l_list_elt_s *elt = l_list_lpop(list);
    L_LOG_DEBUG("%s\n",(char *)elt->data);
    L_LOG_INFO("after pop\n");
    l_list_display(list);

    l_list_lpush(list, "hello1", 6);
    l_list_lpush(list, "hello2", 6);
    l_list_lpush(list, "hello3", 6);
    L_LOG_INFO("before pop\n");
    l_list_display(list);
    elt = l_list_lpop(list);
    L_LOG_DEBUG("pop %s\n",(char *)elt->data);
    l_list_elt_destroy(list, elt);
    L_LOG_INFO("after pop\n");
    l_list_display(list);
    elt = l_list_lpop(list);
    L_LOG_DEBUG("pop %s\n",(char *)elt->data);
    l_list_elt_destroy(list, elt);
    L_LOG_INFO("after pop\n");
    l_list_display(list);
    elt = l_list_lpop(list);
    L_LOG_DEBUG("pop %s\n",(char *)elt->data);
    l_list_elt_destroy(list, elt);
    L_LOG_INFO("after pop\n");
    l_list_display(list);
    elt = l_list_lpop(list);
    L_ASSERT(elt == L_NULL);

    l_list_lpush(list, "hello1", 6);
    l_list_lpush(list, "hello2", 6);
    l_list_lpush(list, "hello3", 6);
    L_LOG_INFO("before pop\n");
    l_list_display(list);
    elt = l_list_rpop(list);
    L_LOG_DEBUG("pop %s\n",(char *)elt->data);
    l_list_elt_destroy(list, elt);
    L_LOG_INFO("after pop\n");
    l_list_display(list);
    elt = l_list_rpop(list);
    L_LOG_DEBUG("pop %s\n",(char *)elt->data);
    l_list_elt_destroy(list, elt);
    L_LOG_INFO("after pop\n");
    l_list_display(list);
    elt = l_list_rpop(list);
    L_LOG_DEBUG("pop %s\n",(char *)elt->data);
    l_list_elt_destroy(list, elt);
    L_LOG_INFO("after pop\n");
    l_list_display(list);
    elt = l_list_rpop(list);
    L_ASSERT(elt == L_NULL);
    
    l_list_destroy(list);

}
void test_slice(l_list_s *list)
{
    
    
    l_list_lpush(list, "hello1", 6);

    
    l_list_s *dest;
    L_LOG_INFO("list:\n");
    l_list_display(list);
    dest = l_list_slice(list, 0, 0);
    L_LOG_INFO("dest:\n");
    l_list_display(dest);
    l_list_destroy(dest);
    L_FREE(dest);

    l_list_rpush(list, "hello2", 6);
    L_LOG_INFO("list:\n");
    l_list_display(list);
    dest = l_list_slice(list, 0, 0);
    L_LOG_INFO("dest1:\n");
    l_list_display(dest);
    l_list_destroy(dest);
    L_FREE(dest);
    
    dest = l_list_slice(list, 0, 1);
    L_LOG_INFO("dest2:\n");
    l_list_display(dest);
    l_list_destroy(dest);
    L_FREE(dest);

    dest = l_list_slice(list, 1, 1);
    L_LOG_INFO("dest3:\n");
    l_list_display(dest);
    l_list_destroy(dest);
    L_FREE(dest);

    l_list_rpush(list, "hello3", 6);
    L_LOG_INFO("list:\n");
    l_list_display(list);

    dest = l_list_slice(list, 2, 2);
    L_LOG_INFO("dest4:\n");
    l_list_display(dest);
    l_list_destroy(dest);
    L_FREE(dest);
    
    
}

void test_exists(l_list_s *list)
{
    l_list_lpush(list, "hello1", 6);
    l_list_lpush(list, "hello2", 6);
    l_list_lpush(list, "hello3", 6);
    l_list_lpush(list, "hello4", 6);
    char *a = "hello3";
    if(l_list_elt_exist(list, a, 6) == L_TRUE)
    {
        L_LOG_INFO("exists1 test ok\n");
    }
    else{
        L_LOG_INFO("exists1 test fail\n");
    }
    if(l_list_elt_exist(list, a, 5) == L_FALSE)
    {
        L_LOG_INFO("exists2 test ok\n");
    }
    else{
        L_LOG_INFO("exists2 test fail\n");
    }
    if(l_list_elt_exist(list, "HELLO", 6) == L_FALSE)
    {
        L_LOG_INFO("exists3 test ok\n");
    }
    else{
        L_LOG_INFO("exists3 test fail\n");
    }
}

void test_bigdata(l_list_s *list)
{
    char *s;
    for(int i = 0; i < 100000; i ++)
    {
        
        L_MALLOC(s, char, 20);
        sprintf(s, "hello%d", i);
        l_list_rpush(list, s, strlen(s));
    }
    L_LOG_INFO("list size if %lu\n", list->size);
}
L_LIST_FREE_ELT_FUNC(test)
{
    // L_LOG_INFO("free mem %p\n", elt);
    L_FREE(elt->data);
}

L_LIST_ELT_EQUALS_FUNC(test)
{
    return L_FALSE;
}

int main(int argv, char *args)
{
    l_list_s list;
    l_list_init(&list);
    l_list_set_elt_free_method(&list, L_LIST_FREE_ELT_FUNC_NAME(test));
    l_list_set_elt_equals_method(&list, L_LIST_ELT_EQUALS_FUNC_NAME(test));
    // test_push_insert(&list);
    // l_list_empty(&list);
    // test_remove(&list);
    // l_list_empty(&list);
    // test_get(&list);
    // l_list_empty(&list);
    // test_pop(&list);
    // l_list_empty(&list);
    // test_slice(&list);
    // test_exists(&list);
    test_bigdata(&list);
    getchar();
    l_list_destroy(&list);
}