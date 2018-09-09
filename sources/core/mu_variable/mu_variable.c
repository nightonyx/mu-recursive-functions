#include <stdlib.h>
#include <core/mu_variable/mu_variable.h>
#include <lib4aio/utils/memory_utils/memory_utils.h>
#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/utils/type_utils/type.utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>

/**
 * Creates new variable.
 * @param variable_name - name.
 * @param value - double value.
 * @return variable.
 */

mu_variable *new_mu_variable(str_hook *variable_name, const double value)
{
    mu_variable *variable = new_object(sizeof(mu_variable));
    variable->name = variable_name;
    variable->value = value;
    return variable;
}

/**
 * Deletes variable.
 * @param variable
 */

void free_mu_variable(mu_variable *variable)
{
    free(variable);
}

#define INIT_CAPACITY 2

/**
 * Creates new list.
 * @return list.
 */

mu_variable_list *new_mu_variable_list()
{
    mu_variable_list *list = new_object(sizeof(mu_variable_list));
    list->capacity = INIT_CAPACITY;
    list->size = 0;
    list->variables = new_object_array(INIT_CAPACITY, sizeof(mu_variable *));
    return list;
}

/**
 * Increases capacity and memory in list.
 * @param list
 */

static void update_memory_in_mu_variable_map(mu_variable_list *list)
{
    if (list->size + 1 == list->capacity) {
        list->capacity *= INIT_CAPACITY;
        list->variables = reallocate_object_array(
                list->variables,
                list->capacity,
                sizeof(mu_variable *)
        );
    }
}

/**
 * Adds variable in list.
 * @param list - current list.
 * @param variable - current variable.
 */

void add_mu_variable_in_list(mu_variable_list *list, mu_variable *variable)
{
    val variable_name = variable->name;
    val variables = list->variables;
    val size = list->size;
    for (unsigned i = 0; i < size; ++i) {
        val is_variable_already_exist = are_equal_hooked_str(
                variables[i]->name,
                variable_name
        );
        if (is_variable_already_exist) {
            return;
        }
    }
    update_memory_in_mu_variable_map(list);
    list->variables[list->size++] = variable;
}

/**
 * Finds variable by name in list.
 * @param list current list.
 * @param name - name of required variable.
 * @return variable.
 */

mu_variable *get_mu_variable_in_list_by_name(mu_variable_list *list, str_hook *name)
{
    val variables = list->variables;
    val size = list->size;
    for (unsigned i = 0; i < size; ++i) {
        val has_variable = are_equal_hooked_str(variables[i]->name, name);
        if (has_variable) {
            return list->variables[i];
        }
    }
    return NULL;
}

/**
 * Deletes list.
 * @param list.
 */

void free_mu_variable_list(mu_variable_list *list)
{
    val variables = list->variables;
    val size = list->size;
    for (unsigned i = 0; i < size; ++i) {
        free_mu_variable(variables[i]);
    }
    free(list);
}
