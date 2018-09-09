
#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/utils/boolean_utils/boolean_utils.h>
#include <lib4aio/utils/string_utils/string_utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>
#include <lib4aio/utils/char_utils/char_utils.h>

static str_hook *border_trim(
        const str_hook *hook,
        const_boolean has_left,
        const_boolean has_right,
        boolean (*trim_condition)(const char)
)
{
    const_string src = hook->source_string;
    str_hook *new_hook = new_str_hook_by_other(hook);
    const int src_length = get_str_hook_size(hook);
    if (src_length == 0) {
        return new_hook;
    }
    if (has_left == TRUE) {
        while (trim_condition(src[new_hook->start])) {
            new_hook->start++;
        }
    }
    if (has_right == TRUE) {
        while (trim_condition(src[new_hook->end - 1])) {
            new_hook->end--;
        }
    }
    if (new_hook->start > new_hook->end) {
        new_hook->start = hook->start;
        new_hook->end = hook->start;
    }
    return new_hook;
}

str_hook *trim_str_hook_by_start(const str_hook *hook)
{
    return border_trim(hook, TRUE, FALSE, is_space);
}

str_hook *trim_str_hook_by_end(const str_hook *hook)
{
    return border_trim(hook, FALSE, TRUE, is_space);
}

str_hook *trim_str_hook(const str_hook *hook)
{
    return border_trim(hook, TRUE, TRUE, is_space);
}

str_hook *trim_str_hook_with_line_break_by_start(const str_hook *hook)
{
    return border_trim(hook, TRUE, FALSE, is_space_or_line_break);
}

str_hook *trim_str_hook_with_line_break_by_end(const str_hook *hook)
{
    return border_trim(hook, FALSE, TRUE, is_space_or_line_break);
}

str_hook *trim_str_hook_with_line_break(const str_hook *hook)
{
    return border_trim(hook, TRUE, TRUE, is_space_or_line_break);
}

str_hook_list *trim_str_hook_list(const str_hook_list *list)
{
    str_hook** hooks = list->hooks;
    str_hook_list *new_list = new_str_hook_list();
    for (unsigned i = 0; i < list->size; ++i) {
        add_str_hook_in_list(new_list, trim_str_hook(hooks[i]));
    }
    return new_list;
}

str_hook_list *trim_str_hook_list_with_line_break(const str_hook_list *list)
{
    const str_hook** hooks = list->hooks;
    str_hook_list *new_list = new_str_hook_list();
    for (unsigned i = 0; i < list->size; ++i) {
        add_str_hook_in_list(new_list, trim_str_hook_with_line_break(hooks[i]));
    }
    return new_list;
}
