#ifndef MU_FUNCTION_MANAGER_H
#define MU_FUNCTION_MANAGER_H

/**
 * Linking.
 */

#include <lib4aio/utils/str_hook/str_hook.h>

/**
 * Fun modifier.
 */

#define FUNCTION_MODIFIER "fun"

/**
 * Native function modifiers.
 */

#define COS_FUNCTION_NAME "cos"

#define LOG_FUNCTION_NAME "log"

#define SIN_FUNCTION_NAME "sin"

#define SQRT_FUNCTION_NAME "sqrt"

/**
 * @struct mu_function_manager maintains all function definitions in runtime.
 * It is literally the core of programm.
 */

typedef struct mu_function_manager
{
    struct mu_function_definition_list *definition_list;
    char *source_code;
} mu_function_manager;

/**
 * Function manager is a global field in the program.
 */

extern struct mu_function_manager *function_manager;

/**
 * Create function manager.
 */

void start_mu_function_manager();

/**
 * Destroy function manager.
 */

void finish_mu_function_manager();

/**
 * Checks validity or variable name.
 * @param name
 * @return true/false.
 */

_Bool can_use_name(struct str_hook *hook);

#endif //MU_FUNCTION_MANAGER_H
