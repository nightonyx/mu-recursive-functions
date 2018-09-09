#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <lib4aio/utils/string_utils/string_utils.h>
#include <lib4aio/utils/boolean_utils/boolean_utils.h>
#include <lib4aio/utils/char_utils/char_utils.h>
#include <lib4aio/utils/string_utils/string_builder.h>

#define STRING_UTILS_TAG "STRING_UTILS"

#define EMPTY_STRING ""

string new_string(const_string src)
{
    if (src != NULL) {
        string new_string = calloc(strlen(src) + 1, sizeof(char));
        strcpy(new_string, src);
        return new_string;
    } else {
        return NULL;
    }
}

string_array filter_strings(string_array src, const int src_size, boolean (*filter_condition)(const_string))
{
    int *new_indices = calloc((size_t) src_size, sizeof(int));
    char **dst;
    size_t new_length = 0;
    //Look at values and measure new string:
    for (int i = 0; i < src_size; ++i) {
        if (filter_condition(src[i]) == 0) {
            new_indices[i] = i;
            new_length++;
        } else {
            new_indices[i] = -1;
        }
    }
    if (new_length > 0) {
        dst = calloc(new_length, sizeof(char *));
    } else {
        dst = calloc(1, sizeof(char *));
        return dst;
    }
    int new_pointer = 0;
    for (int j = 0; j < src_size; ++j) {
        if (new_indices[j] != -1) {
            dst[new_pointer] = calloc(strlen(src[j]), sizeof(char));
            dst[new_pointer] = src[j];
            new_pointer++;
        }
    }
    return dst;
}

//Passed JUnitTest!
boolean is_not_empty_string(const_string string)
{
    if (string == NULL) {
        return FALSE;
    }
    if (strcmp(string, "") != 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

//Passed JUnitTest!
boolean is_empty_string(const_string string)
{
    const_boolean result = strcmp(string, EMPTY_STRING) == 0;
    return result;
}

//준비된!
//Passed JUnitTest!
string join_to_string(string_array src_strings, const_string delimiter, const int src_size)
{
    char *dst;
    int delimiter_length = strlen(delimiter);
    //Delimiters less than values by 1:
    int current_string_length = 0 - delimiter_length;
    for (int i = 0; i < src_size; ++i) {
        //Get increase length of general string:
        current_string_length = current_string_length + strlen(src_strings[i]) + delimiter_length;
    }
    if (current_string_length == 0) {
        dst = calloc(2, sizeof(char));
        return dst;
    }
    dst = calloc((size_t) (current_string_length + 1), sizeof(char));
    if (dst == NULL) {
        perror("cannot allocate memory for dst");
    }
    int position = 0;
    for (int j = 0; j < src_size; ++j) {
        int line_length = strlen(src_strings[j]);
        for (int i = 0; i < line_length; ++i) {
            dst[position] = src_strings[j][i];
            position = position + 1;
        }
        if (j != src_size - 1) {
            for (int i = 0; i < delimiter_length; ++i) {
                dst[position] = delimiter[i];
                position = position + 1;
            }
        }
    }
    return dst;
}

//Passed JUnitTest!
boolean is_word(const_string line)
{
    int length = strlen(line);
    if (length < 1) {
        return FALSE;
    }
    if (isalpha(line[0])) {
        if (length > 1) {
            for (int i = 1; i < length; ++i) {
                if (!isalnum(line[i])) {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

//Passed JUnitTest!
string substring_by_offset(const_string string, int offset, int length)
{
    char *dst = calloc((size_t) (length + 1), sizeof(char));
    for (int i = 0; i < length; ++i) {
        dst[i] = string[offset + i];
    }
    return dst;
}

string substring(const_string string, int start, int end)
{
    const size_t dst_size = (const size_t) (end - start + 1);
    char *dst = calloc(dst_size, sizeof(char));
    int position = 0;
    for (int i = start; i < end; ++i) {
        dst[position++] = string[i];
    }
    return dst;
}

int get_string_array_size(char **src)
{
    return _msize(src) / 4;
}

string squeeze_string_for_expression(const_string src)
{
    const unsigned src_length = strlen(src);
    string dst = calloc(src_length + 1, sizeof(char));
    size_t new_length = 0;
    boolean in_quote_scope = FALSE;
    for (int i = 0; i < src_length; ++i) {
        const char symbol = src[i];
        const_boolean is_not_whitespace = !is_space_or_line_break(symbol);
        const_boolean is_quote = is_single_quote(symbol);
        if (is_quote) {
            if (!in_quote_scope) {
                in_quote_scope = TRUE;
            } else {
                in_quote_scope = FALSE;
            }
        }
        if (is_not_whitespace || in_quote_scope) {
            dst[new_length++] = symbol;
        }
    }
    string old_string = dst;
    dst = calloc(new_length, sizeof(char));
    strcpy(dst, old_string);
    free(old_string);
    return dst;
}

//Passed JUnitTest!
boolean matches_string(const_string src)
{
    boolean result = FALSE;
    int length = strlen(src);
    if (length > 1 && src[0] == '\'' && src[length - 1] == '\'') {
        result = TRUE;
    }
    return result;
}

//Passed JUnitTest!
string int_to_string(int src)
{
    int division = src;
    unsigned int_size_in_string = 0;
    while (division != 0) {
        division = division / 10;
        int_size_in_string = int_size_in_string + 1;
    }
    string integer_array;
    int negative_shift = 0;
    if (int_size_in_string > 0) {
        if (src < 0) {
            negative_shift = 1;
        }
        integer_array = calloc(int_size_in_string + 1 + negative_shift, sizeof(char));
        division = src;
        integer_array[0] = '-';
        int pointer = int_size_in_string - 1 + negative_shift;
        while (division != 0) {
            (integer_array)[pointer] = (char) (abs(division % 10) + '0');
            division = division / 10;
            pointer--;
        }
    } else {
        integer_array = calloc(2, sizeof(char));
        if (integer_array == NULL) {
            perror("cannot allocate memory for integer_array in int_to_str");
            exit(1);
        }
        integer_array[0] = '0';
        int_size_in_string = 1;
    }
    char *dst = calloc(int_size_in_string + 1 + negative_shift, sizeof(char));
    if (dst == NULL) {
        perror("cannot allocate memory for dst array!");
        exit(1);
    }
    for (int k = 0; k < int_size_in_string + negative_shift; ++k) {
        dst[k] = integer_array[k];
    }
    free(integer_array);
    return dst;
}

string double_to_string(const double src)
{
    const static int LEFT_BORDER = 0;
    const static char DOT = '.';
    const static int NANO = 9;
    const static int SHIFT = 10;
    string_builder *sb = new_string_builder();
    //Put int part:
    const int int_part = (int) src;
    const_string int_string = int_to_string(int_part);
    append_string(sb, int_string);
    //Put dot:
    append_char_to(sb, DOT);
    //Put fractional part:
    double fractional_part = src - int_part;
    for (int i = 0; i < NANO; ++i) {
        fractional_part *= SHIFT;
        const int digit = (int) fractional_part;
        const char symbol = (const char) (digit + '0');
        append_char_to(sb, symbol);
        fractional_part -= digit;
    }
    //Extract string builder accumulator:
    int right_border = sb->length - 1;
    string sb_acc = sb->string_value;
    //Shift empty cells:
    while (right_border >= 0) {
        const char symbol = sb_acc[right_border];
        if (symbol != '0') {
            break;
        } else {
            right_border--;
        }
    }
    string result = substring(sb_acc, LEFT_BORDER, right_border + 1);
    free_string_builder(sb);
    return result;
}
