#include <mem.h>
#include <malloc.h>
#include <stdio.h>
#include <process.h>
#include <core/mu_function/mu_function_definition.h>
#include <core/mu_function/mu_function.h>
#include <core/mu_variable/mu_variable.h>
#include <core/mu_parser/mu_parser.h>
#include <core/mu_function_managment/mu_function_manager.h>
#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/collections/lists/double_list.h>
#include <lib4aio/utils/type_utils/type.utils.h>
#include <lib4aio/utils/memory_utils/memory_utils.h>
#include <lib4aio/utils/error_utils/error_utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>

#define MU_FUNCTION_TAG "MU_FUNCTION"

#define MU_FUNCTION_DEBUG

#ifdef MU_FUNCTION_DEBUG

#include <lib4aio/utils/log_utils/log_utils.h>

#endif

mu_function_manager *function_manager;

/**
 * Performs function.
 * @param function_name - name.
 * @param input_value_list argument values.
 * @return double value.
 */

double invoke_mu_function(str_hook *function_name, double_list *input_value_list)
{
    var definition = get_mu_function_definition_in_list_by_name(
            function_manager->definition_list,
            function_name
    );
    if (definition) {
        val is_native = definition->type == MU_FUNCTION_NATIVE;
        if (is_native) {
            return definition->get.native_function(input_value_list);
        } else {
            val custom_attributes = definition->get.custom_attributes;
            //Extract arg names:
            val required_arg_list = custom_attributes->arg_list;
            val required_number_of_args = required_arg_list->size;
            val required_args = required_arg_list->hooks;
            //Extract input values:
            val input_values = input_value_list->values;
            val matches_input_and_required_args = required_number_of_args == input_value_list->size;
            if (matches_input_and_required_args) {
                //Create variables and put in parser:
                var variable_list = new_mu_variable_list();
                for (unsigned i = 0; i < required_number_of_args; ++i) {
                    var variable = new_mu_variable(required_args[i], input_values[i]);
                    add_mu_variable_in_list(variable_list, variable);
#ifdef MU_FUNCTION_DEBUG
                    log_info(MU_FUNCTION_TAG, "CREATE VARIABLE:");
                    log_info(MU_FUNCTION_TAG, "----------------");
                    log_info_str_hook(MU_FUNCTION_TAG, "<NAME>:", variable->name);
                    log_info_double(MU_FUNCTION_TAG, "<VALUE>:", variable->value);
                    log_info(MU_FUNCTION_TAG, "----------------");
#endif
                }
                val function_expression = custom_attributes->expression;
#ifdef MU_FUNCTION_DEBUG
                log_info_str_hook_chain(MU_FUNCTION_TAG, "<EXPRESSION>:", function_expression);
#endif
                //Count result:
                const double result = parse_expression(function_expression, variable_list);
                //------------------------------------------------------------------------------------------------------
                //GC:
                free_mu_variable_list(variable_list);
                //------------------------------------------------------------------------------------------------------
                return result;
            } else {
                throw_error_with_hook(
                        MU_FUNCTION_TAG,
                        "Invalid number of function arguments:",
                        function_name
                );
            }
        }
    } else {
        throw_error_with_hook(
                MU_FUNCTION_TAG,
                "Function is not found:",
                function_name
        );
    }
}
