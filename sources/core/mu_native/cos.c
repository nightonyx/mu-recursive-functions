#include <math.h>
#include <core/mu_function/mu_function_definition.h>
#include <core/mu_function_managment/mu_function_manager.h>
#include <lib4aio/utils/error_utils/error_utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>

/**
 * Calculates 'cos' function.
 * @param variable_list - double variables
 * @return result.
 */

static double cos_function(double_list *variable_list)
{
    if (variable_list->size == 1) {
        return cos(variable_list->values[0]);
    } else {
        throw_error("Illegal args number in cos!");
    }
}

/**
 * Creates 'cos' definition.
 * @return definition.
 */

mu_function_definition *new_cos_function_definition()
{
    return new_native_mu_function_definition(new_str_hook_by_string(COS_FUNCTION_NAME), cos_function);
}
