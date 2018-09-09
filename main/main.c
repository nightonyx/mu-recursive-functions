#include <stdio.h>
#include <process.h>
#include <malloc.h>
#include <mem.h>
#include <core/mu_function_managment/mu_function_manager.h>
#include <core/mu_function/mu_function.h>
#include <core/mu_function_managment/mu_context_inflater.h>
#include <lib4aio/utils/string_utils/string_utils.h>
#include <lib4aio/collections/lists/double_list.h>
#include <lib4aio/utils/type_utils/type.utils.h>
#include <lib4aio/utils/log_utils/log_utils.h>
#include <lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h>
#include <lib4aio/utils/double_utils/double_utils.h>

#define MU_DEBUG

#define MU_TAG "MU"

/**
 * Performs mu function.
 * @param file_path - file with mu functions.
 * @param function_name - name
 * @param double_values - double function args.
 * @return result.
 */
static double calc(
        const_string file_path,
        string function_name,
        double_list *double_values
)
{
    //1. Open manager:
    start_mu_function_manager();
    //2. Building custom functions definitions:
    inflate_mu_context(file_path);
    //3. Prepare for invoke function:
    var function_name_hook = new_str_hook_by_string(function_name);
    //4. Invoke result:
    val result = invoke_mu_function(function_name_hook, double_values);
    //5. Close manager:
    finish_mu_function_manager();
    return result;
}

/**
 * Performs mu function.
 * @param argc - number of arguments
 * @param argv - strings
 * @return - program state.
 */

int main(int argc, char *argv[])
{
    //1. *.mu file path:
    val file_path = argv[1];
#ifdef MU_DEBUG
    log_info_string(MU_TAG, "File path:", file_path);
#endif
    //2. Function_name:
    val function_name = argv[2];
#ifdef MU_DEBUG
    log_info_string(MU_TAG, "Function name:", function_name);
#endif
    //3. Function arguments:
    var function_arguments = new_double_list();
    val has_args = argc > 3;
    if (has_args) {
        for (int i = 3; i < argc; ++i) {
            val function_argument = string_to_double(argv[i]);
#ifdef MU_DEBUG
            log_info_double(MU_TAG, "External argument:", function_argument);
#endif
            add_in_double_list(function_arguments, function_argument);
        }
    }
    //4. Invoke function:
    val result = calc(file_path, function_name, function_arguments);
    //------------------------------------------------------------------------------------------------------------------
    //5. Print result:
    log_info_double(MU_TAG, "<RESULT>:", result);
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free_double_list(function_arguments);
    //------------------------------------------------------------------------------------------------------------------
    return 0;
}
