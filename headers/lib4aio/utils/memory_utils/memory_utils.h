#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

void *new_object(const unsigned size_of_object);

void *new_object_array(const unsigned number_of_elements, const unsigned size_of_object);

void *reallocate_object_array(
        void *object_array,
        const unsigned new_number_of_elements,
        const unsigned size_of_object
);

#endif //MEMORY_UTILS_H
