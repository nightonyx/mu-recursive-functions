
#include <stdio.h>
#include <lib4aio/utils/string_utils/string_utils.h>

void log_info(const_string tag, const_string message)
{
    printf("\n%s: %s\n", tag, message);
}

void log_info_string(const_string tag, const_string message, const_string value)
{
    printf("\n%s: %s -%s-\n", tag, message, value);
}

void log_info_char(const_string tag, const_string message, const char value)
{
    printf("\n%s: %s -%c-\n", tag, message, value);
}

void log_info_double(const_string tag, const_string message, const double value)
{
    printf("\n%s: %s -%lf-\n", tag, message, value);
}

void log_info_int(const_string tag, const_string message, const int value)
{
    printf("\n%s: %s -%d-\n", tag, message, value);
}

void log_info_boolean(const_string tag, const_string message, const _Bool value)
{
    const_string boolean_string = NULL;
    if (value) {
        boolean_string = "true";
    } else {
        boolean_string = "false";
    }
    printf("\n%s: %s -%s-\n", tag, message, boolean_string);
}
