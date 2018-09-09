#include <math.h>
#include <core/mu_function_managment/mu_function_manager.h>
#include <core/mu_function/mu_function_definition.h>
#include <lib4aio/utils/error_utils/error_utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>

/**
 * Calculates 'sin' function.
 * @param variable_list - double variables
 * @return result.
 */

static double sin_function(double_list *list)
{
    if (list->size == 1) {
        return sin(list->values[0]);
    } else {
        throw_error("illegal args number in sin!");
    }
}

/**
 * Creates 'sin' definition.
 * @return definition.
 */

mu_function_definition *new_sin_function_definition()
{
    return new_native_mu_function_definition(new_str_hook_by_string(SIN_FUNCTION_NAME), sin_function);
}
