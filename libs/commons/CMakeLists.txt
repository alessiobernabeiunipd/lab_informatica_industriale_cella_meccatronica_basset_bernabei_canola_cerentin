project(commons_lib
        LANGUAGES C
        DESCRIPTION "libreria definizione tipi e funzioni di utilità")

add_library(commons STATIC src/utils.c)

target_include_directories(commons PUBLIC include)
