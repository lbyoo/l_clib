#include "l_list.h"
#include "l_type.h"
#include "l_mem.h"
#include "l_log.h"
/**
 * 销毁一个list元素
 */
static int l_list_elt_destroy(l_list_elt_s *elt)
{
    L_FREE(elt->data);
    L_FREE(elt);
}

/**
 * 初始化一个list，将list的首节点/末节点置空，size设置为0
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
    return L_SUCCESS;
}

/**
 * 销毁一个list
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
    l_list_elt_s *p = list->first;
    l_list_elt_s *next = L_NULL;

    while(p)
    {
        next = p->next;
        l_list_elt_destroy(p);
        p = next;
    }

    return L_SUCCESS;
}
/**
 * 从list的左侧添加一个元素
 * 参数：list，list对象指针
 *       data，要添加到list中的数据
 *       size, 要添加到list中的数据的长度
 * 返回：0：成功，其他值失败
 */
int l_list_lpush(l_list_s *list, void *data, size_t size)
{
    L_ASSERT(list);
    l_list_elt_s *elt;
    L_MALLOC(elt, l_list_elt_s, 1);
    L_MALLOC(elt->data, char, size);
    memcpy(elt->data, data, size);
    elt->pre = L_NULL;
    elt->next = list->first;
    elt->data_len = size;
    if(list->first)
    {
        list->first->pre = elt;
    }
    list->first = elt;
    
    list->size++;
    if(list->size == 1)
    {
        list->last = elt;
    }
    return L_SUCCESS;
}

/**
 * 从list的右侧添加一个元素
 * 参数：list，list对象指针
 *       data，要添加到list中的数据
 *       size, 要添加到list中的数据的长度
 * 返回：0：成功，其他值失败
 */
int l_list_rpush(l_list_s *list, void *data, size_t size)
{
    L_ASSERT(list);
    l_list_elt_s *elt;
    L_MALLOC(elt, l_list_elt_s, 1);
    L_MALLOC(elt->data, char, size);
    memcpy(elt->data, data, size);
    elt->data_len = size;
    elt->pre = list->last;
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
    return L_SUCCESS;
}

/**
 * 从list的第n个元素的位置添加一个元素，第n个元素变成第n+1个元素
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
        L_LOG_ERROR("list out of range, n:%d, size:%d\n", n, list->size);
        return L_FAIL;
    }
    l_list_elt_s *p;
    l_list_elt_s *elt;
    L_MALLOC(elt, l_list_elt_s, 1);
    L_MALLOC(elt->data, char, size);
    memcpy(elt->data, data, size);
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
        elt->pre = list->last;
        list->last->next = elt;
        list->last = elt;
        elt->next = L_NULL;
        goto success;
    }
    if(p->pre)
    {
        p->pre->next = elt;
        elt->pre = p->pre;
    }else
    {
        elt->pre = L_NULL;
        list->first = elt;   
    }
    elt->next = p;
    p->pre = elt;

success:
    list->size++;
    return L_SUCCESS;
}

/**
 * 从list中，移除第n个元素，被移除的元素会直接在函数内释放内存
 * 参数：list，list对象指针
 *       n，要移除的元素索引
 * 返回：0：成功，其他值失败
 */
int l_list_remove(l_list_s *list, int n)
{
    L_ASSERT(list);
    if(n >= list->size)
    {
        L_LOG_ERROR("list out of range, n:%d, size:%d\n", n, list->size);
        return L_FAIL;
    }
    l_list_elt_s *p;
    l_list_elt_s *elt;
    int i = 0;
    p = list->first;
    while(i < n)
    {
        p = p->next;
        i++;
    }
    elt = p;
    if(p->pre == L_NULL && p->next == L_NULL){
        list->first = L_NULL;
        list->last = L_NULL;
        goto success;
    }
    if(p->pre == L_NULL && p->next)
    {
        list->first = p->next;
        p->next->pre = L_NULL;
        goto success;
    }
    if(p->next == L_NULL && p->pre)
    {
        list->last = p->pre;
        p->pre->next = L_NULL;
        goto success;
    }
    if(p->next && p->pre)
    {
        p->pre->next = p->next;
        p->next->pre = p->pre;
        goto success;
    }

success:
    l_list_elt_destroy(elt);
    list->size--;
    return L_SUCCESS;
}

/**
 * 从list获取第n个元素，返回的元素指针依然指在list中，不会影响list的连续性，修改获取的元素，同时会修改list中的元素，
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
        L_LOG_ERROR("list out of range, n:%d, size:%d\n", n, list->size);
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
 * 从list的左侧弹出一个元素，该元素从list中被移除
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
    elt = list->first;
    list->first = list->first->next;
    if(list->first)
    {
        list->first->pre = L_NULL;
    }
    list->size--;
    if(list->size == 0)
    {
        list->last = L_NULL;
    }
    return elt;
}

/**
 * 从list的右侧弹出一个元素，该元素从list中被移除
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
    elt = list->last;
    list->last = list->last->pre;
    if(list->last)
    {
        list->last->next = L_NULL;
    }
    list->size--;
    if(list->size == 0)
    {
        list->first = L_NULL;
    }
    return elt;
}

/**
 * 克隆一个list元素
 * 参数：src，被克隆的源元素指针
 * 返回：被克隆的元素指针
 */
l_list_elt_s *l_list_elt_clone(l_list_elt_s *src)
{
    L_ASSERT(src);
    l_list_elt_s *dest;
    L_MALLOC(dest, l_list_elt_s, 1);
    dest->pre = src->pre;
    dest->next = src->next;
    dest->data_len = src->data_len;
    memcpy(dest->data, src->data, src->data_len);
    return dest;
}

/**
 * list切片，返回一个切片的list
 * 参数：list，原list
 *      m，切片起始位置
 *      n，切片结束位置
 * 返回：切片list
 */
l_list_s *l_list_slice(l_list_s *list,  int m, int n)
{
    if(m < 0 || n > list->size - 1)
    {
        L_LOG_ERROR("out of range (%d:%d)/%d\n", m,n, list->size);
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
    return dest;

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