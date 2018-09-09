#include <mem.h>
#include <stdio.h>
#include <malloc.h>
#include <lib4aio/utils/string_utils/string_utils.h>
#include <lib4aio/collections/lists/string_list.h>
#include <lib4aio/utils/error_utils/error_utils.h>

#define MU_COMMENTS "//"

#define READ "r"

#define CHUNK 1024

#define FILE_READER_DEBUG

#define FILE_READER_TAG "FILE_READER"

#ifdef FILE_READER_DEBUG

#include <lib4aio/utils/log_utils/log_utils.h>

#endif

const_string get_code_without_comments(const_string file_content)
{
    string_list *clean_line_list = new_string_list();
    const_string_array lines = split_by_string(file_content, "\n");
    for (int i = 0; i < get_string_array_size(lines); ++i) {
        const_string line = lines[i];
        if (!starts_with_prefix(line, MU_COMMENTS) && !is_empty_string(line)) {
            const_string_array dirty_line_with_comments = split_by_string(line, MU_COMMENTS);
            string dirty_line = new_string(dirty_line_with_comments[0]);
            string clean_line = trim(dirty_line);
            if (is_not_empty_string(clean_line)) {
                add_string_in_list(clean_line_list, clean_line);
            }
            //----------------------------------------------------------------------------------------------------------
            //찌꺼기 수집기 (Garbage collector):
            free(dirty_line);
            free(dirty_line_with_comments);
        }
    }
    string clean_code = join_to_string(clean_line_list->strings, "\n", clean_line_list->size);
    //------------------------------------------------------------------------------------------------------------------
    //찌꺼기 수집기 (Garbage collector):
    for (unsigned j = 0; j < clean_line_list->size; ++j) {
        free(clean_line_list->strings[j]);
    }
    free_string_list(clean_line_list);
    free(lines);
#ifdef FILE_READER_DEBUG
    log_info_string(FILE_READER_TAG, "Source code:\n", clean_code);
#endif
    return clean_code;
}

string read_file_and_join_to_string_without_comments(const_string path)
{
    unsigned read_position = 0;
    size_t code_size = CHUNK;
    string file_content = calloc(code_size, sizeof(char));
    char read_pointer;
    //Create file:
    FILE *file_reference;
    if ((file_reference = fopen(path, READ)) == NULL) {
        throw_error("Can not open source file");
    }
    while ((read_pointer = (char) fgetc(file_reference)) != EOF) {
        if (read_position == code_size) {
            code_size *= 2;
            file_content = realloc(file_content, code_size);
        }
        file_content[read_position] = read_pointer;
        read_position++;
    }
    fclose(file_reference);
    const_string clean_code = get_code_without_comments(file_content);
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free(file_content);
    return clean_code;
}
