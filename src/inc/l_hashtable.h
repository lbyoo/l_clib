#ifndef _L_HASHTABLE_H_
#define _L_HASHTABLE_H_

#define MAX_HASHTABLE_ARRAY_SIZE 1024
typedef struct l_hashtable_entry l_hashtable_entry_s;
typedef struct l_hashtable_entry_root l_hashtable_entry_root_s;

typedef struct l_hashtable
{
    l_hashtable_entry_root_s *_array[MAX_HASHTABLE_ARRAY_SIZE];
    int size;
    L_MUTEX lock;
    int (*free_value)(l_hashtable_entry_s *entry);
} l_hashtable_s;

typedef struct l_hashtable_entry_root
{
    l_hashtable_entry_s *first;
    l_hashtable_entry_s *last;
    int size;
}l_hashtable_entry_root_s;

typedef struct l_hashtable_entry
{
    char *key;
    void *value;
    size_t value_len;
    int hashcode;
    l_hashtable_entry_s *next;
    l_hashtable_entry_s *prev;
    int delete_flag;

}l_hashtable_entry_s;


//for each callback;
typedef int (*l_hashtable_enum_callback)(l_hashtable_entry_s *entry);

extern int l_hashtable_init(l_hashtable_s *hash);
extern int l_hashtable_destroy(l_hashtable_s *hash);
extern int l_hashtable_empty(l_hashtable_s *hash);
extern L_BOOL l_hashtable_exist(l_hashtable_s *hash, char *key, l_hashtable_entry_s **found);
extern int l_hashtable_push(l_hashtable_s *hash, char *key, void *value, size_t value_len);
extern int l_hashtable_remove(l_hashtable_s *hash, char *key);
extern int l_hashtable_entry_destroy(l_hashtable_s *hash, l_hashtable_entry_s *entry);
extern int l_hashtable_enum(l_hashtable_s *hash, l_hashtable_enum_callback callback);
extern l_hashtable_entry_s *l_hashtable_get(l_hashtable_s *hash, char *key);
extern int l_hashtable_entry_init(l_hashtable_entry_s *entry);


#define L_HASHTABLE_FREE_VALUE_FUNC_NAME(FUNC) l_hashtable_free_value_##FUNC
#define L_HASHTABLE_FREE_VALUE_FUNC(FUNC) int l_hashtable_free_value_##FUNC(l_hashtable_entry_s *entry)
#define L_HASHTABLE_ENUM_CALLBACK_NAME(FUNC) l_hashtable_callback_##FUNC
#define L_HASHTABLE_ENUM_CALLBACK(FUNC) int l_hashtable_callback_##FUNC(l_hashtable_entry_s *entry)
#endif
