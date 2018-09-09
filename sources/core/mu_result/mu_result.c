#include <stdlib.h>
#include <core/mu_result/mu_result.h>
#include <lib4aio/utils/str_hook/str_hook.h>
#include <lib4aio/utils/memory_utils/memory_utils.h>

/**
 * Creates new result.
 * @param acc - double value;
 * @param rest - rest function expression.
 * @return result.
 */

mu_result *new_mu_result(const double acc, str_hook_chain *rest)
{
    mu_result *result = new_object(sizeof(mu_result));
    result->acc = acc;
    result->rest = rest;
    return result;
}

/**
 * Deletes result.
 * @param result.
 */

void free_mu_result(mu_result *result)
{
    free_str_hook_chain(result->rest);
    free(result);
}
