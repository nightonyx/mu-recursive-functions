#ifndef STRING_HOOK_UTILS_H
#define STRING_HOOK_UTILS_H

struct str_hook *new_str_hook_with_start(char *source_string, const int start_index);

struct str_hook *new_str_hook_with_end(char *source_string, const int end_index);

struct str_hook *new_str_hook_with_start_and_end(char *source_string, const int start_index, const int end_index);

struct str_hook *new_str_hook_by_point_watcher(char *source_string, const struct point_watcher *watcher);

struct str_hook *new_str_hook_by_other(const struct str_hook *other_hook);

struct str_hook *new_str_hook_by_other_hook_string(const struct str_hook *other_hook);

struct str_hook *new_str_hook_by_string(char *source_ref);

char *substring_by_str_hook(const struct str_hook *hook);

struct str_hook *new_str_hook_by_offset(char *source_string, const int offset, const int length);

int get_str_hook_size(const struct str_hook *hook);

char get_str_hook_char(const struct str_hook *hook, const int index);

_Bool are_equal_hooked_str(const struct str_hook *hook_1, const struct str_hook *hook_2);

_Bool is_word_hooked(const struct str_hook *hook);

void reset_str_hook_positions(struct str_hook *hook);

_Bool is_hook_equals_str(const struct str_hook *hook, const char *str);

_Bool is_empty_hooked_str(const struct str_hook *hook);

_Bool is_not_empty_hooked_str(const struct str_hook *hook);

struct str_hook_list *new_str_hook_chain_by_other(struct str_hook_list *chain);

struct str_hook_list *new_str_hook_chain_by_other_with_start(struct str_hook_list *chain, const int start);

struct str_hook_list *new_str_hook_chain_by_other_with_start_and_end(
        struct str_hook_list *chain,
        const int start,
        const int end
);

struct str_hook_list *new_str_hook_chain_by_other_with_start_and_step(
        struct str_hook_list *chain,
        const int start,
        const int step
);

struct str_hook_list *explore_str_hook_chain_by_borders(
        struct str_hook_list *src_chain,
        const int start_position,
        const char left_border,
        const char right_border
);

struct str_hook_list *split_str_hook_by_string(const struct str_hook *hook, const char *delimiter);

struct str_hook_list *split_str_hook_by_comma(const struct str_hook *hook);

struct str_hook_list *split_str_hook_by_space(const struct str_hook *hook);

struct str_hook_list *split_str_hook_by_line_break(const struct str_hook *hook);

struct str_hook_list *split_str_hook_by_whitespace(const struct str_hook *hook);

struct str_hook_list *split_str_hook_by_char_condition(
        const struct str_hook *hook,
        _Bool (*char_condition)(const char)
);

struct str_hook *trim_str_hook_by_start(const struct str_hook *hook);

struct str_hook *trim_str_hook_by_end(const struct str_hook *hook);

struct str_hook *trim_str_hook(const struct str_hook *hook);

struct str_hook *trim_str_hook_with_line_break_by_start(const struct str_hook *hook);

struct str_hook *trim_str_hook_with_line_break_by_end(const struct str_hook *hook);

struct str_hook *trim_str_hook_with_line_break(const struct str_hook *hook);

struct str_hook_list *trim_str_hook_list(const struct str_hook_list *list);

struct str_hook_list *trim_str_hook_list_with_line_break(const struct str_hook_list *list);

struct str_hook_list *filter_str_hook_list(
        const struct str_hook_list *list,
        _Bool (*filter_condition)(const struct str_hook *)
);

void foreach_str_hook(const struct str_hook_list *list, void (*str_hook_action)(const struct str_hook *hook));

void log_info_str_hook(const char *tag, const char *message, const struct str_hook *hook);

void log_info_str_hook_list(const char *tag, const char *message, const struct str_hook_list *list);

void log_info_str_hook_chain(const char *tag, const char *message, const struct str_hook_list *chain);

void throw_error_with_hook(
        const char *tag,
        const char *message,
        const struct str_hook *hook
) __attribute__ ((__noreturn__));

/**
 * Primitive type matchers.
 */

_Bool is_int_hooked(const struct str_hook *hook);

_Bool is_double_hooked(const struct str_hook *hook);

_Bool is_string_hooked(const struct str_hook *hook);

_Bool is_boolean_hooked(const struct str_hook *hook);

_Bool is_null_hooked(const struct str_hook *hook);

/**
 * Casts.
 */

double str_hook_to_double(const struct str_hook *hook);

#endif //STRING_HOOK_UTILS_H
