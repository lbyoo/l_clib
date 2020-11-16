#include "l_list.h"
#include "l_type.h"
#include "l_mem.h"
#include "l_log.h"
#include "l_lock.h"
#include "l_md5.h"

/**
 * 销毁一个list元素,如果指定了元素销毁函数，则调用函数销毁
 */
int l_list_elt_destroy(l_list_s *list, l_list_elt_s *elt)
{
    if(list->free_elt)
    {
        list->free_elt(elt);
    }
    L_FREE(elt);
    return L_SUCCESS;
}

/**
 * l_list_init(l_list_s *list): 初始化一个list，将list的首节点/末节点置空，size设置为0
 * 参数：list，l_list_s指针，传入函数前，需经过内存申请
 * 返回：0：成功，其他值失败
 */
int l_list_init(l_list_s *list)
{
    L_ASSERT(list);
    list->first = L_NULL;
    list->current = L_NULL;
    list->last = L_NULL;
    list->size = 0;
    list->free_elt = L_NULL;
    list->equals = l_list_elt_equals;
    L_LOCK_INIT(&list->lock);
    return L_SUCCESS;
}

/**
 * l_list_destroy(l_list_s *list): 销毁一个list
 * 参数：list指针，如果传入的指针为空，则直接返回成功
 * 从list的首节点开始，依次释放list元素所占内存
 * list本身所占内存不会在此函数中释放，需要单独释放，因为调用时可能传入的时一个局部list变量
 * 返回：0：成功，其他值失败
 */
int l_list_destroy(l_list_s *list)
{
    if(!list)
    {
        return L_SUCCESS;
    }
    l_list_empty(list);
    L_LOCK_DESTROY(&list->lock);
    return L_SUCCESS;
}
/**
 * l_list_empty(l_list_s *list): 清空list
 * 参数：list指针，如果传入的指针为空，则直接返回成功
 * 返回：0：成功，其他值失败
 */
int l_list_empty(l_list_s *list)
{
    L_ASSERT(list);
    l_list_elt_s *p;
    L_LOCK(&list->lock);
    while((p = l_list_lpop(list)) != L_NULL)
    {
        l_list_elt_destroy(list, p);
    }
    L_UNLOCK(&list->lock);
    return L_SUCCESS;
}
/**
 * l_list_lpush(l_list_s *list, void *data, size_t size): 从list的左侧添加一个元素，list中的数据只是指针转递不复制参数中的数据
 * 参数：list，list对象指针
 *       data，要添加到list中的数据
 *       size, 要添加到list中的数据的长度
 * 返回：0：成功，其他值失败
 */
int l_list_lpush(l_list_s *list, void *data, size_t size)
{
    L_ASSERT(list);
    l_list_elt_s *elt;
    L_LOCK(&list->lock);
    L_MALLOC(elt, l_list_elt_s, 1);
    elt->data = data;
    elt->data_len = size;
    elt->prev = L_NULL;
    elt->next = list->first;
    elt->data_len = size;
    if(list->first)
    {
        list->first->prev = elt;
    }
    list->first = elt;
    
    list->size++;
    if(list->size == 1)
    {
        list->last = elt;
    }
    L_UNLOCK(&list->lock);
    return L_SUCCESS;
}

/**
 * l_list_rpush(l_list_s *list, void *data, size_t size): 从list的右侧添加一个元素，list中的数据只是指针转递不复制参数中的数据
 * 参数：list，list对象指针
 *       data，要添加到list中的数据
 *       size, 要添加到list中的数据的长度
 * 返回：0：成功，其他值失败
 */
int l_list_rpush(l_list_s *list, void *data, size_t size)
{
    L_ASSERT(list);
    l_list_elt_s *elt;
    L_LOCK(&list->lock);
    L_MALLOC(elt, l_list_elt_s, 1);
    elt->data = data;
    elt->data_len = size;
    elt->prev = list->last;
    elt->next = L_NULL;
    if(list->last)
    {
        list->last->next = elt;
    }
    list->last = elt;
    list->size++;
    if(list->size == 1)
    {
        list->first = elt;
    }
    L_UNLOCK(&list->lock);
    return L_SUCCESS;
}

/**
 * l_list_insert(l_list_s *list, int n, void *data, size_t size): 从list的第n个元素的位置添加一个元素，第n个元素变成第n+1个元素
 * 参数：list，list对象指针
 *       n，插入元素的位置
 *       data，要添加到list中的数据
 *       size, 要添加到list中的数据的长度
 * 返回：0：成功，其他值失败
 */
int l_list_insert(l_list_s *list, int n, void *data, size_t size)
{
    L_ASSERT(list);
    if(n > list->size)
    {
        L_LOG_ERROR("list out of range, n:%d, size:%lu\n", n, list->size);
        return L_FAIL;
    }
    l_list_elt_s *p;
    l_list_elt_s *elt;
    L_LOCK(&list->lock);
    L_MALLOC(elt, l_list_elt_s, 1);
    elt->data = data;
    elt->data_len = size;
    int i = 0;
    p = list->first;
    while(i < n)
    {
        p = p->next;
        i++;
    }
    if(p == L_NULL)
    {
        elt->prev = list->last;
        list->last->next = elt;
        list->last = elt;
        elt->next = L_NULL;
        goto success;
    }
    if(p->prev)
    {
        p->prev->next = elt;
        elt->prev = p->prev;
    }else
    {
        elt->prev = L_NULL;
        list->first = elt;   
    }
    elt->next = p;
    p->prev = elt;

success:
    list->size++;
    L_UNLOCK(&list->lock);
    return L_SUCCESS;
}

/**
 * l_list_remove(l_list_s *list, int n): 从list中，移除第n个元素，被移除的元素会直接在函数内释放内存
 * 参数：list，list对象指针
 *       n，要移除的元素索引
 * 返回：0：成功，其他值失败
 */
int l_list_remove(l_list_s *list, int n)
{
    L_ASSERT(list);
    if(n >= list->size)
    {
        L_LOG_ERROR("list out of range, n:%d, size:%lu\n", n, list->size);
        return L_FAIL;
    }
    l_list_elt_s *p;
    l_list_elt_s *elt;
    L_LOCK(&list->lock);
    int i = 0;
    p = list->first;
    while(i < n)
    {
        p = p->next;
        i++;
    }
    elt = p;
    if(p->prev == L_NULL && p->next == L_NULL){
        list->first = L_NULL;
        list->last = L_NULL;
        goto success;
    }
    if(p->prev == L_NULL && p->next)
    {
        list->first = p->next;
        p->next->prev = L_NULL;
        goto success;
    }
    if(p->next == L_NULL && p->prev)
    {
        list->last = p->prev;
        p->prev->next = L_NULL;
        goto success;
    }
    if(p->next && p->prev)
    {
        p->prev->next = p->next;
        p->next->prev = p->prev;
        goto success;
    }

success:
    l_list_elt_destroy(list, elt);
    list->size--;
    L_UNLOCK(&list->lock);
    return L_SUCCESS;
}

/**
 * l_list_get(l_list_s *list, int n): 从list获取第n个元素，返回的元素指针依然指在list中，不会影响list的连续性，修改获取的元素，同时会修改list中的元素，
 * 如修改元素的前后指针，会破坏元list的连续，使用时注意
 * 参数：list，list对象指针
 *       n，要获取的元素索引
 * 返回：元素指针，如果n超过list的索引范围，则返回NULL
 */
l_list_elt_s *l_list_get(l_list_s *list, int n)
{
    L_ASSERT(list);
    if(n >= list->size)
    {
        L_LOG_ERROR("list out of range, n:%d, size:%lu\n", n, list->size);
        return L_NULL;
    }
    l_list_elt_s *p;
    int i = 0;
    p = list->first;
    while(i < n)
    {
        p = p->next;
        i++;
    }
    return p;
}

/**
 * l_list_lpop(l_list_s *list): 从list的左侧弹出一个元素，该元素从list中被移除
 * 参数：list，list对象指针
 * 返回：元素指针，如果list已空，则返回NULL
 */
l_list_elt_s *l_list_lpop(l_list_s *list)
{
    L_ASSERT(list);
    if(list->size == 0)
    {
        return L_NULL;
    }
    l_list_elt_s *elt;
    L_LOCK(&list->lock);
    elt = list->first;
    list->first = list->first->next;
    if(list->first)
    {
        list->first->prev = L_NULL;
    }
    list->size--;
    if(list->size == 0)
    {
        list->last = L_NULL;
    }
    L_UNLOCK(&list->lock);
    return elt;
}

/**
 * l_list_rpop(l_list_s *list): 从list的右侧弹出一个元素，该元素从list中被移除
 * 参数：list，list对象指针
 * 返回：元素指针，如果list已空，则返回NULL
 */
l_list_elt_s *l_list_rpop(l_list_s *list)
{
    L_ASSERT(list);
    if(list->size == 0)
    {
        return L_NULL;
    }
    l_list_elt_s *elt;
    L_LOCK(&list->lock);
    elt = list->last;
    list->last = list->last->prev;
    if(list->last)
    {
        list->last->next = L_NULL;
    }
    list->size--;
    if(list->size == 0)
    {
        list->first = L_NULL;
    }
    L_UNLOCK(&list->lock);
    return elt;
}

/**
 * l_list_elt_clone(l_list_elt_s *src): 克隆一个list元素
 * 参数：src，被克隆的源元素指针
 * 返回：被克隆的元素指针
 */
l_list_elt_s *l_list_elt_clone(l_list_elt_s *src)
{
    L_ASSERT(src);
    l_list_elt_s *dest;
    L_MALLOC(dest, l_list_elt_s, 1);
    dest->prev = src->prev;
    dest->next = src->next;
    dest->data_len = src->data_len;
    dest->data = src->data;
    return dest;
}

/**
 * l_list_slice(l_list_s *list,  int m, int n): list切片，返回一个切片的list
 * 参数：list，原list
 *      m，切片起始位置
 *      n，切片结束位置
 * 返回：切片list
 */
l_list_s *l_list_slice(l_list_s *list,  int m, int n)
{
    L_LOCK(&list->lock);
    if(m < 0 || n > list->size - 1)
    {
        L_LOG_ERROR("out of range (%d:%d)/%lu\n", m,n, list->size);
        return L_NULL;
    }
    l_list_s *dest;
    L_MALLOC(dest, l_list_s, 1);
    l_list_init(dest);
    int i = m;
    l_list_elt_s *p;
    while(i <= n)
    {
        p = l_list_get(list, i);
        if(p)
        {
            l_list_rpush(dest, p->data, p->data_len);
        }
        i++;
    }
    L_UNLOCK(&list->lock);
    return dest;

}

void l_list_set_elt_free_method(l_list_s *list, int (*free_elt)(l_list_elt_s *elt))
{
    list->free_elt = free_elt;
}
void l_list_set_elt_equals_method(l_list_s *list, int (*equals)(l_list_elt_s *elt, void *data, size_t data_len))
{
    list->equals = equals;
}

L_BOOL l_list_elt_equals(l_list_elt_s *elt, void *data, size_t data_len)
{
    if(elt->data_len != data_len)
    {
        return L_FALSE;
    }
    if(memcmp(elt->data, data, data_len) == 0)
    {
        return L_TRUE;
    };
    return L_FALSE;
}

L_BOOL l_list_elt_exist(l_list_s *list, void *data, size_t data_len)
{
    l_list_elt_s *elt = list->first;
    while(elt)
    {
        if(list->equals(elt, data, data_len)){
            return L_TRUE;
        }
        elt = elt->next;
    }
    return L_FALSE;
}


void l_list_display(l_list_s *list){
    L_ASSERT(list);
    l_list_elt_s *elt = list->first;
    char buf[50];
    for(int i=0; i<list->size; i++){
        memset(buf,0, sizeof(buf));
        memcpy(buf, elt->data, elt->data_len);
        printf("%s\n", buf);
        elt = elt->next;
    }
}