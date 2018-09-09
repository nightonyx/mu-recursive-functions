#include <mem.h>
#include <malloc.h>
#include <lib4aio/collections/lists/string_list.h>
#include <lib4aio/utils/memory_utils/memory_utils.h>
#include <lib4aio/utils/string_utils/string_utils.h>

#define INIT_CAPACITY 2

string_list *new_string_list()
{
    string_list *list = new_object(sizeof(string_list));
    list->capacity = INIT_CAPACITY;
    list->size = 0;
    list->strings = new_object_array(INIT_CAPACITY, sizeof(string));
    return list;
}

static void update_memory_in_string_list(string_list *string_list)
{
    if (string_list->size == string_list->capacity) {
        string_list->capacity *= INIT_CAPACITY;
        string_list->strings = reallocate_object_array(
                string_list->strings,
                string_list->capacity,
                sizeof(string)
        );
    }
}

void add_string_in_list(string_list *string_list, string source)
{
    update_memory_in_string_list(string_list);
    string_list->strings[string_list->size++] = source;
}

void free_strings_in_list(string_list *list)
{
    string_array strings = list->strings;
    for (unsigned i = 0; i < list->size; ++i) {
        string old_string = strings[i];
        list->strings[i] = NULL;
        if (old_string) {
            free(old_string);
        }
    }
}

void free_string_list(string_list *list)
{
    const size_t size = list->size;
    for (unsigned i = 0; i < size; ++i) {
        list->strings[i] = NULL;
    }
    free(list->strings);
    free(list);
}
