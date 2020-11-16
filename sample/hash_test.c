#include "l_set.h"
#include "l_log.h"
#include "l_type.h"
#include "l_mem.h"
#include "l_hashtable.h"
L_HASHTABLE_ENUM_CALLBACK(test)
{
    printf("--->%d, %s, %s\n", entry->hashcode, entry->key, (char*)entry->value);
}

void test1(l_hashtable_s *hash)
{
    l_hashtable_push(hash, "liby","good", 5);
    l_hashtable_push(hash, "zhangguoxing","hello", 6);
    l_hashtable_push(hash, "bozhiguo","good", 5);
    l_hashtable_push(hash, "wanggeng","hello", 6);
    l_hashtable_enum(hash, L_NULL);
    l_hashtable_entry_s *p = l_hashtable_get(hash, "bozhiguo");
    l_hashtable_entry_s *found;
    if(l_hashtable_exist(hash, "zhangguoxing", &found) == L_TRUE)
    {
        L_LOG_INFO("exist ok\n");
    }
    l_hashtable_remove(hash, "zhangguoxing");
    L_LOG_INFO("%s\n", (char*)p->value);
    l_hashtable_enum(hash, L_NULL);
    if(l_hashtable_exist(hash, "zhangguoxing", &found) == L_FALSE)
    {
        L_LOG_INFO("no exist ok\n");
    }
    l_hashtable_enum(hash, L_HASHTABLE_ENUM_CALLBACK_NAME(test));
}


int main(int argv, char **args)
{
    l_hashtable_s hash;
    l_hashtable_init(&hash);
    
    test1(&hash);
    l_hashtable_destroy(&hash);

}