#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <assert.h>
#include <stddef.h>
#include <lib4aio/utils/string_utils/string_utils.h>

static void move_string_to_dst(const char *src, const size_t string_length, char ***dst) {
    //allocate memory for string array:
    *dst = calloc(1, sizeof(string));
    if (*dst == NULL) {
        perror("Cannot allocate memory for *dst in split by char!");
        exit(1);
    }
    //allocate memory for only string:
    **dst = calloc(string_length, sizeof(char) + 1);
    if (**dst == NULL) {
        perror("Cannot allocate memory for **dst in split by char!");
        exit(1);
    }
    strcpy(**dst, src);
}

static void move_empty_string_to_dst(const char *src, char ***dst) {
    move_string_to_dst(src, 1, dst);
}

static void create_left_part(const char *src, const unsigned *indices, size_t delimiter_length, char ***dst) {
    (*dst)[0] = calloc(indices[0] - delimiter_length + 2, sizeof(char));
    if ((*dst)[0] == NULL) {
        perror("cannot allocate memory for *dst[0] in split by char!");
        exit(1);
    }
    for (int i = 0; i < indices[0] - delimiter_length; ++i) {
        (*dst)[0][i] = src[i];
    }
}

static void create_right_part(const char *src, const unsigned *indices, char ***dst, int pointers, size_t length) {
    (*dst)[pointers] = calloc(length - indices[pointers - 1] + 1, sizeof(char));
    if ((*dst)[pointers] == NULL) {
        perror("cannot allocate memory for *dst[last] in split by char!");
        exit(1);
    }
    int k = 0;
    for (int i = indices[pointers - 1]; i < length; ++i) {
        (*dst)[pointers][k] = src[i];
        k = k + 1;
    }
}

string_array split_by_comma(const_string src) {
    static const_string COMMA = ",";
    string_array result = split_by_string(src, COMMA);
    return result;
}

string_array split_by_space(const_string src) {
    static const_string SPACE = " ";
    string_array result = split_by_string(src, SPACE);
    return result;
}

string_array split_by_line_break(const_string src){
    static const_string LINE_BREAK = "\n";
    string_array result = split_by_string(src, LINE_BREAK);
    return result;
}

string_array split_by_string(const_string src, const_string delimiter) {
    char **dst;
    const size_t src_length = strlen(src);
    //Is empty string:
    if (src_length < 1 || strcmp(delimiter, "") == 0) {
        move_empty_string_to_dst(src, &dst);
        return dst;
    }
    //Create split indices:
    unsigned *indices = calloc(src_length, sizeof(int));
    if (indices == NULL) {
        perror("cannot allocate memory for indices in split by char!");
        exit(1);
    }
    const size_t delimiter_length = strlen(delimiter);
    unsigned pointers = 0;
    //Mark split points:
    for (unsigned i = 0; i < src_length; ++i) {
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
        move_string_to_dst(src, src_length, &dst);
        return dst;
    }
    indices = realloc(indices, pointers * sizeof(unsigned));
    if (indices == NULL) {
        perror("cannot allocate memory for indices in split by char!");
        exit(1);
    }
    //Parts more than pointers by 1:
    const unsigned parts = pointers + 1;
    dst = calloc(parts, sizeof(char *));
    if (dst == NULL) {
        perror("cannot allocate memory for *dst in split by char!");
        exit(1);
    }
    //Create left part:
    create_left_part(src, indices, delimiter_length, &dst);
    //Create right part:
    create_right_part(src, indices, &dst, pointers, src_length);
    //Create middle parts:
    if (pointers > 1) {
        //From second delimiter:
        for (int j = 0; j < pointers - 1; ++j) {
            if (indices[j + 1] - indices[j] - delimiter_length > 0) {
                dst[j + 1] = calloc(indices[j + 1] - indices[j] - delimiter_length + 1, sizeof(char));
                if (dst[j + 1] == NULL) {
                    perror("cannot allocate memory for *dst[j] in split by char!");
                    exit(1);
                }
                for (int i = 0; i < indices[j + 1] - indices[j] - delimiter_length; ++i) {
                    dst[j + 1][i] = src[indices[j] + i];
                }
            } else {
                dst[j + 1] = calloc(2, sizeof(char));
                if (dst[j + 1] == NULL) {
                    perror("cannot allocate memory for *dst[j] in split by char!");
                    exit(1);
                }
                dst[j + 1] = "";
            }
        }
    }
    return dst;
}