# l_clib
常用的c数据结构，list，hashtable等

# 编译运行

需要实现安装python3.6+ scons

```c
$ cd sample
$ scons
$ ./list_test
```

# LIST
## 函数

- l_list_init(l_list_s *list): 初始化一个list，将list的首节点/末节点置空，size设置为0
- l_list_destroy(l_list_s *list): 销毁一个list
- l_list_lpush(l_list_s *list, void *data, size_t size): 从list的左侧添加一个元素
- l_list_rpush(l_list_s *list, void *data, size_t size): 从list的右侧添加一个元素
- l_list_insert(l_list_s *list, int n, void *data, size_t size): 从list的第n个元素的位置添加一个元素，第n个元素变成第n+1个元素
- l_list_remove(l_list_s *list, int n): 从list中，移除第n个元素，被移除的元素会直接在函数内释放内存
- l_list_get(l_list_s *list, int n): 从list获取第n个元素，返回的元素指针依然指在list中，不会影响list的连续性，修改获取的元素，同时会修改list中的元素，如修改元素的前后指针，会破坏元list的连续，使用时注意
- l_list_lpop(l_list_s *list): 从list的左侧弹出一个元素，该元素从list中被移除
- l_list_rpop(l_list_s *list): 从list的右侧弹出一个元素，该元素从list中被移除
- l_list_elt_clone(l_list_elt_s *src): 克隆一个list元素
- l_list_slice(l_list_s *list,  int m, int n): list切片，返回一个切片的list

## 样例
```c
#include "l_list.h"
#include "l_log.h"
#include "l_type.h"
int main(int argv, char *args)
{
    l_list_s list;
    l_list_init(&list);
    l_list_lpush(&list, "hello1", 6);
    l_list_lpush(&list, "hello2", 6);
    l_list_lpush(&list, "hello3", 6);
    l_list_insert(&list, 3, "hello4", 6);
    l_list_display(&list);
    l_list_destroy(&list);
    return 1;
}

