cmake_minimum_required(VERSION 3.10)
project(mu_recursive)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Wextra")

include_directories(headers)

add_executable(mu_recursive
        #---------------------------------------------------------------------------------------------------------------
        #MAIN:
        main/main.c
        #---------------------------------------------------------------------------------------------------------------

        #---------------------------------------------------------------------------------------------------------------
        #HEADERS:
        headers
        #core:
        headers/core/mu_function/mu_function.h
        headers/core/mu_function/mu_function_definition.h
        headers/core/mu_function_managment/mu_context_inflater.h
        headers/core/mu_function_managment/mu_function_manager.h
        headers/core/mu_parser/mu_parser.h
        headers/core/mu_result/mu_result.h
        headers/core/mu_variable/mu_variable.h
        #lib4aio:
        headers/lib4aio/collections/lists/double_list.h
        headers/lib4aio/collections/lists/string_list.h
        headers/lib4aio/utils/boolean_utils/boolean_utils.h
        headers/lib4aio/utils/char_utils/char_utils.h
        headers/lib4aio/utils/double_utils/double_utils.h
        headers/lib4aio/utils/error_utils/error_utils.h
        headers/lib4aio/utils/file_utils/file_reader.h
        headers/lib4aio/utils/log_utils/log_utils.h
        headers/lib4aio/utils/memory_utils/memory_utils.h
        headers/lib4aio/utils/point_watcher/point_watcher.h
        headers/lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.h
        headers/lib4aio/utils/str_hook/str_hook.h
        headers/lib4aio/utils/string_utils/string_builder.h
        headers/lib4aio/utils/string_utils/string_utils.h
        headers/lib4aio/utils/type_utils/type.utils.h
        #---------------------------------------------------------------------------------------------------------------

        #---------------------------------------------------------------------------------------------------------------
        #SOURCES:
        sources
        #core:
        sources/core/mu_function/mu_function.c
        sources/core/mu_function/mu_function_definition.c
        sources/core/mu_function_managment/mu_context_inflater.c
        sources/core/mu_function_managment/mu_function_manager.c
        sources/core/mu_native/cos.c
        sources/core/mu_native/log.c
        sources/core/mu_native/sin.c
        sources/core/mu_native/sqrt.c
        sources/core/mu_parser/mu_parser.c
        sources/core/mu_result/mu_result.c
        sources/core/mu_variable/mu_variable.c
        #lib4aio:
        sources/lib4aio/collections/lists/double_list.c
        sources/lib4aio/collections/lists/string_list.c
        sources/lib4aio/utils/char_utils/char_utils.c
        sources/lib4aio/utils/double_utils/string_to_double.c
        sources/lib4aio/utils/error_utils/error_utils.c
        sources/lib4aio/utils/file_utils/file_reader.c
        sources/lib4aio/utils/log_utils/log_utils.c
        sources/lib4aio/utils/memory_utils/memory_utils.c
        sources/lib4aio/utils/point_watcher/point_watcher.c
        sources/lib4aio/utils/str_hook/str_hook_utils/str_hook_split.c
        sources/lib4aio/utils/str_hook/str_hook_utils/str_hook_trim.c
        sources/lib4aio/utils/str_hook/str_hook_utils/str_hook_utils.c
        sources/lib4aio/utils/str_hook/str_hook.c
        sources/lib4aio/utils/string_utils/prefix_suffix.c
        sources/lib4aio/utils/string_utils/split.c
        sources/lib4aio/utils/string_utils/string_builder.c
        sources/lib4aio/utils/string_utils/string_utils.c
        sources/lib4aio/utils/string_utils/trim.c
        #---------------------------------------------------------------------------------------------------------------
        )