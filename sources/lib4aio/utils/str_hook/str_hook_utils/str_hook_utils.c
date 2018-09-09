#include <ctype.h>
#include <mem.h>
#include <stdio.h>
#include <process.h>
#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/utils/string_utils/string_utils.h>
#include <lib4aio/utils/memory_utils/memory_utils.h>
#include <lib4aio/utils/boolean_utils/boolean_utils.h>
#include <lib4aio/utils/error_utils/error_utils.h>
#include <lib4aio/utils/point_watcher/point_watcher.h>
#include <lib4aio/utils/char_utils/char_utils.h>
#include <lib4aio/utils/type_utils/type.utils.h>

#define STRING_HOOK_DEBUG

#ifdef STRING_HOOK_DEBUG

#include <lib4aio/utils/log_utils/log_utils.h>

#endif

#define STRING_HOOK_TAG "STRING_HOOK"

/**
 * Extra constructors.
 */

str_hook *new_str_hook_with_start(string source_string, const int start_index)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = source_string;
    hook->start = start_index;
    hook->end = 0;
    return hook;
}

str_hook *new_str_hook_with_end(string source_string, const int end_index)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = source_string;
    hook->start = 0;
    hook->end = end_index;
    return hook;
}

str_hook *new_str_hook_with_start_and_end(string source_string, const int start_index, const int end_index)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = source_string;
    hook->start = start_index;
    hook->end = end_index;
    return hook;
}

str_hook *new_str_hook_by_point_watcher(string source_string, const point_watcher *watcher)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = source_string;
    hook->start = watcher->start;
    hook->end = watcher->end;
    return hook;
}

str_hook *new_str_hook_by_other(const str_hook *other_hook)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = other_hook->source_string;
    hook->start = other_hook->start;
    hook->end = other_hook->end;
    return hook;
}

str_hook *new_str_hook_by_other_hook_string(const str_hook *other_hook)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = other_hook->source_string;
    hook->start = 0;
    hook->end = 0;
    return hook;
}

str_hook *new_str_hook_by_string(string source_string)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = source_string;
    hook->start = 0;
    hook->end = strlen(source_string);
    return hook;
}

str_hook *new_str_hook_by_offset(string source_string, const int offset, const int length)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = source_string;
    hook->start = offset;
    hook->end = offset + length;
    return hook;
}

/**
 * Getters.
 */

int get_str_hook_size(const str_hook *hook)
{
    return hook->end - hook->start;
}

char get_str_hook_char(const str_hook *hook, const int index)
{
    return hook->source_string[index];
}

/**
 * Common utils.
 */

boolean is_word_hooked(const str_hook *hook)
{
    val length = get_str_hook_size(hook);
    if (length < 1) {
        return FALSE;
    }
    val first_symbol = get_str_hook_char(hook, hook->start);
    log_info_char(STRING_HOOK_TAG, "C:", first_symbol);
    if (isalpha(first_symbol)) {
        if (length > 1) {
            for (int i = hook->start + 1; i < length; ++i) {
                val symbol = get_str_hook_char(hook, i);
                if (!isalnum(symbol)) {
                    return FALSE;
                }
            }
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

string substring_by_str_hook(const str_hook *hook)
{
    return substring(hook->source_string, hook->start, hook->end);
}

boolean are_equal_hooked_str(const str_hook *hook_1, const str_hook *hook_2)
{
    const int size_1 = get_str_hook_size(hook_1);
    const int size_2 = get_str_hook_size(hook_2);
    if (size_1 != size_2) {
        return FALSE;
    }
    const_string str_1 = hook_1->source_string;
    const_string str_2 = hook_2->source_string;
    const int start_1 = hook_1->start;
    const int start_2 = hook_2->start;
    for (int i = 0; i < size_1; ++i) {
        if (str_1[start_1 + i] != str_2[start_2 + i]) {
            return FALSE;
        }
    }
    return TRUE;
}

boolean is_hook_equals_str(const str_hook *hook, const_string str)
{
    const int hook_size = get_str_hook_size(hook);
    const int str_length = strlen(str);
    if (hook_size != str_length) {
        return FALSE;
    }
    const_string hooked_str = hook->source_string;
    const int start = hook->start;
    for (int i = 0; i < hook_size; ++i) {
        if (hooked_str[start + i] != str[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

boolean is_empty_hooked_str(const str_hook *hook)
{
    return hook->end - hook->start <= 0;
}

boolean is_not_empty_hooked_str(const str_hook *hook)
{
    return hook->end - hook->start > 0;
}

str_hook_list *filter_str_hook_list(const str_hook_list *list, boolean (*filter_condition)(const str_hook *))
{
    const unsigned src_list_size = list->size;
    val src_hooks = list->hooks;
    var new_hook_list = new_str_hook_list();
    for (unsigned k = 0; k < src_list_size; ++k) {
        val src_hook = src_hooks[k];
        if (filter_condition(src_hook)) {
            var new_hook = new_str_hook_by_other(src_hook);
            add_str_hook_in_list(new_hook_list, new_hook);
        }
    }
    return new_hook_list;
}

/**
 * Log utils.
 */

void log_info_str_hook(const_string tag, const_string message, const str_hook *hook)
{
    printf("\n%s: %s -", tag, message);
    for (int i = hook->start; i < hook->end; ++i) {
        printf("%c", hook->source_string[i]);
    }
    printf("-\n");
}

void log_info_str_hook_list(const_string tag, const_string message, const str_hook_list *list)
{
    val size = list->size;
    val hooks = list->hooks;
    for (unsigned i = 0; i < size; ++i) {
        val hook = hooks[i];
        val src = hook->source_string;
        printf("\n%s: %s -", tag, message);
        for (int j = hook->start; j < hook->end; ++j) {
            printf("%c", src[j]);
        }
        printf("-\n");
    }
}

void log_info_str_hook_chain(const_string tag, const_string message, const str_hook_chain *chain)
{
    val size = chain->size;
    val hooks = chain->hooks;
    printf("\n%s: %s -", tag, message);
    for (unsigned i = 0; i < size; ++i) {
        val hook = hooks[i];
        val src = hook->source_string;
        for (int j = hook->start; j < hook->end; ++j) {
            printf("%c", src[j]);
        }
    }
    printf("-\n");
}

/**
 * Primitive type matchers.
 */

boolean is_int_hooked(const str_hook *hook)
{
    const_string string = hook->source_string;
    const int length = get_str_hook_size(hook);
    int start_position = hook->start;
    if (length <= 0) {
        throw_error_with_tag(STRING_HOOK_TAG, "Empty string doesn't matches int!");
    }
    if (string[start_position] == '-') {
        if (length == 1) {
            throw_error_with_tag(STRING_HOOK_TAG, "Minus doesn't matches int!");
        }
        start_position++;
    }
    for (int i = start_position; i < hook->end; ++i) {
        const int digit = string[i] - '0';
        if (digit < 0 || digit > 9) {
            return FALSE;
        }
    }
    return TRUE;
}


boolean is_double_hooked(const str_hook *hook)
{
    const_string string = hook->source_string;
    const int length = get_str_hook_size(hook);
    boolean was_dot = FALSE;
    boolean was_fraction = FALSE;
    int start_position = hook->start;
    if (length <= 0) {
        throw_error_with_tag(STRING_HOOK_TAG, "Empty string doesn't matches double!");
    }
    if (string[start_position] == '-') {
        if (length == 1) {
            throw_error_with_tag(STRING_HOOK_TAG, "Minus doesn't matches double!");
        }
        start_position++;
    }
    for (int i = start_position; i < hook->end; ++i) {
        const int digit = string[i] - '0';
        if ((digit < 0 || digit > 9)) {
            if (string[i] == '.' && was_dot == FALSE) {
                was_dot = TRUE;
            } else {
                return FALSE;
            }
        }
        if (digit >= 0 && digit < 10 && was_dot == TRUE) {
            was_fraction = TRUE;
        }
    }
    if (was_dot == TRUE && was_fraction == TRUE) {
        return TRUE;
    }
    return FALSE;
}

boolean is_string_hooked(const str_hook *hook)
{
    const int length = get_str_hook_size(hook);
    const_string hooked_string = hook->source_string;
    return length > 1 && hooked_string[hook->start] == '\'' && hooked_string[hook->end - 1] == '\'';
}

void throw_error_with_hook(const_string tag, const_string message, const str_hook *hook)
{
    printf("\n%s %s: %s -", ERROR_TAG, tag, message);
    for (int i = hook->start; i < hook->end; ++i) {
        printf("%c", hook->source_string[i]);
    }
    printf("-\n");
    exit(1);
}

str_hook_chain *new_str_hook_chain_by_other(str_hook_chain *chain)
{
    val size = chain->size;
    val hooks = chain->hooks;
    var new_chain = new_str_hook_list();
    for (unsigned i = 0; i < size; ++i) {
        add_str_hook_in_list(new_chain, new_str_hook_by_other(hooks[i]));
    }
    return new_chain;
}


str_hook_list *new_str_hook_chain_by_other_with_start(str_hook_list *chain, const int start)
{
    val size = chain->size;
    val hooks = chain->hooks;
    var new_chain = new_str_hook_chain();
    for (unsigned i = 0; i < size; ++i) {
        val hook = hooks[i];
        if (hook->end > start) {
            var new_hook = new_str_hook(hook->source_string);
            new_hook->end = hook->end;
            if (hook->start < start) {
                new_hook->start = start;
            } else {
                new_hook->start = hook->start;
            }
            if (is_not_empty_hooked_str(new_hook)) {
                add_str_hook_in_list(new_chain, new_hook);
            }
        }
    }
    return new_chain;
}

str_hook_list *new_str_hook_chain_by_other_with_start_and_end(
        str_hook_list *chain,
        const int start,
        const int end
)
{
#ifdef STRING_HOOK_DEBUG
    log_info_str_hook_chain(STRING_HOOK_TAG, "CHAIN:", chain);
#endif
    val size = chain->size;
    val hooks = chain->hooks;
    var new_chain = new_str_hook_chain();
    for (unsigned i = 0; i < size; ++i) {
        val hook = hooks[i];
#ifdef STRING_HOOK_DEBUG
        log_info(STRING_HOOK_TAG, "---------------------");
        log_info_str_hook(STRING_HOOK_TAG, "/HOOK:", hook);
        log_info_int(STRING_HOOK_TAG, "RSTART", start);
        log_info_int(STRING_HOOK_TAG, "/START", hook->start);
        log_info_int(STRING_HOOK_TAG, "REND", end);
        log_info_int(STRING_HOOK_TAG, "/END", hook->end);
        log_info(STRING_HOOK_TAG, "---------------------");
#endif
        if (hook->end < start || hook->start > end) {
            continue;
        } else {
#ifdef STRING_HOOK_DEBUG
            log_info(STRING_HOOK_TAG, "VALID HOOK!");
#endif
            var new_hook = new_str_hook(hooks[i]->source_string);
            if (hook->start < start) {
                new_hook->start = start;
            } else {
                new_hook->start = hook->start;
            }
            if (hook->end > end) {
                new_hook->end = end;
            } else {
                new_hook->end = hook->end;
            }
            log_info_str_hook(STRING_HOOK_TAG, "READY HOOK:", new_hook);
            if (is_not_empty_hooked_str(new_hook)) {
                add_str_hook_in_list(new_chain, new_hook);
            }
        }
    }
    return new_chain;
}


str_hook_list *new_str_hook_chain_by_other_with_start_and_step(
        str_hook_list *chain,
        const int start,
        const int step
)
{
    val chain_size = chain->size;
    if (chain_size > 0) {
        val hooks = chain->hooks;
        var iterator = new_str_hook_iterator();
        iterator->str_hook_list = chain;
        iterator->current_hook_index = 0;
        iterator->position = hooks[0]->start;
        while (TRUE) {
            if (iterator->position >= start) {
                for (int i = 0; i < step; ++i) {
                    if (!next_in_str_hook_iterator(iterator)) {
                        throw_error_with_tag(STRING_HOOK_TAG, "Index off bound error!");
                    }
                }
                //Create result chain:
                var result_chain = new_str_hook_chain();
                //Init current params:
                val current_index = iterator->current_hook_index;
                val current_hook = hooks[current_index];
                //Create first element:
                var first_hook = new_str_hook(current_hook->source_string);
                first_hook->start = iterator->position;
                first_hook->end = current_hook->end;
                if (is_not_empty_hooked_str(first_hook)) {
                    add_str_hook_in_list(result_chain, first_hook);
                }
                //Check rest hooks:
                val has_rest_hooks = current_index < chain_size - 1;
                if (has_rest_hooks) {
                    val next_position = current_index + 1;
                    for (unsigned j = next_position; j < chain_size; ++j) {
                        add_str_hook_in_list(result_chain, new_str_hook_by_other(hooks[j]));
                    }
                }
                return result_chain;
            }
        }
    } else {
        return new_str_hook_chain();
    }
}

str_hook_chain *explore_str_hook_chain_by_borders(
        str_hook_chain *src_chain,
        const int start_position,
        const char left_border,
        const char right_border
)
{
#ifdef STRING_HOOK_DEBUG
    log_info_str_hook_chain(STRING_HOOK_TAG, "INPUT CHAIN:", src_chain);
#endif
    if (src_chain->size > 0) {
        val hooks = src_chain->hooks;
        //Prepare to find bounds:
        var iterator = new_str_hook_iterator();
        iterator->str_hook_list = src_chain;
        iterator->current_hook_index = 0;
        iterator->position = hooks[0]->start;
        while (iterator->position < start_position) {
            val was_next = next_in_str_hook_iterator(iterator);
            if (!was_next) {
                throw_error_with_tag(STRING_HOOK_TAG, "Invalid start position!");
            }
        }
        var parenthesis_watcher = new_point_watcher();
        var is_found_start_index = FALSE;
        var is_found_end_index = FALSE;
        while (TRUE) {
            val current_hook = hooks[iterator->current_hook_index];
            val source_string = current_hook->source_string;
            val current_position = (unsigned int) iterator->position;
            val symbol = source_string[current_position];
#ifdef STRING_HOOK_DEBUG
            log_info_char(STRING_HOOK_TAG, "GET CHAR:", symbol);
#endif
            //Check symbol:
            val is_opening_border = symbol == left_border;
            val is_closing_border = symbol == right_border;
            //Meet open brace:
            if (is_opening_border) {
                //Begin reading:
                if (parenthesis_watcher->mode == POINT_WATCHER_PASSIVE_MODE) {
                    next_in_str_hook_iterator(iterator);
                    parenthesis_watcher->start = iterator->position;
#ifdef STRING_HOOK_DEBUG
                    log_info_int(STRING_HOOK_TAG, "START:", parenthesis_watcher->start);
#endif
                    parenthesis_watcher->mode = POINT_WATCHER_ACTIVE_MODE;
                    is_found_start_index = TRUE;
                }
                if (parenthesis_watcher->mode == POINT_WATCHER_ACTIVE_MODE) {
                    parenthesis_watcher->pointer++;
                }
            }
            if (is_closing_border) {
                if (parenthesis_watcher->mode == POINT_WATCHER_ACTIVE_MODE) {
                    parenthesis_watcher->pointer--;
                    //Stop reading:
                    val is_last_close_brace = parenthesis_watcher->pointer == 0;
                    if (is_last_close_brace) {
                        parenthesis_watcher->end = current_position;
#ifdef STRING_HOOK_DEBUG
                        log_info_int(STRING_HOOK_TAG, "END:", parenthesis_watcher->end);
#endif
                        is_found_end_index = TRUE;
                        break;
                    }
                }
            }
            val was_next = next_in_str_hook_iterator(iterator);
            if (!was_next) {
                break;
            }
        }
#ifdef STRING_HOOK_DEBUG
        log_info_int(STRING_HOOK_TAG, "/START:", parenthesis_watcher->start);
        log_info_int(STRING_HOOK_TAG, "/END:", parenthesis_watcher->end);
#endif
        if (is_found_start_index && is_found_end_index) {
            var in_border_hook_chain = new_str_hook_chain_by_other_with_start_and_end(
                    src_chain,
                    parenthesis_watcher->start,
                    parenthesis_watcher->end
            );
            //----------------------------------------------------------------------------------------------------------
            //GC:
            free_point_watcher(parenthesis_watcher);
            return in_border_hook_chain;
        } else {
            throw_error_with_tag(STRING_HOOK_TAG, "Bounds not found!");
        }
    } else {
        throw_error_with_tag(STRING_HOOK_TAG, "Empty chain!");
    }
}

double str_hook_to_double(const struct str_hook *hook)
{
    const static char CHAR_SHIFT = '0';
    const static int DIGIT_SHIFT = 10;
    const_string string = hook->source_string;
    int integer_part = 0;
    int i = hook->start;
    while (!is_dot(string[i])) {
        integer_part = integer_part * DIGIT_SHIFT + (string[i++] - CHAR_SHIFT);
    }
    double fraction_part = 0.0;
    double fraction_counter = DIGIT_SHIFT;
    for (int j = i + 1; j < hook->end; ++j) {
        const double digit = ((double) (string[j] - CHAR_SHIFT) / fraction_counter);
        fraction_part += digit;
        fraction_counter *= DIGIT_SHIFT;
    }
    return ((double) integer_part) + fraction_part;
}
