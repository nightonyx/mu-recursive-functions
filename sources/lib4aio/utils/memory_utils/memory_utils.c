#include <mem.h>
#include <malloc.h>
#include <stdlib.h>

void *new_object(const size_t size_of_object)
{
    void *object = calloc(1, size_of_object);
    if (!object) {
        perror("Can not allocate memory for object!");
        exit(1);
    }
    return object;
}

void *new_object_array(const size_t number_of_elements, const size_t size_of_object)
{
    void *object_array = calloc(number_of_elements, size_of_object);
    if (!object_array) {
        perror("Can not allocate memory for object array!");
        exit(1);
    }
    return object_array;
}

void *reallocate_object_array(
        void *object_array,
        const size_t new_number_of_elements,
        const size_t size_of_object
)
{
    object_array = realloc(object_array, new_number_of_elements * sizeof(size_of_object));
    if (!object_array) {
        perror("Can not reallocate memory for object array!");
        exit(1);
    }
    return realloc(object_array, new_number_of_elements * size_of_object);
}

void free_object(void *object)
{
    if (object) {
        free(object);
    }
}
