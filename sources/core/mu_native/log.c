#include <math.h>
#include <core/mu_function_managment/mu_function_manager.h>
#include <core/mu_function/mu_function_definition.h>
#include <lib4aio/utils/error_utils/error_utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>
#include <lib4aio/utils/type_utils/type.utils.h>

/**
 * Calculates 'log' function.
 * @param variable_list - double variables
 * @return result.
 */

static double log_function(double_list *list)
{
    if (list->size == 1) {
        val value = list->values[0];
        if (value > 0) {
            return log(value);
        } else {
            throw_error("Log value must be more than zero!");
        }
    } else {
        throw_error("Illegal args number in log!");
    }
}

/**
 * Creates 'log' definition.
 * @return definition.
 */

mu_function_definition *new_log_function_definition()
{
    return new_native_mu_function_definition(new_str_hook_by_string(LOG_FUNCTION_NAME), log_function);
}
