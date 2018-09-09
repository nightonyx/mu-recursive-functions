#include <mem.h>
#include <malloc.h>
#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>
#include <lib4aio/utils/char_utils/char_utils.h>
#include <lib4aio/utils/string_utils/string_utils.h>
#include <lib4aio/utils/memory_utils/memory_utils.h>
#include <lib4aio/utils/point_watcher/point_watcher.h>

static void create_left_part(
        const str_hook *hook,
        const int left_index,
        size_t delimiter_length,
        str_hook_list *list
)
{
    str_hook *left_hook = new_str_hook_by_other_hook_string(hook);
    left_hook->start = hook->start;
    left_hook->end = left_index - delimiter_length;
    add_str_hook_in_list(list, left_hook);
}

static void create_right_part(const str_hook *hook, const int right_index, str_hook_list *list)
{
    str_hook *right_hook = new_str_hook_by_other_hook_string(hook);
    right_hook->start = right_index;
    right_hook->end = hook->end;
    add_str_hook_in_list(list, right_hook);
}

str_hook_list *split_str_hook_by_comma(const str_hook *hook)
{
    str_hook_list *result = split_str_hook_by_char_condition(hook, is_comma);
    return result;
}

str_hook_list *split_str_hook_by_space(const str_hook *hook)
{
    str_hook_list *result = split_str_hook_by_char_condition(hook, is_space);
    return result;
}

str_hook_list *split_str_hook_by_line_break(const str_hook *hook)
{
    str_hook_list *result = split_str_hook_by_char_condition(hook, is_line_break);
    return result;
}

str_hook_list *split_str_hook_by_whitespace(const str_hook *hook)
{
    str_hook_list *result = split_str_hook_by_char_condition(hook, is_space_or_line_break);
    return result;
}

static str_hook_list *create_str_hook_list_without_split(const str_hook *hook)
{
    str_hook_list *list = new_str_hook_list();
    str_hook *new_hook = new_str_hook_by_other(hook);
    add_str_hook_in_list(list, new_hook);
    return list;
}

str_hook_list *split_str_hook_by_string(const str_hook *hook, const_string delimiter)
{
    const_string src = hook->source_string;
    const size_t src_length = (size_t) get_str_hook_size(hook);
    const size_t delimiter_length = strlen(delimiter);
    //Is empty string:
    if (src_length < 1 || delimiter_length == 0) {
        return create_str_hook_list_without_split(hook);
    }
    //Create split indices:
    int *indices = new_object_array(src_length, sizeof(int));
    unsigned pointers = 0;
    //Mark split points:
    for (int i = hook->start; i < hook->end; ++i) {
        int result = 0;
        while (result == 0) {
            for (int j = 0; j < delimiter_length; ++j) {
                if (src[i + j] != delimiter[j]) {
                    result = -1;
                    break;
                }
            }
            if (result == 0) {
                i += delimiter_length;
                indices[pointers] = i;
                pointers++;
            }
        }
    }
    //Cannot find points:
    if (pointers == 0) {
        return create_str_hook_list_without_split(hook);
    }
    reallocate_object_array(indices, pointers, sizeof(unsigned));
    str_hook_list *list = new_str_hook_list();
    //Create left part:
    create_left_part(hook, indices[0], delimiter_length, list);
    //Create middle parts:
    if (pointers > 1) {
        //From second delimiter:
        for (unsigned j = 0; j < pointers - 1; ++j) {
            str_hook *new_hook = new_str_hook(src);
            const int start_position = indices[j];
            const int hold_length = indices[j + 1] - indices[j] - delimiter_length;
            if (hold_length > 0) {
                new_hook->start = start_position;
                new_hook->end = start_position + hold_length;
            }
            add_str_hook_in_list(list, new_hook);
        }
    }
    //Create right part:
    create_right_part(hook, indices[pointers - 1], list);
    return list;
}

str_hook_list *split_str_hook_by_char_condition(const str_hook *hook, boolean (*char_condition)(const char))
{
    const_string src = hook->source_string;
    const int start = hook->start;
    const int end = hook->end;
    str_hook_list *hooks = new_str_hook_list();
    point_watcher *watcher = new_point_watcher();
    watcher->start = (unsigned int) start;
    for (int i = start; i < end; ++i) {
        const char symbol = src[i];
        const_boolean is_split_point = char_condition(symbol);
        if (is_split_point) {
            str_hook *new_hook = new_str_hook(src);
            new_hook->start = watcher->start;
            new_hook->end = i;
            add_str_hook_in_list(hooks, new_hook);
            watcher->start = (unsigned int) (i + 1);
        }
    }
    str_hook *last_hook = new_str_hook(src);
    last_hook->start = watcher->start;
    last_hook->end = end;
    add_str_hook_in_list(hooks, last_hook);
    return hooks;
}
