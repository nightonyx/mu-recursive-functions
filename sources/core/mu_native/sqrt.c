#include <math.h>
#include <core/mu_function_managment/mu_function_manager.h>
#include <core/mu_function/mu_function_definition.h>
#include <lib4aio/utils/error_utils/error_utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>
#include <lib4aio/utils/type_utils/type.utils.h>
#include <lib4aio/collections/lists/double_list.h>

#define MU_SQRT_TAG "MU_SQRT"

/**
 * Calculates square root function.
 * @param variable_list - double variables
 * @return result.
 */

static double sqrt_function(double_list *list)
{
    if (list->size == 1) {
        val value = list->values[0];
        if (value >= 0) {
            return sqrt(value);
        } else {
            throw_error_with_tag(MU_SQRT_TAG, "Can not calc sqrt from negative value!");
        }
    } else {
        throw_error_with_tag(MU_SQRT_TAG, "Illegal args number in sqrt!");
    }
}

/**
 * Creates 'sqrt' definition.
 * @return definition.
 */

mu_function_definition *new_sqrt_function_definition()
{
    return new_native_mu_function_definition(new_str_hook_by_string(SQRT_FUNCTION_NAME), sqrt_function);
}
