#include <malloc.h>
#include <lib4aio/utils/memory_utils/memory_utils.h>
#include <lib4aio/collections/lists/double_list.h>

#define INIT_CAPACITY 2

double_list *new_double_list()
{
    double_list *list = new_object(sizeof(double_list));
    list->capacity = INIT_CAPACITY;
    list->size = 0;
    list->values = new_object_array(INIT_CAPACITY, sizeof(double));
    return list;
}

static void update_memory_in_double_list(double_list *list)
{
    if (list->size + 1 == list->capacity) {
        list->capacity *= INIT_CAPACITY;
        list->values = reallocate_object_array(
                list->values,
                list->capacity,
                sizeof(double)
        );
    }
}

void add_in_double_list(double_list *list, const double value)
{
    update_memory_in_double_list(list);
    list->values[list->size++] = value;
}

void free_double_list(double_list *list)
{
    free(list->values);
    free(list);
}
