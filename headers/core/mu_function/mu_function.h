#ifndef MU_FUNCTION_H
#define MU_FUNCTION_H

/**
 * Performs function.
 * @param function_name - name.
 * @param input_value_list argument values.
 * @return double value.
 */

double invoke_mu_function(struct str_hook *function, struct double_list *input_value_list);

#endif //MU_FUNCTION_H
