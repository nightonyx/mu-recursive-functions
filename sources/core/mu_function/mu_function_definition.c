#include <mem.h>
#include <malloc.h>
#include <core/mu_function/mu_function_definition.h>
#include <core/mu_variable/mu_variable.h>
#include <lib4aio/utils/memory_utils/memory_utils.h>
#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/utils/type_utils/type.utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>
#include <lib4aio/utils/error_utils/error_utils.h>
#include <lib4aio/collections/lists/double_list.h>

/**
 * Creates new function definition.
 * @param function_name - name.
 * @param type custom / native.
 * @return definition.
 */

static mu_function_definition *new_mu_function_definition(
        str_hook *function_name,
        mu_function_type type
)
{
    mu_function_definition *definition = new_object(sizeof(mu_function_definition));
    definition->name = function_name;
    definition->type = type;
    return definition;
}

/**
 * Deletes definition.
 * @param function_definition - current definiton.
 */

static void free_mu_function_definition(mu_function_definition *function_definition)
{
    free_str_hook(function_definition->name);
    free(function_definition);
}

/**
 * Creates custom defunition.
 * @param function_name - name;
 * @param arg_list argument names;
 * @param expression_chain - expression of function.
 * @return definition.
 */

mu_function_definition *new_custom_mu_function_definition(
        str_hook *function_name,
        str_hook_list *arg_list,
        str_hook_list *expression_chain
)
{
    var definition = new_mu_function_definition(function_name, MU_FUNCTION_CUSTOM);
    //Create custom attributes for function definition:
    mu_custom_function_attributes *custom_attributes = new_object(sizeof(mu_custom_function_attributes));
    custom_attributes->arg_list = arg_list;
    custom_attributes->expression = expression_chain;
    //Set attributes:
    definition->get.custom_attributes = custom_attributes;
    return definition;
}

/**
 * Deletes custom function definition.
 * @param definition.
 */

void free_custom_mu_function_definition(mu_function_definition *definition)
{
    mu_custom_function_attributes *attributes = definition->get.custom_attributes;
    var arg_list = attributes->arg_list;
    var expression = attributes->expression;
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free_str_hooks_in_list(arg_list);
    free_str_hook_list(arg_list);
    free_str_hooks_in_list(expression);
    free_str_hook_list(expression);
    free(attributes);
    free_mu_function_definition(definition);
}

/**
 * Creates native function definition
 * @param function_name - name.
 * @param calc - native function
 * @return definition.
 */

mu_function_definition *new_native_mu_function_definition(
        str_hook *function_name,
        double (*calc)(double_list *)
)
{
    mu_function_definition *definition = new_mu_function_definition(
            function_name,
            MU_FUNCTION_NATIVE
    );
    definition->get.native_function = calc;
    return definition;
}

#define INIT_CAPACITY 2

/**
 * Creates new definition list.
 * @return list.
 */

mu_function_definition_list *new_mu_function_definition_list()
{
    mu_function_definition_list *list = new_object(sizeof(mu_function_definition_list));
    list->capacity = INIT_CAPACITY;
    list->size = 0;
    list->definitions = new_object_array(INIT_CAPACITY, sizeof(mu_function_definition *));
    return list;
}

/**
 * Increases capacity and memory in list.
 * @param list
 */

static void update_memory_in_mu_function_definition_list(mu_function_definition_list *list)
{
    if (list->size + 1 == list->capacity) {
        list->capacity *= INIT_CAPACITY;
        list->definitions = reallocate_object_array(
                list->definitions,
                list->capacity,
                sizeof(mu_function_definition_list *)
        );
    }
}

/**
 * Adds definition in list.
 * @param list - current list.
 * @param definition - current definition.
 */

void add_mu_function_definition_in_list(mu_function_definition_list *list, mu_function_definition *definition)
{
    val definitions = list->definitions;
    val size = list->size;
    for (unsigned i = 0; i < size; ++i) {
        val is_definition_already_exist = are_equal_hooked_str(
                definitions[i]->name,
                definition->name
        );
        if (is_definition_already_exist) {
            return;
        }
    }
    update_memory_in_mu_function_definition_list(list);
    list->definitions[list->size++] = definition;
}

/**
 * Search definition in list by name.
 * @param list - current list
 * @param name - current name
 * @return definition.
 */

mu_function_definition *get_mu_function_definition_in_list_by_name(mu_function_definition_list *list, str_hook *name)
{
    val definitions = list->definitions;
    val size = list->size;
    for (unsigned i = 0; i < size; ++i) {
        val has_definition = are_equal_hooked_str(
                definitions[i]->name,
                name
        );
        if (has_definition) {
            return list->definitions[i];
        }
    }
    return NULL;
}

/**
 * Delete definition list.
 * @param list.
 */

void free_mu_function_definition_list(mu_function_definition_list *list)
{
    val size = list->size;
    for (unsigned i = 0; i < size; ++i) {
        free_mu_function_definition(list->definitions[i]);
    }
    free(list);
}
