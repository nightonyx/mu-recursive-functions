#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib4aio/utils/string_utils/string_utils.h>
#include <lib4aio/utils/boolean_utils/boolean_utils.h>
#include <lib4aio/utils/char_utils/char_utils.h>

static string abstract_border_trim(
        const_string src,
        const int has_left,
        const int has_right,
        boolean (*trim_condition)(const char)
)
{
    char *dst;
    const size_t src_length = strlen(src);
    if (src_length == 0) {
        dst = calloc(2, sizeof(char));
        if (dst == NULL) {
            perror("cannot allocate memory for dst in trim_start!");
            exit(1);
        }
        return dst;
    }
    unsigned left_border = 0;
    int right_border = 0;
    if (has_left == TRUE) {
        while (trim_condition(src[left_border])) {
            left_border++;
        }
    }
    if (has_right == TRUE) {
        while (trim_condition(src[src_length - right_border - 1])) {
            right_border++;
        }
    }
    size_t number_of_elements;
    if (left_border < src_length - 1) {
        number_of_elements = src_length - left_border - right_border + 1;
    } else {
        number_of_elements = 2;
    }
    dst = calloc(number_of_elements + 1, sizeof(char));
    if (dst == NULL) {
        perror("cannot allocate memory for dst in trim_start!");
        exit(1);
    }
    unsigned i;
    for (i = left_border; i < src_length - right_border; ++i) {
        dst[i - left_border] = src[i];
    }
    dst[src_length - left_border] = '\0';
    return dst;
}

string trim_start(const_string src)
{
    return abstract_border_trim(src, TRUE, FALSE, is_space);
}

string trim_end(const_string src)
{
    return abstract_border_trim(src, FALSE, TRUE, is_space);
}

string trim(const_string src)
{
    return abstract_border_trim(src, TRUE, TRUE, is_space);
}

string_array trim_all(string_array strings, const int number_of_strings)
{
    char **dst;
    dst = calloc((size_t) number_of_strings, sizeof(string));
    for (int i = 0; i < number_of_strings; ++i) {
        dst[i] = trim(strings[i]);
    }
    return dst;
}

string trim_start_with_line_break(const_string src)
{
    return abstract_border_trim(src, TRUE, FALSE, is_space_or_line_break);
}

string trim_end_with_line_break(const_string src)
{
    return abstract_border_trim(src, FALSE, TRUE, is_space_or_line_break);
}

string trim_with_line_break(const_string src)
{
    return abstract_border_trim(src, TRUE, TRUE, is_space_or_line_break);
}

string_array trim_all_with_line_break(string_array strings, const int number_of_strings)
{
    char **dst = calloc((size_t) number_of_strings, sizeof(string));
    for (int i = 0; i < number_of_strings; ++i) {
        dst[i] = trim_with_line_break(strings[i]);
    }
    return dst;
}
