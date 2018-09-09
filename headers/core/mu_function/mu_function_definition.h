#ifndef MU_FUNCTION_DEFINITION_H
#define MU_FUNCTION_DEFINITION_H

/**
 * Linking.
 */

#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/collections/lists/double_list.h>

 /**
  * @enum mu_function_type.
  * There are two function type: native and custom.
  * Native is a function which applied in function manager automatically.
  * Custom is a function which found in input file.
  */

typedef enum mu_function_type
{
    MU_FUNCTION_CUSTOM,
    MU_FUNCTION_NATIVE
} mu_function_type;

/**
  * @struct mu_function_definition describes function attributes:
  *
  * function name;
  *
  * type defines union:
  *
  * if @enum mu_function_type is MU_FUNCTION_CUSTOM
  * then definition contains custom attributes @see mu_custom_function_attributes.
  *
  * if @enum mu_function_type is MU_FUNCTION_NATIVE
  * then definition has native function.
  */


typedef struct mu_function_definition
{
    struct str_hook *name;

    enum mu_function_type type;

    union
    {
        /**
         * Custom function:
         */
        struct mu_custom_function_attributes *custom_attributes;

        /**
         * Native function (Examples: sin, cos, sqrt, log etc):
         */

        double (*native_function)(struct double_list *);

    } get;

} mu_function_definition;

/**
 * @struct mu_custom_function_attributes has argument names and expression.
 */

typedef struct mu_custom_function_attributes
{
    struct str_hook_list *arg_list;
    struct str_hook_list *expression;
} mu_custom_function_attributes;

/**
 * Creates custom function defunition.
 * @param function_name - name;
 * @param arg_list argument names;
 * @param expression_chain - expression of function.
 * @return definition.
 */

struct mu_function_definition *new_custom_mu_function_definition(
        struct str_hook *function_name,
        struct str_hook_list *arg_list,
        struct str_hook_list *expression_chain
);

/**
 * Deletes custom function definition.
 * @param definition.
 */

void free_custom_mu_function_definition(struct mu_function_definition *definition);

/**
 * Creates native function definition
 * @param function_name - name.
 * @param calc - native function
 * @return definition.
 */

struct mu_function_definition *new_native_mu_function_definition(
        struct str_hook *function_name,
        double (*calc)(struct double_list *)
);

/**
 * Delete native function definition.
 * @param definition.
 */

void free_native_mu_function_definition(struct mu_function_definition *definition);

/**
 * @struct mu_function_definition_list is a collection of @struct mu_function_definition.
 */

typedef struct mu_function_definition_list
{
    unsigned capacity;
    unsigned size;
    struct mu_function_definition **definitions;
} mu_function_definition_list;

/**
 * Creates new definition list.
 * @return list.
 */

struct mu_function_definition_list *new_mu_function_definition_list();

/**
 * Adds definition in list.
 * @param list - current list.
 * @param definition - current definition.
 */

void add_mu_function_definition_in_list(
        struct mu_function_definition_list *list,
        struct mu_function_definition *definition
);

/**
 * Search definition in list by name.
 * @param list - current list
 * @param name - current name
 * @return definition.
 */

struct mu_function_definition *get_mu_function_definition_in_list_by_name(
        struct mu_function_definition_list *list,
        struct str_hook *name
);

/**
 * Delete definition list.
 * @param list.
 */

void free_mu_function_definition_list(struct mu_function_definition_list *list);

#endif //MU_FUNCTION_DEFINITION_H
