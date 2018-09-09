#ifndef MU_DOUBLE_RESULT_H
#define MU_DOUBLE_RESULT_H

/**
 * @struct mu_result is a wrapper for double value in local operation of double parser.
 * acc - is double value
 * rest - is a rest of function expression which parsed by parser.
 */

typedef struct mu_result {
    double acc;
    struct str_hook_list* rest;
} mu_result;

/**
 * Creates new result.
 * @param acc - double value;
 * @param rest - rest function expression.
 * @return result.
 */

struct mu_result* new_mu_result(const double acc, struct str_hook_list* rest);

/**
 * Delete result.
 * @param result.
 */

void free_mu_result(struct mu_result *result);

#endif //MU_DOUBLE_RESULT_H
