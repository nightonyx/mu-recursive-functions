#ifndef MU_VARIABLE_H
#define MU_VARIABLE_H

/**
 * @struct mu_variable describes variable state in mu function in runtime.
 */

typedef struct mu_variable
{
    struct str_hook *name;
    double value;
} mu_variable;

/**
 * Creates new variable.
 * @param variable_name - name.
 * @param value - double value.
 * @return variable.
 */

struct mu_variable *new_mu_variable(struct str_hook *variable_name, const double value);

/**
 * Deletes variable.
 * @param variable
 */

void free_mu_variable(struct mu_variable *variable);

/**
 * @struct mu_variable_list is a collection of @struct mu_variable.
 */

typedef struct mu_variable_list
{
    unsigned capacity;
    unsigned size;
    struct mu_variable **variables;
} mu_variable_list;

/**
 * Creates new list.
 * @return list.
 */

struct mu_variable_list *new_mu_variable_list();

/**
 * Adds variable in list.
 * @param list - current list.
 * @param variable - current variable.
 */

void add_mu_variable_in_list(struct mu_variable_list *list, struct mu_variable *variable);

/**
 * Finds variable by name in list.
 * @param list current list.
 * @param name - name of required variable.
 * @return variable.
 */

struct mu_variable *get_mu_variable_in_list_by_name(struct mu_variable_list *list, struct str_hook *name);

/**
 * Deletes list.
 * @param list.
 */

void free_mu_variable_list(struct mu_variable_list *list);

#endif //MU_VARIABLE_H
