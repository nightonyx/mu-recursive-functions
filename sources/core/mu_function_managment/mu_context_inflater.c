#include <mem.h>
#include <ctype.h>
#include <stdlib.h>
#include <core/mu_function/mu_function_definition.h>
#include <core/mu_function_managment/mu_function_manager.h>
#include <lib4aio/utils/type_utils/type.utils.h>
#include <lib4aio/utils/char_utils/char_utils.h>
#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/utils/file_utils/file_reader.h>
#include <lib4aio/utils/boolean_utils/boolean_utils.h>
#include <lib4aio/utils/string_utils/string_utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>
#include <lib4aio/utils/error_utils/error_utils.h>
#include <lib4aio/utils/point_watcher/point_watcher.h>

#define MU_CONTEXT_INFLATER_TAG "MU_CONTEXT_INFLATER"

#define MU_CONTEXT_INFLATER_DEBUG

#ifdef MU_CONTEXT_INFLATER_DEBUG

#include <lib4aio/utils/log_utils/log_utils.h>

#endif

mu_function_manager *function_manager;

/**
 * Extracts function name.
 * @param source_watcher - rest string scope.
 * @param source_code - string from file.
 * @return function name.
 */

static str_hook *dig_function_name(
        point_watcher *source_watcher,
        const_string source_code
)
{
#ifdef MU_CONTEXT_INFLATER_DEBUG
    log_info(MU_CONTEXT_INFLATER_TAG, "Find function name...");
#endif
    val source_code_length = source_watcher->end;
#ifdef MU_CONTEXT_INFLATER_DEBUG
    log_info_int(MU_CONTEXT_INFLATER_TAG, "START:", source_watcher->start);
    log_info_int(MU_CONTEXT_INFLATER_TAG, "END:", source_watcher->end);
#endif
    //Scan first token:
    for (unsigned i = source_watcher->start; i < source_code_length; ++i) {
        val symbol = source_code[i];
        //Check symbol:
        val is_space_or_line_break = is_space(symbol) || is_line_break(symbol);
        val is_space_or_line_break_or_open_parenthesis = is_space_or_line_break || is_opening_parenthesis(symbol);
        val is_letter = isalpha(symbol);
        //Starts with symbol:
        if (is_letter && source_watcher->mode == POINT_WATCHER_PASSIVE_MODE) {
            source_watcher->start = i;
            source_watcher->mode = POINT_WATCHER_ACTIVE_MODE;
        }
        //Stop reading:
        if (is_space_or_line_break_or_open_parenthesis && source_watcher->mode == POINT_WATCHER_ACTIVE_MODE) {
            source_watcher->end = i;
            break;
        }
        //Skip whitespace and line breaks before watching:
        if (source_watcher->mode == POINT_WATCHER_PASSIVE_MODE) {
            if (!is_space_or_line_break) {
                throw_error_with_tag(MU_CONTEXT_INFLATER_TAG, "Name must start with letter!");
            }
        }
    }
    //Get function name string:
    var function_name = new_str_hook_by_point_watcher(source_code, source_watcher);
    val is_valid_name = can_use_name(function_name);
    if (is_valid_name) {
#ifdef MU_CONTEXT_INFLATER_DEBUG
        log_info_str_hook(MU_CONTEXT_INFLATER_TAG, "<Function name>:", function_name);
#endif
        //Set attributes for next digging:
        source_watcher->start = source_watcher->end;
        source_watcher->end = source_code_length;
        source_watcher->mode = POINT_WATCHER_PASSIVE_MODE;
        return function_name;
    } else {
        throw_error_with_hook(MU_CONTEXT_INFLATER_TAG, "Invalid mu function name:", function_name);
    }
}

/**
 * Extracts argument names.
 * @param source_watcher - rest string scope.
 * @param source_code - string from file.
 * @return list of argument names.
 */

static str_hook_list *dig_arguments(
        point_watcher *source_watcher,
        const_string source_code
)
{
    var arg_list = NULL;
    val source_code_length = source_watcher->end;
    //Search scope of arguments:
    for (unsigned i = source_watcher->start; i < source_code_length; ++i) {
        val symbol = source_code[i];
        //Begin reading:
        if (is_opening_parenthesis(symbol) && source_watcher->mode == POINT_WATCHER_PASSIVE_MODE) {
            source_watcher->start = i + 1;
            source_watcher->mode = POINT_WATCHER_ACTIVE_MODE;
        }
        //Stop reading:
        if (is_closing_parenthesis(symbol)) {
            source_watcher->end = i;
            break;
        }
        //Skip whitespace and line breaks before watching:
        if (source_watcher->mode == POINT_WATCHER_PASSIVE_MODE) {
            if (!is_space_or_line_break(symbol)) {
                throw_error_with_tag(MU_CONTEXT_INFLATER_TAG, "Arguments must start with opening parenthesis!");
            }
        }
    }
    //Get function argument content string:
    var dirty_argument_content_chunk = new_str_hook_by_point_watcher(source_code, source_watcher);
    var argument_content_chunk = trim_str_hook_with_line_break(dirty_argument_content_chunk);
#ifdef MU_CONTEXT_INFLATER_DEBUG
    log_info_str_hook(MU_CONTEXT_INFLATER_TAG, "DIRTY:", dirty_argument_content_chunk);
    log_info_str_hook(MU_CONTEXT_INFLATER_TAG, "CLEAN:", argument_content_chunk);
#endif
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free_str_hook(dirty_argument_content_chunk);
    //------------------------------------------------------------------------------------------------------------------
    val has_arguments = !is_empty_hooked_str(argument_content_chunk);
    if (has_arguments) {
        //Dig arg contents:
        str_hook_list *dirty_arg_chunks = split_str_hook_by_comma(argument_content_chunk);
        str_hook_list *clean_args = trim_str_hook_list_with_line_break(dirty_arg_chunks);
        //Assign args to result list:
        arg_list = clean_args;
        //--------------------------------------------------------------------------------------------------------------
        //GC:
        free_str_hooks_in_list(dirty_arg_chunks);
        free_str_hook_list(dirty_arg_chunks);
    } else {
        //Create empty result list:
        arg_list = new_str_hook_list();
    }
    //Set attributes for next digging:
    source_watcher->start = source_watcher->end + 1; //After parenthesis
    source_watcher->end = source_code_length;
    source_watcher->mode = POINT_WATCHER_PASSIVE_MODE;
#ifdef MU_CONTEXT_INFLATER_DEBUG
//    log_info_char(MU_CONTEXT_INFLATER_TAG, "::", source_code[source_watcher->start - 3]);
//    log_info_char(MU_CONTEXT_INFLATER_TAG, "::", source_code[source_watcher->start - 2]);
//    log_info_char(MU_CONTEXT_INFLATER_TAG, "::", source_code[source_watcher->start - 1]);
//    log_info_char(MU_CONTEXT_INFLATER_TAG, "::", source_code[source_watcher->start]);
    log_info_str_hook_list(MU_CONTEXT_INFLATER_TAG, "<ARG>:", arg_list);
#endif
    return arg_list;
}

/**
 * Extracts function body.
 * @param source_watcher - rest string scope.
 * @param source_code - string from file.
 * @return
 */

static str_hook_chain *dig_expression(
        point_watcher *source_watcher,
        const_string source_code
)
{
    val source_code_length = source_watcher->end;
    var j = source_watcher->start;
    //Find equal sign:
    for (; j < source_code_length; ++j) {
        val symbol = source_code[j];
        //Check symbol:
        val is_white_space_cond = is_space_or_line_break(symbol);
        val is_equal_sign_cond = is_equal_sign(symbol);
        if (is_white_space_cond) {
            continue;
        } else if (is_equal_sign_cond) {
            //Found equal sign:
            j++;
#ifdef MU_CONTEXT_INFLATER_DEBUG
            log_info(MU_CONTEXT_INFLATER_TAG, "FOUND EQUAL SIGN!");
#endif
            break;
        } else {
            throw_error_with_tag(MU_CONTEXT_INFLATER_TAG, "Expected equal sign after closing parenthesis...");
        }
    }
    //Prepare expression in exotic form:
    var expression_chain = new_str_hook_chain();
    for (; j < source_code_length; ++j) {
        val symbol = source_code[j];
#ifdef MU_CONTEXT_INFLATER_DEBUG
        log_info_char(MU_CONTEXT_INFLATER_TAG, "Symbol:", symbol);
#endif
        //Check symbol:
        val is_white_space_cond = is_space_or_line_break(symbol);
        val is_semicolon_cond = is_semicolon(symbol);
        if ((!is_white_space_cond && !is_semicolon_cond) && source_watcher->mode == POINT_WATCHER_PASSIVE_MODE) {
            source_watcher->start = j;
            source_watcher->mode = POINT_WATCHER_ACTIVE_MODE;
        }
        if (is_white_space_cond && source_watcher->mode == POINT_WATCHER_ACTIVE_MODE) {
            source_watcher->mode = POINT_WATCHER_PASSIVE_MODE;
            val expression_chunk = new_str_hook(source_code);
            expression_chunk->start = source_watcher->start;
            expression_chunk->end = j;
            add_str_hook_in_list(expression_chain, expression_chunk);
#ifdef MU_CONTEXT_INFLATER_DEBUG
            log_info_str_hook(MU_CONTEXT_INFLATER_TAG, "EXPRESSION CHUNK:", expression_chunk);
#endif
        }
        if (is_semicolon_cond) {
            if (source_watcher->mode == POINT_WATCHER_ACTIVE_MODE) {
                val expression_chunk = new_str_hook(source_code);
                expression_chunk->start = source_watcher->start;
                expression_chunk->end = j;
                add_str_hook_in_list(expression_chain, expression_chunk);
#ifdef MU_CONTEXT_INFLATER_DEBUG
                log_info_str_hook(MU_CONTEXT_INFLATER_TAG, "EXPRESSION CHUNK:", expression_chunk);
#endif
            }
            //Set attributes for next function:
            source_watcher->start = j + 1; //After semicolon
            source_watcher->pointer = j + 1; //After semicolon
            source_watcher->end = source_code_length;
            source_watcher->mode = POINT_WATCHER_PASSIVE_MODE;
#ifdef MU_CONTEXT_INFLATER_DEBUG
            log_info_str_hook_list(MU_CONTEXT_INFLATER_TAG, "CHUNKS_:", expression_chain);
            log_info_char(MU_CONTEXT_INFLATER_TAG, "::", source_code[source_watcher->start - 3]);
            log_info_char(MU_CONTEXT_INFLATER_TAG, "::", source_code[source_watcher->start - 2]);
            log_info_char(MU_CONTEXT_INFLATER_TAG, "::", source_code[source_watcher->start - 1]);
            log_info_char(MU_CONTEXT_INFLATER_TAG, "::", source_code[source_watcher->start]);
#endif
            return expression_chain;
        }
    }
    //Can not found end of function:
    throw_error_with_tag(MU_CONTEXT_INFLATER_TAG, "Can not found semicolon symbol in function expression!");
}

/**
 * Creates mu function definition.
 * @param source_watcher - rest string scope
 * @param source_code - string from file
 * @return definition.
 */

static mu_function_definition *conjure_mu_function_definition(
        point_watcher *source_watcher,
        const_string source_code
)
{
    //Find function materials step by step:
    str_hook *function_name = dig_function_name(source_watcher, source_code);
    str_hook_list *arg_definition_list = dig_arguments(source_watcher, source_code);
    str_hook_chain *expression = dig_expression(source_watcher, source_code);
    return new_custom_mu_function_definition(function_name, arg_definition_list, expression);
}

/**
 * Search custom mu functions in file and add their in manager.
 * @param file_path file with functions.
 */

void inflate_mu_context(const_string file_path)
{
    //Get source string from file:
    val source_code = read_file_and_join_to_string_without_comments(file_path);
    val source_code_length = strlen(source_code);
    val is_empty_context = source_code_length <= 0;
    if (!is_empty_context) {
        //Set source code in function manager:
        function_manager->source_code = source_code;
        //Get definition list:
        var definition_list = function_manager->definition_list;
        //Create context tools:
        var source_watcher = new_point_watcher();
        while (source_watcher->pointer < source_code_length) {
            val symbol = source_code[source_watcher->pointer];
            //Check symbol:
            val is_letter = isalpha(symbol);
            val is_whitespace = is_space_or_line_break(symbol);
            //Before "fun" modifier mode:
            if (is_letter && source_watcher->mode == POINT_WATCHER_PASSIVE_MODE) {
                source_watcher->mode = POINT_WATCHER_ACTIVE_MODE;
                source_watcher->start = source_watcher->pointer;
            }
            //Scan word:
            if (is_whitespace && source_watcher->mode == POINT_WATCHER_ACTIVE_MODE) {
                source_watcher->end = source_watcher->pointer;
                val modifier_hook = new_str_hook_by_point_watcher(source_code, source_watcher);
                val is_fun_modifier_found = is_hook_equals_str(modifier_hook, FUNCTION_MODIFIER);
                if (is_fun_modifier_found) {
#ifdef MU_CONTEXT_INFLATER_DEBUG
                    log_info_str_hook(MU_CONTEXT_INFLATER_TAG, "FUNCTION MODIFIER:", modifier_hook);
#endif
                    //Set attributes for point watcher about positions:
                    source_watcher->start = source_watcher->pointer;
                    source_watcher->end = source_code_length;
                    source_watcher->mode = POINT_WATCHER_PASSIVE_MODE;
                    //Find definition by source_watcher:
                    var function_definition = conjure_mu_function_definition(source_watcher, source_code);
                    add_mu_function_definition_in_list(definition_list, function_definition);
#ifdef MU_CONTEXT_INFLATER_DEBUG
                    var custom_attributes = function_definition->get.custom_attributes;
                    log_info(MU_CONTEXT_INFLATER_TAG, "CONJURED MU FUNCTION:");
                    log_info(MU_CONTEXT_INFLATER_TAG, "---------------------");
                    log_info_str_hook(MU_CONTEXT_INFLATER_TAG, "<NAME:>", function_definition->name);
                    log_info_str_hook_list(MU_CONTEXT_INFLATER_TAG, "<ARG:>", custom_attributes->arg_list);
                    log_info_str_hook_chain(MU_CONTEXT_INFLATER_TAG, "<EXPRESSION:>", custom_attributes->expression);
                    log_info(MU_CONTEXT_INFLATER_TAG, "---------------------");
#endif
                    continue;
                } else {
                    throw_error_with_hook(MU_CONTEXT_INFLATER_TAG, "First must be 'fun' modifier", modifier_hook);
                }
            }
            source_watcher->pointer++;
        }
#ifdef MU_CONTEXT_INFLATER_DEBUG
        log_info(MU_CONTEXT_INFLATER_TAG, "Complete!");
#endif
    } else {
        throw_error_with_tag(MU_CONTEXT_INFLATER_TAG, "Mu context is empty!");
    }
}
