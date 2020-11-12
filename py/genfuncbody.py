text = """
extern int l_set_init(l_set_s *list);
extern int l_set_destroy(l_set_s *list);
extern int l_set_lpush(l_set_s *list, void *data, size_t size);
extern int l_set_rpush(l_set_s *list, void *data, size_t size);
extern int l_set_insert(l_set_s *list, int n, void *data, size_t size);
extern int l_set_remove(l_set_s *list, int n);
extern int l_set_empty(l_set_s *list);
extern int l_set_elt_destroy(l_set_s *list, l_set_elt_s *elt);
extern int l_set_elt_exist(l_set_s *list, void *data, size_t data_len);
extern int l_set_elt_equals(l_set_elt_s *elt, void *data, size_t data_len);
extern l_set_elt_s *l_set_get(l_set_s *list, int n);
extern l_set_elt_s *l_set_lpop(l_set_s *list);
extern l_set_elt_s *l_set_rpop(l_set_s *list);
extern l_set_s *l_set_slice(l_set_s *list, int m, int n);
extern l_set_elt_s *l_set_elt_clone(l_set_elt_s *src);
extern void l_set_display(l_set_s *list);
extern void l_set_set_elt_free_method(l_set_s *list, int (*free_elt)(l_set_elt_s *elt));
extern void l_set_set_elt_equals_method(l_set_s *list, int (*equals)(l_set_elt_s *elt, void *data, size_t data_len));
"""

for x in text.splitlines():
    print(x.strip("extern ").strip(";") + "\n{\n}\n")

