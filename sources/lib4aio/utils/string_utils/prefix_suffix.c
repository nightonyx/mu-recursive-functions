#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib4aio/utils/boolean_utils/boolean_utils.h>
#include <lib4aio/utils/string_utils/string_utils.h>

int is_correct_src_prefix_suffix(const size_t src_length, const size_t prefix_length, const size_t suffix_length,
                                 const char *src, char **dst)
{
    if (src_length == 0) {
        *dst = calloc(2, sizeof(char));
        if (*dst == NULL) {
            perror("cannot allocate memory for *dst in split by char!");
            exit(1);
        }
        return FALSE;
    }
    if (prefix_length + suffix_length > src_length || (prefix_length == 0 && suffix_length == 0)) {
        *dst = calloc(src_length + 1, sizeof(char));
        if (*dst == NULL) {
            perror("cannot allocate memory for *dst in split by char!");
            exit(1);
        }
        strcpy(*dst, src);
        return FALSE;
    }
    return TRUE;
}

char *remove_prefix_suffix(const char *src, const char *prefix, const char *suffix)
{
    const size_t src_length = strlen(src);
    const size_t prefix_length = strlen(prefix);
    const size_t suffix_length = strlen(suffix);
    char *dst;
    if (is_correct_src_prefix_suffix(src_length, prefix_length, suffix_length, src, &dst) == FALSE) {
        return dst;
    }
    int left_part = 0;
    for (int j = 0; j < prefix_length; ++j) {
        if (src[j] != prefix[j]) {
            left_part = 0;
            break;
        } else {
            left_part++;
        }
    }
    int right_part = 0;
    for (int j = 0; j < suffix_length; ++j) {
        if (src[src_length - suffix_length + j] != suffix[j]) {
            right_part = 0;
            break;
        } else {
            right_part++;
        }
    }
    size_t num_of_elements = src_length - left_part - right_part;
    dst = calloc(num_of_elements + 1, sizeof(char));
    for (int i = left_part; i < src_length - right_part; ++i) {
        dst[i - left_part] = src[i];
    }
    return dst;
}

char *remove_prefix(const char *src, const char *prefix)
{
    return remove_prefix_suffix(src, prefix, "");
}

char *remove_suffix(const char *src, const char *suffix)
{
    return remove_prefix_suffix(src, "", suffix);
}

boolean starts_with_prefix(const_string src, const_string prefix)
{
    for (int i = 0; i < strlen(prefix); ++i) {
        if (src[i] != prefix[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

boolean ends_with_suffix(const_string src, const_string suffix)
{
    const size_t src_length = strlen(src);
    const size_t suffix_length = strlen(suffix);
    for (int j = 0; j < suffix_length; ++j) {
        if (src[src_length - suffix_length + j] != suffix[j]) {
            return FALSE;
        }
    }
    return TRUE;
}
