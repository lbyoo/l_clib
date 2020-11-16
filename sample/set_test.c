#include "l_set.h"
#include "l_log.h"
#include "l_type.h"
#include "l_mem.h"

void test1(l_set_s *set)
{
    l_set_add(set, "hello1", 6);
    l_set_add(set, "hello2", 6);
    l_set_add(set, "hello3", 6);
    l_set_add(set, "hello2", 6);
    l_set_display(set);
}
int main(int argv, char *args)
{
    l_set_s set;
    l_set_init(&set);
    test1(&set);
    l_set_empty(&set);

    l_set_destroy(&set);


}