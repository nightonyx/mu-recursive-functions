#include <stdlib.h>
#include <stdio.h>
#include <lib4aio/utils/error_utils/error_utils.h>
#include <lib4aio/utils/string_utils/string_utils.h>

void throw_error(const_string message)
{
    perror(message);
    free((void *) message);
    exit(1);
}

void throw_error_with_tag(const_string tag, const_string message)
{
    printf("\n%s: %s: %s\n", ERROR_TAG, tag, message);
    free((void *) message);
    exit(1);
}

void throw_error_with_details(const_string tag, const_string message, const_string value)
{
    printf("\n%s %s: %s %s\n", ERROR_TAG, tag, message, value);
    free((void *) message);
    exit(1);
}