#ifndef STRING_UTILS_H
#define STRING_UTILS_H


/**
 * Typedef utils.
 */

typedef _Bool boolean;

typedef const _Bool const_boolean;

typedef char *string;

typedef const char *const_string;

typedef char **string_array;

typedef const string_array const_string_array;

/**
 * Declare functions.
 */

char *new_string(const char *src);

char *int_to_string(int src);

char *double_to_string(const double src);

char **split_by_string(const char *src, const char *delimiter);

char **split_by_space(const char *src);

char **split_by_comma(const char *src);

char **split_by_line_break(const char *src);

char *trim_start(const char *src);

char *trim_end(const char *src);

char *trim(const char *src);

char *squeeze_string_for_expression(const char *src);

char **trim_all(char **strings, const int number_of_strings);

char *trim_start_with_line_break(const char *src);

char *trim_end_with_line_break(const char *src);

char *trim_with_line_break(const char *src);

char **trim_all_with_line_break(char **strings, const int number_of_strings);

char *remove_prefix(const char *src, const char *prefix);

char *remove_suffix(const char *src, const char *suffix);

char *remove_prefix_suffix(const char *src, const char *prefix, const char *suffix);

_Bool starts_with_prefix(const char *src, const char *prefix);

_Bool ends_with_suffix(const char *src, const char *suffix);

char **filter_strings(char **src, const int src_size, _Bool (*filter_condition)(const char *));

_Bool is_not_empty_string(const char *string);

_Bool is_empty_string(const char *string);

char *join_to_string(char **src_strings, const char *delimiter, const int src_size);

_Bool is_word(const char *line);

char *substring_by_offset(const char *string, int offset, int length);

char *substring(const char *string, int start, int end);

int get_string_array_size(char **src);

_Bool matches_string(const char *src);

_Bool are_equal_strings(const char *first, const char *second);

char *boolean_to_string(const _Bool src);

void free_strings(char **src);

void free_string(char *src);

#endif //STRING_UTILS_H