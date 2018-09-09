#include <mem.h>
#include <stdlib.h>
#include <core/mu_function/mu_function_definition.h>
#include <core/mu_function_managment/mu_function_manager.h>
#include <lib4aio/utils/boolean_utils/boolean_utils.h>
#include <lib4aio/utils/type_utils/type.utils.h>
#include <lib4aio/utils/memory_utils/memory_utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>

/**
 *  Native functions.
 */

mu_function_definition *new_cos_function_definition();

mu_function_definition *new_log_function_definition();

mu_function_definition *new_sin_function_definition();

mu_function_definition *new_sqrt_function_definition();

/**
 * Function manager.
 */

#define MU_FUNCTION_MANAGER_TAG "MU_FUNCTION_MANAGER"

mu_function_manager *function_manager;

/**
 * Creates built-in functions.
 * @return root mu function definitions.
 */

static mu_function_definition_list *create_mu_native_function_definitions()
{
    var definitions = new_mu_function_definition_list();
    //Load native_function functions:
    add_mu_function_definition_in_list(definitions, new_cos_function_definition());
    add_mu_function_definition_in_list(definitions, new_log_function_definition());
    add_mu_function_definition_in_list(definitions, new_sin_function_definition());
    add_mu_function_definition_in_list(definitions, new_sqrt_function_definition());
    return definitions;
}

/**
 * Create function manager.
 */

void start_mu_function_manager()
{
    function_manager = new_object(sizeof(mu_function_manager));
    function_manager->definition_list = create_mu_native_function_definitions();
}

/**
 * Destroy function manager.
 */

void finish_mu_function_manager()
{
    free_mu_function_definition_list(function_manager->definition_list);
    free(function_manager->source_code);
    free(function_manager);
}

/**
 * Checks validity or variable name.
 * @param name
 * @return true/false.
 */

boolean can_use_name(str_hook *hook)
{
    return is_word_hooked(hook)
           && !is_hook_equals_str(hook, COS_FUNCTION_NAME)
           && !is_hook_equals_str(hook, LOG_FUNCTION_NAME)
           && !is_hook_equals_str(hook, SIN_FUNCTION_NAME)
           && !is_hook_equals_str(hook, SQRT_FUNCTION_NAME);
}
