text = """
extern int l_hashtable_push(l_hashtable_s *hash, char *key, void *value);
extern int l_hashtable_remove(l_hashtable_s *hash, char *key);
extern l_hashtable_entry_s *l_hashtable_get(l_hashtable_s *hash, char *key);
"""

for x in text.splitlines():
    if x:
        print(x.strip("extern ").strip(";") + "\n{\n}\n")

