 /*****************************************************************
 * file: l_hashtable.c
 * author: libaoyu
 * date: 2020-11-13 10:08:24
 * desc: 
 ******************************************************************/
#include "l_mem.h"
#include "l_lock.h"
#include "l_log.h"
#include "l_mem.h"
#include "l_type.h"
#include "l_hashtable.h"

static int l_hashtable_hashcode(char *key)
{
    unsigned char *p = (unsigned char *)key;
    int rt = 0;
    for(int i=0; i<strlen(key); i++)
    {
        rt = (rt * 31 + *p) % MAX_HASHTABLE_ARRAY_SIZE;
    }
    return rt;
}
static int l_hashtable_set_key(l_hashtable_entry_s *entry, const char *key)
{
    L_MALLOC(entry->key, char, strlen(key) + 1);
    strcpy(entry->key, key);
    return L_SUCCESS;
}

static int l_hashtable_free_key(l_hashtable_entry_s *entry)
{
    L_FREE(entry->key);
    return L_SUCCESS;
}
int l_hashtable_entry_destroy(l_hashtable_s *hash, l_hashtable_entry_s *entry)
{
    L_FREE(entry->key);
    if(hash->free_value)
    {
        hash->free_value(entry);
    }
    
    
    return L_SUCCESS;
}
int l_hashtable_init(l_hashtable_s *hash)
{
    L_ASSERT(hash);
    L_LOCK_INIT(&hash->lock);
    hash->size = 0;
    for(int i=0; i<MAX_HASHTABLE_ARRAY_SIZE; i++)
    {
        l_hashtable_entry_root_s *root;
        L_MALLOC(root, l_hashtable_entry_root_s, 1);
        root->first = L_NULL;
        root->last = L_NULL;
        root->size = 0;
        hash->_array[i] = root;
    }
    hash->free_value = L_NULL;
    return L_SUCCESS;
}
int l_hashtable_destroy(l_hashtable_s *hash)
{
    l_hashtable_empty(hash);
    for(int i=0; i<MAX_HASHTABLE_ARRAY_SIZE; i++)
    {
        L_FREE(hash->_array[i]);
    }
    L_LOCK_DESTROY(&hash->lock);
    return L_SUCCESS;
}
int l_hashtable_empty(l_hashtable_s *hash)
{
    L_LOCK(&hash->lock);
    l_hashtable_entry_s *p;
    for(int i=0; i<MAX_HASHTABLE_ARRAY_SIZE; i++)
    {
        do
        {
            p = hash->_array[i]->first;
            if(p)
            {
                l_hashtable_remove(hash, p->key);
            }
        } while (p);
    }
    L_UNLOCK(&hash->lock);
    return L_SUCCESS;
}
int l_hashtable_exist(l_hashtable_s *hash, char *key, l_hashtable_entry_s **found)
{
    int pos = l_hashtable_hashcode(key);
    L_ASSERT(hash->_array[pos]);
    l_hashtable_entry_s *p = hash->_array[pos]->first;
    while(p)
    {
        if(strcmp(key, p->key) == 0)
        {
            *found = p;
            return L_TRUE;
        }
        p = p->next;
    }
    return L_FALSE;
}

int l_hashtable_push(l_hashtable_s *hash, char *key, void *value, size_t value_len)
{
    L_ASSERT(hash);
    L_ASSERT(key);
    
    l_hashtable_entry_s *p;
    L_LOCK(&hash->lock);
    if(l_hashtable_exist(hash, key, &p))
    {
        L_FREE(p->value);
        p->value = value;
        p->value_len = value_len;
        goto success;
    }

    int pos = l_hashtable_hashcode(key);
    L_ASSERT(hash->_array[pos]);
    
    l_hashtable_entry_s *entry;
    L_MALLOC(entry, l_hashtable_entry_s, 1);
    entry->hashcode = pos;
    l_hashtable_set_key(entry, key);
    entry->value = value;
    entry->value_len = value_len;
    l_hashtable_entry_s *last = hash->_array[pos]->last;
    if(last)
    {
        last->next = entry;
        entry->prev = last;
        
    }
    else
    {
        hash->_array[pos]->first = entry;
        entry->prev = L_NULL;

    }
    hash->_array[pos]->last = entry;
    hash->_array[pos]->size++;
    hash->size++;
    
    
success:
    L_UNLOCK(&hash->lock);
    return L_SUCCESS;
}


int l_hashtable_remove(l_hashtable_s *hash, char *key)
{
    L_ASSERT(hash);
    L_ASSERT(key);
    l_hashtable_entry_s *found;
    if(l_hashtable_exist(hash, key, &found))
    {
        if(found->prev)
        {
            found->prev->next = found->next;
        }else{
            hash->_array[found->hashcode]->first = found->next;
        }
        if(found->next)
        {
            found->next->prev = found->prev;
        }else{
            hash->_array[found->hashcode]->last = found->prev;
        }
        
        l_hashtable_entry_destroy(hash, found);
        hash->_array[found->hashcode]->size--;
        hash->size--;
        L_FREE(found);
    }
    return L_SUCCESS;
}

l_hashtable_entry_s *l_hashtable_get(l_hashtable_s *hash, char *key)
{
    L_ASSERT(hash);
    L_ASSERT(key);
    l_hashtable_entry_s *found;
    if(l_hashtable_exist(hash, key, &found))
    {
        return found;
    }
    return L_NULL;
}

int l_hashtable_enum(l_hashtable_s *hash, l_hashtable_enum_callback callback)
{
    l_hashtable_entry_s *p;
    for(int i=0; i<MAX_HASHTABLE_ARRAY_SIZE; i++)
    {
        p = hash->_array[i]->first;
        while(p)
        {
            if(callback){
                callback(p);
            }
            else{
                printf("hash:%d, key:%s, value:%p\n", p->hashcode, p->key, p->value);
            }
            p = p->next;
        }
    }
}