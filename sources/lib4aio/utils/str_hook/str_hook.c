
#include <malloc.h>
#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/utils/string_utils/string_utils.h>
#include <lib4aio/utils/memory_utils/memory_utils.h>
#include <lib4aio/utils/type_utils/type.utils.h>
#include <lib4aio/utils/boolean_utils/boolean_utils.h>
#include <lib4aio/utils/error_utils/error_utils.h>

#define INIT_CAPACITY 2

/**
 * String hook.
 */

str_hook *new_str_hook(string source_ref)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = source_ref;
    hook->start = 0;
    hook->end = 0;
    return hook;
}

void reset_str_hook_positions(str_hook *hook)
{
    hook->start = 0;
    hook->end = 0;
}

void free_str_hook(str_hook *hook)
{
    if (hook) {
        free(hook);
    }
}

/**
 * List.
 */

str_hook_list *new_str_hook_list()
{
    str_hook_list *list = new_object(sizeof(str_hook_list));
    list->capacity = INIT_CAPACITY;
    list->size = 0;
    list->hooks = new_object_array(INIT_CAPACITY, sizeof(str_hook));
    return list;
}

static void update_memory_in_string_hook_list(str_hook_list *list)
{
    if (list->size == list->capacity) {
        list->capacity *= INIT_CAPACITY;
        list->hooks = reallocate_object_array(list->hooks, list->capacity, sizeof(str_hook));
    }
}

void add_str_hook_in_list(str_hook_list *list, str_hook *hook)
{
    update_memory_in_string_hook_list(list);
    list->hooks[list->size] = hook;
    list->size++;
}

void free_str_hooks_in_list(str_hook_list *list)
{
    str_hook **hooks = list->hooks;
    for (unsigned i = 0; i < list->size; ++i) {
        free_str_hook(hooks[i]);
    }
}

void free_str_hook_list(str_hook_list *list)
{
    str_hook **hooks = list->hooks;
    free(hooks);
    free((void *) list);
}

/**
 * String hook iterator.
 */


str_hook_iterator *new_str_hook_iterator()
{
    str_hook_iterator *iterator = new_object(sizeof(str_hook_iterator));
    iterator->position = 0;
    return iterator;
}

str_hook_iterator *new_str_hook_iterator_by_list(
        str_hook_list *str_hook_chain,
        const unsigned current_hook_index
)
{
    val start_position = str_hook_chain->hooks[current_hook_index]->start;
    //Create iterator:
    str_hook_iterator *iterator = new_object(sizeof(str_hook_iterator));
    iterator->str_hook_list = str_hook_chain;
    iterator->current_hook_index = current_hook_index;
    iterator->position = start_position;
    return iterator;
}

boolean next_in_str_hook_iterator(str_hook_iterator *iterator)
{
    //Extract list:
    str_hook_list *list = iterator->str_hook_list;
    str_hook **hooks = list->hooks;
    //Get current hook:
    const unsigned current_hook_index = iterator->current_hook_index;
    str_hook *current_hook = hooks[current_hook_index];
    //Check next position:
    if (iterator->position < current_hook->end - 1) {
        iterator->position++;
        return TRUE;
    } else {
        if (current_hook_index < list->size - 1) {
            iterator->position = hooks[++iterator->current_hook_index]->start;
            return TRUE;
        } else {
            iterator->position++;
            return FALSE;
        }
    }
}


boolean back_in_str_hook_iterator(str_hook_iterator *iterator)
{
    //Extract list:
    str_hook_list *list = iterator->str_hook_list;
    str_hook **hooks = list->hooks;
    //Get current hook:
    const unsigned current_hook_index = iterator->current_hook_index;
    str_hook *current_hook = hooks[current_hook_index];
    //Check next position:
    if (iterator->position > current_hook->start) {
        iterator->position--;
        return TRUE;
    } else {
        if (current_hook_index > 0) {
            iterator->position = hooks[--iterator->current_hook_index]->end - 1;
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

void free_str_hook_iterator(str_hook_iterator *iterator)
{
    if (iterator) {
        free(iterator);
    }
}

#define STR_HOOK_CHAIN_TAG "STR_HOOK_CHAIN"

str_hook_chain *new_str_hook_chain()
{
    return new_str_hook_list();
}

int get_str_hook_chain_start(str_hook_list *list)
{
    if (list->size > 0) {
        val first_hook = list->hooks[0];
        return first_hook->start;
    } else {
        throw_error_with_tag(STR_HOOK_CHAIN_TAG, "String hook chain is empty!");
    }
}

int get_str_hook_chain_end(str_hook_list *list)
{
    val list_size = list->size;
    if (list_size > 0) {
        val last_hook = list->hooks[list_size - 1];
        return last_hook->end;
    } else {
        throw_error_with_tag(STR_HOOK_CHAIN_TAG, "String hook chain is empty!");
    }
}

int get_str_hook_chain_position(str_hook_list *list, const int number_of_steps)
{
    val size = list->size;
    if (size > 0) {
        var step_counter = 0;
        for (unsigned i = 0; i < size; ++i) {
            val first_hook = list->hooks[i];
            val start = first_hook->start;
            val end = first_hook->end;
            for (int j = start; j < end; ++j) {
                if (step_counter++ == number_of_steps) {
                    return j;
                }
            }
        }
        throw_error_with_tag(STR_HOOK_CHAIN_TAG, "Index of bound error!");
    } else {
        throw_error_with_tag(STR_HOOK_CHAIN_TAG, "String hook chain is empty!");
    }
}


void free_str_hook_chain(str_hook_chain *chain)
{
    free_str_hooks_in_list(chain);
    free_str_hook_list(chain);
}

/**
 * String hook chain list.
 */

str_hook_list *new_str_hook_chain_list()
{
    str_hook_chain_list *list = new_object(sizeof(str_hook_chain_list));
    list->capacity = INIT_CAPACITY;
    list->size = 0;
    list->chains = new_object_array(INIT_CAPACITY, sizeof(str_hook_list *));
    return list;
}

static void update_memory_in_string_hook_chain_list(str_hook_chain_list *list)
{
    if (list->size == list->capacity) {
        list->capacity *= INIT_CAPACITY;
        list->chains = reallocate_object_array(list->chains, list->capacity, sizeof(str_hook_chain_list *));
    }
}

void add_str_hook_chain_in_list(str_hook_chain_list *list, str_hook_list *chain)
{
    update_memory_in_string_hook_chain_list(list);
    list->chains[list->size++] = chain;
}

void free_str_hook_chains_in_list(str_hook_chain_list *list)
{
    var hooks = list->chains;
    free(hooks);
    free(list);
}

void free_str_hook_chain_list(str_hook_chain_list *list)
{
    val size = list->size;
    var chains = list->chains;
    for (unsigned i = 0; i < size; ++i) {
        free_str_hooks_in_list(chains[i]);
        free_str_hook_list(chains[i]);
    }
}
