#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

typedef struct double_list
{
    unsigned capacity;
    unsigned size;
    double *values;
} double_list;

double_list *new_double_list();

void add_in_double_list(double_list *list, const double value);

void free_double_list(double_list *list);

#endif //DOUBLE_LIST_H
