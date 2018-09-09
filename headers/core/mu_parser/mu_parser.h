#ifndef MU_PARSER_H
#define MU_PARSER_H

/**
 * Parses expression.
 * @param expression_chain - function expression.
 * @param variable_list - table of variables.
 * @return double value.
 */

double parse_expression(struct str_hook_list *expression, struct mu_variable_list *variable_list);

#endif //MU_PARSER_H
