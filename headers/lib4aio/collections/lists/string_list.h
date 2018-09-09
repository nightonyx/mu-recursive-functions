#ifndef STRING_LIST_H
#define STRING_LIST_H

typedef struct string_list {
    unsigned capacity;
    unsigned size;
    char **strings;
} string_list;

struct string_list *new_string_list();

void add_string_in_list(struct string_list *string_list, char *source);

void free_strings_in_list(struct string_list *list);

void free_string_list(struct string_list *list);

#endif //STRING_LIST_H
