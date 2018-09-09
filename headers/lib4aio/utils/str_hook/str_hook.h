#ifndef STRING_HOOK_H
#define STRING_HOOK_H

/**
 * String hook.
 */

typedef struct str_hook
{
    char *source_string;
    int start;
    int end;
} str_hook;

/**
 * Functions.
 */

struct str_hook *new_str_hook(char *source_ref);

void free_str_hook(struct str_hook *hook);

/**
 * String hook list.
 */

typedef struct str_hook_list
{
    unsigned capacity;
    unsigned size;
    struct str_hook **hooks;
} str_hook_list;

typedef str_hook_list str_hook_chain;

/**
 * Functions.
 */

struct str_hook_list *new_str_hook_list();

void add_str_hook_in_list(struct str_hook_list *list, struct str_hook *hook);

void free_str_hooks_in_list(struct str_hook_list *list);

void free_str_hook_list(struct str_hook_list *list);

/**
 * String hook iterator.
 */

typedef struct str_hook_iterator
{
    struct str_hook_list *str_hook_list;
    unsigned current_hook_index;
    int position;
} str_hook_iterator;

/**
 * Functions.
 */

struct str_hook_iterator *new_str_hook_iterator();

struct str_hook_iterator *new_str_hook_iterator_by_list(
        struct str_hook_list *str_hook_chain,
        const unsigned current_hook_index
);

_Bool next_in_str_hook_iterator(struct str_hook_iterator *iterator);

_Bool back_in_str_hook_iterator(struct str_hook_iterator *iterator);

void free_str_hook_iterator(struct str_hook_iterator *iterator);

/**
 * Chain utils.
 */

struct str_hook_list *new_str_hook_chain();

int get_str_hook_chain_start(struct str_hook_list *list);

int get_str_hook_chain_end(struct str_hook_list *list);

int get_str_hook_chain_position(struct str_hook_list *list, const int number_of_steps);

void free_str_hook_chain(struct str_hook_list *chain);

/**
 * Chain list.
 */

typedef struct str_hook_chain_list
{
    unsigned capacity;
    unsigned size;
    struct str_hook_list **chains;
} str_hook_chain_list;

/**
 * Functions.
 */

struct str_hook_list *new_str_hook_chain_list();

void add_str_hook_chain_in_list(struct str_hook_chain_list *list, struct str_hook_list *chain);

void free_str_hook_chains_in_list(struct str_hook_chain_list *list);

void free_str_hook_chain_list(struct str_hook_chain_list *list);

#endif //STRING_HOOK_H
