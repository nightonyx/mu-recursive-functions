#include <stdio.h>
#include <process.h>
#include <mem.h>
#include <ctype.h>
#include <malloc.h>
#include <core/mu_result/mu_result.h>
#include <core/mu_function/mu_function.h>
#include <core/mu_variable/mu_variable.h>
#include <core/mu_function_managment/mu_function_manager.h>
#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>
#include <lib4aio/utils/error_utils/error_utils.h>
#include <lib4aio/utils/type_utils/type.utils.h>
#include <lib4aio/utils/log_utils/log_utils.h>
#include <lib4aio/utils/boolean_utils/boolean_utils.h>
#include <lib4aio/utils/char_utils/char_utils.h>
#include <lib4aio/collections/lists/double_list.h>

#define MU_DOUBLE_PARSER_DEBUG

#ifdef MU_DOUBLE_PARSER_DEBUG

#endif

#define MU_DOUBLE_PARSER_TAG "MU_DOUBLE_PARSER"

mu_function_manager *function_manager;

/**
 * Parses expression.
 * @param expression_chain - function expression.
 * @param variable_list - table of variables.
 * @return double value.
 */

double parse_expression(str_hook_chain *expression_chain, mu_variable_list *variable_list);

/**
 * Extracts double.
 * @param expression_chain
 * @return local result.
 */

static mu_result *make_double(str_hook_chain *expression_chain)
{
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "Make double for expression:", expression_chain);
#endif
    val source_code = function_manager->source_code;
    val start_position = get_str_hook_chain_start(expression_chain);
    var iterator = new_str_hook_iterator();
    iterator->str_hook_list = expression_chain;
    iterator->current_hook_index = 0;
    iterator->position = start_position;
    while (TRUE) {
        val symbol = source_code[iterator->position];
#ifdef MU_DOUBLE_PARSER_DEBUG
        log_info_char(MU_DOUBLE_PARSER_TAG, "DOUBLE SYMBOL", symbol);
#endif
        if (is_sign(symbol)) {
            break;
        } else {
            val was_next = next_in_str_hook_iterator(iterator);
            if (!was_next) {
                break;
            }
        }
    }
    back_in_str_hook_iterator(iterator);
    var captured_double_hook = new_str_hook(source_code);
    captured_double_hook->start = start_position;
    captured_double_hook->end = iterator->position + 1;
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook(MU_DOUBLE_PARSER_TAG, "CAPTURED DOUBLE:", captured_double_hook);
#endif
    double value = 0;
    if (is_double_hooked(captured_double_hook)) {
        value = str_hook_to_double(captured_double_hook);
    } else {
        throw_error_with_hook(MU_DOUBLE_PARSER_TAG, "Can not define value of expression!", captured_double_hook);
    }
    next_in_str_hook_iterator(iterator);
    var rest_part = new_str_hook_chain_by_other_with_start(expression_chain, iterator->position);
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free_str_hook(captured_double_hook);
    return new_mu_result(value, rest_part);
}

/**
 * Prepares args for inner function and shifts rest function expression position.
 * @param expression_chain - rest function expression.
 * @param inner_expressions - fill this with inner expressions for inner function args.
 * @param next_point - rest function expression position after inner function closing parenthesis.
 */

void make_expression_chunks_and_count_next_point(
        str_hook_chain *expression_chain,
        str_hook_list *inner_expressions,
        int *next_point
)
{
    val source_code = function_manager->source_code;
    val iterator = new_str_hook_iterator();
    iterator->str_hook_list = expression_chain;
    iterator->position = get_str_hook_chain_start(expression_chain);
    //Skip first opening parenthesis:
    next_in_str_hook_iterator(iterator);
    var parenthesis_up_downer = 1;
    val start_position = iterator->position;
    int last_pointer = start_position;
    while (TRUE) {
        val symbol = source_code[iterator->position];
        if (is_opening_parenthesis(symbol)) {
            parenthesis_up_downer++;
        }
        if (is_closing_parenthesis(symbol)) {
            parenthesis_up_downer--;
            if (parenthesis_up_downer == 0) {
                var inner_expression = new_str_hook_chain_by_other_with_start_and_end(
                        expression_chain,
                        last_pointer,
                        iterator->position
                );
                if (inner_expression->size > 0) {
                    add_str_hook_chain_in_list(inner_expressions, inner_expression);
                } else {
                    //--------------------------------------------------------------------------------------------------
                    //GC:
                    free_str_hook_chain(inner_expression);
                    //--------------------------------------------------------------------------------------------------
                }
                next_in_str_hook_iterator(iterator);
                *next_point = iterator->position;
                return;
            }
            if (parenthesis_up_downer < 0) {
                throw_error_with_tag(MU_DOUBLE_PARSER_TAG, "Invalid parenthesis placement!");
            }
        }
        if (is_comma(symbol) && parenthesis_up_downer == 1) {
            var inner_expression = new_str_hook_chain_by_other_with_start_and_end(
                    expression_chain,
                    last_pointer,
                    iterator->position
            );
            add_str_hook_chain_in_list(inner_expressions, inner_expression);
            next_in_str_hook_iterator(iterator);
            last_pointer = iterator->position;
            continue;
        }
        if (!next_in_str_hook_iterator(iterator)) {
            break;
        }
    }
}

/**
 * Counts value in inner function or get variable value.
 * @param expression_chain - rest function expression.
 * @param variable_list - table of variables.
 * @return local result.
 */

mu_result *make_function_or_variable(
        str_hook_chain *expression_chain,
        mu_variable_list *variable_list
)
{
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "Make function or varibale for expression:", expression_chain);
#endif
    val source_code = function_manager->source_code;
    val start_position = get_str_hook_chain_start(expression_chain);
    //Create str_hook_iterator:
    str_hook_iterator *iterator = new_str_hook_iterator();
    iterator->str_hook_list = expression_chain;
    iterator->current_hook_index = 0;
    iterator->position = start_position;
    while (TRUE) {
        val symbol = source_code[iterator->position];
#ifdef MU_DOUBLE_PARSER_DEBUG
        log_info_char(MU_DOUBLE_PARSER_TAG, "FV::CHAR:", symbol);
#endif
        //Check symbol:
        val is_letter = isalpha(symbol);
        val is_digit = isdigit(symbol);
        if (is_letter || (is_digit && iterator->position != start_position)) {
            val was_shift = next_in_str_hook_iterator(iterator);
            if (!was_shift) {
                break;
            }
        } else {
            break;
        }
    }
    val stop_position = iterator->position;
    back_in_str_hook_iterator(iterator);
    //Must be whole hook:
    var function_or_variable_hook = new_str_hook_with_start_and_end(
            source_code,
            start_position,
            iterator->position + 1
    );
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook(MU_DOUBLE_PARSER_TAG, "FUNCTION OR VARIABLE HOOK:", function_or_variable_hook);
#endif
    val is_variable_or_function_name = is_word_hooked(function_or_variable_hook);
    if (is_variable_or_function_name) {
        val is_function_found = is_opening_parenthesis(source_code[stop_position]);
        if (is_function_found) {
#ifdef MU_DOUBLE_PARSER_DEBUG
            log_info(MU_DOUBLE_PARSER_TAG, "Found function!");
#endif
            val function_name = function_or_variable_hook;
            var in_function_parenthesis = new_str_hook_chain_by_other_with_start(
                    expression_chain,
                    function_or_variable_hook->end
            );
            //Prepare to invoke function:
            var inner_expressions = new_str_hook_list();
            int next_expression_point = 0;
            make_expression_chunks_and_count_next_point(
                    in_function_parenthesis,
                    inner_expressions,
                    &next_expression_point
            );
            val number_of_chunks = inner_expressions->size;
            var expression_hooks = inner_expressions->hooks;
            double_list *input_values = new_double_list();
            for (unsigned j = 0; j < number_of_chunks; ++j) {
                val inner_argument_value = parse_expression(expression_hooks[j], variable_list);
#ifdef MU_DOUBLE_PARSER_DEBUG
                log_info_double(MU_DOUBLE_PARSER_TAG, "INNER ARGUMENT IS READY:", inner_argument_value);
#endif
                add_in_double_list(input_values, inner_argument_value);
            }
#ifdef MU_DOUBLE_PARSER_DEBUG
            for (unsigned i = 0; i < input_values->size; ++i) {
                log_info_double(MU_DOUBLE_PARSER_TAG, "D:", input_values->values[i]);
            }
#endif
            val double_function_result = invoke_mu_function(function_name, input_values);
            str_hook_chain *rest = new_str_hook_chain_by_other_with_start(expression_chain, next_expression_point);
            return new_mu_result(double_function_result, rest);
        } else {
#ifdef MU_DOUBLE_PARSER_DEBUG
            log_info(MU_DOUBLE_PARSER_TAG, "Found variable!");
#endif
            val variable_name = function_or_variable_hook;
            if (can_use_name(variable_name)) {
                val variable = get_mu_variable_in_list_by_name(variable_list, variable_name);
                if (variable) {
                    val double_value = variable->value;
                    var rest_hook_chain = new_str_hook_chain_by_other_with_start(expression_chain, variable_name->end);
#ifdef MU_DOUBLE_PARSER_DEBUG
                    log_info_double(MU_DOUBLE_PARSER_TAG, "VARIABLE VALUE:", double_value);
                    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "FV::REST CHAIN", rest_hook_chain);
                    log_info(MU_DOUBLE_PARSER_TAG, "--------------------------------------------------------------");
#endif
                    return new_mu_result(double_value, rest_hook_chain);;
                } else {
                    throw_error_with_hook(MU_DOUBLE_PARSER_TAG, "There is not variable name (NULL):", variable_name);
                }
            } else {
                throw_error_with_hook(MU_DOUBLE_PARSER_TAG, "There is not variable name:", variable_name);
            }
        }
    }
    return make_double(expression_chain);
}


/**
 * Count value in parentheses scope.
 * @param expression_chain - rest function expression.
 * @param variable_list - table of variables.
 * @return local result.
 */

mu_result *make_parentheses(
        str_hook_chain *expression_chain,
        mu_variable_list *variable_list
)
{
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "Make parenthesis for expression:", expression_chain);
#endif
    val source_code = function_manager->source_code;
    val first_position = get_str_hook_chain_start(expression_chain);
    val first_symbol = source_code[first_position];
    if (is_opening_parenthesis(first_symbol)) {
        //Prepare to explore parenthesis bounds:
        var in_parenthesis_chain = explore_str_hook_chain_by_borders(
                expression_chain,
                first_position,
                '(',
                ')'
        );
#ifdef MU_DOUBLE_PARSER_DEBUG
        log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "IN PARENTHESIS CHAIN:", in_parenthesis_chain);
#endif
        //Get value into parenthesis:
        val in_parenthesis_double_value = parse_expression(in_parenthesis_chain, variable_list);
#ifdef MU_DOUBLE_PARSER_DEBUG
        log_info_double(MU_DOUBLE_PARSER_TAG, "IN PARENTHESIS COMPLETE!!!", in_parenthesis_double_value);
#endif
        val end_parenthesis = get_str_hook_chain_end(in_parenthesis_chain) + 1;
        var next_hook_chain = new_str_hook_chain_by_other_with_start(expression_chain, end_parenthesis);
        //Create result:
        return new_mu_result(in_parenthesis_double_value, next_hook_chain);
    }
    return make_function_or_variable(expression_chain, variable_list);
}

/**
 * Counts multiplication or division operation.
 * @param expression_chain - rest function expression.
 * @param variable_list - table of variables.
 * @return local result.
 */

static mu_result *make_multiplication_or_division(
        str_hook_chain *expression_chain,
        mu_variable_list *variable_list
)
{
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "Make multiplication or division for expression:", expression_chain);
#endif
    val source_code = function_manager->source_code;
    var left_result = make_parentheses(expression_chain, variable_list);
    var left_chain = new_str_hook_chain_by_other(left_result->rest);
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "After left mul or div chain:", left_chain);
#endif
    double left_acc = left_result->acc;
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free_mu_result(left_result);
    //------------------------------------------------------------------------------------------------------------------
    while (left_chain->size != 0) {
        val start_position = get_str_hook_chain_start(left_chain);
        val symbol = source_code[start_position];
        //Check symbol:
        val is_multiply = is_multiply_sign(symbol);
        val is_division = is_division_sign(symbol);
        if (is_multiply || is_division) {
            //Create after sign part:
            var next_hook_chain = new_str_hook_chain_by_other_with_start_and_step(
                    left_chain,
                    start_position,
                    1
            );
            //Find value after sign part:
            mu_result *right_result = make_parentheses(next_hook_chain, variable_list);
            val right_acc = right_result->acc;
            if (is_multiply) {
                left_acc *= right_acc;
#ifdef MU_DOUBLE_PARSER_DEBUG
                log_info_double(MU_DOUBLE_PARSER_TAG, "After multiply acc:", left_acc);
#endif
            }
            if (is_division) {
                if (right_acc != 0) {
                    left_acc /= right_acc;
#ifdef MU_DOUBLE_PARSER_DEBUG
                    log_info_double(MU_DOUBLE_PARSER_TAG, "After division acc:", left_acc);
#endif
                } else {
                    throw_error_with_tag(MU_DOUBLE_PARSER_TAG, "Detected division by zero!");
                }
            }
            str_hook_chain *old_left_chain = left_chain;
            left_chain = new_str_hook_chain_by_other(right_result->rest);
            //----------------------------------------------------------------------------------------------------------
            //GC:
            free_str_hooks_in_list(next_hook_chain);
            free_str_hook_list(next_hook_chain);
            free_str_hooks_in_list(old_left_chain);
            free_str_hook_list(old_left_chain);
            free_mu_result(right_result);
            //----------------------------------------------------------------------------------------------------------
        } else {
            break;
        }
    }
    var rest = new_str_hook_chain_by_other(left_chain);
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "After multiply or division rest:", rest);
#endif
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free_str_hook_chain(left_chain);
    //------------------------------------------------------------------------------------------------------------------
    return new_mu_result(left_acc, rest);
}

/**
 * Counts plus or minus operation.
 * @param expression_chain - rest function expression.
 * @param variable_list - table of variables.
 * @return local result.
 */

static mu_result *make_plus_or_minus(
        str_hook_chain *expression_chain,
        mu_variable_list *variable_list
)
{
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "Make plus or minus for expression:", expression_chain);
#endif
    val source_code = function_manager->source_code;
    var left_result = make_multiplication_or_division(expression_chain, variable_list);
    var left_chain = new_str_hook_chain_by_other(left_result->rest);
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "After left plus minus chain:", left_chain);
#endif
    double left_acc = left_result->acc;
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free_mu_result(left_result);
    //------------------------------------------------------------------------------------------------------------------
    while (left_chain->size != 0) {
        val start_position = get_str_hook_chain_start(left_chain);
        val symbol = source_code[start_position];
        //Check symbol:
        val is_plus = is_plus_sign(symbol);
        val is_minus = is_minus_sign(symbol);
        if (is_plus || is_minus) {
#ifdef MU_DOUBLE_PARSER_DEBUG
            log_info(MU_DOUBLE_PARSER_TAG, "Found plus or minus!");
#endif
            //Create after sign part:
            var next_hook_chain = new_str_hook_chain_by_other_with_start_and_step(
                    left_chain,
                    start_position,
                    1
            );
#ifdef MU_DOUBLE_PARSER_DEBUG
            log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "Next rest:", next_hook_chain);
#endif
            //Find value after sign part:
            mu_result *right_result = make_multiplication_or_division(next_hook_chain, variable_list);
            val right_acc = right_result->acc;
            if (is_plus) {
                left_acc += right_acc;
#ifdef MU_DOUBLE_PARSER_DEBUG
                log_info_double(MU_DOUBLE_PARSER_TAG, "After plus acc:", left_acc);
#endif
            }
            if (is_minus) {
                left_acc -= right_acc;
#ifdef MU_DOUBLE_PARSER_DEBUG
                log_info_double(MU_DOUBLE_PARSER_TAG, "After minus acc:", left_acc);
#endif
            }
            var old_left_chain = left_chain;
            left_chain = new_str_hook_chain_by_other(right_result->rest);
            //----------------------------------------------------------------------------------------------------------
            //GC:
            free_str_hook_chain(next_hook_chain);
            free_str_hook_chain(old_left_chain);
            free_mu_result(right_result);
            //----------------------------------------------------------------------------------------------------------
        } else {
            break;
        }
    }
    var rest = new_str_hook_chain_by_other(left_chain);
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "After plus or minus rest chain:", rest);
#endif
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free_str_hook_chain(left_chain);
    //------------------------------------------------------------------------------------------------------------------
    return new_mu_result(left_acc, rest);
}

/**
 * Parses expression.
 * @param expression_chain - function expression.
 * @param variable_list - table of variables.
 * @return double value.
 */

double parse_expression(str_hook_chain *expression_chain, mu_variable_list *variable_list)
{
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_chain(MU_DOUBLE_PARSER_TAG, "Start to parse expression:", expression_chain);
#endif
    mu_result *result = make_plus_or_minus(expression_chain, variable_list);
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_str_hook_list(MU_DOUBLE_PARSER_TAG, "CHUNKS:", result->rest);
#endif
    if (result->rest->size != 0) {
        throw_error_with_tag(MU_DOUBLE_PARSER_TAG, "Can not fully parse expression!");
    }
    val double_result = result->acc;
#ifdef MU_DOUBLE_PARSER_DEBUG
    log_info_double(MU_DOUBLE_PARSER_TAG, "Double parsing is complete:", double_result);
#endif
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free_mu_result(result);
    //------------------------------------------------------------------------------------------------------------------
    return double_result;
}
