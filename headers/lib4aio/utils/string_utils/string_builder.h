#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

typedef struct string_builder {
    char *string_value;
    unsigned capacity;
    unsigned length;
} string_builder;

struct string_builder *new_string_builder();

void append_char_to(struct string_builder *builder, const char c);

void append_string(struct string_builder *builder, const char *string);

char *pop_string_from_builder(struct string_builder *builder);

void reset_string_builder(struct string_builder *builder);

void free_string_builder(struct string_builder *builder);

#endif //STRING_BUILDER_H
