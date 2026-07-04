project (parser_lib
        LANGUAGES C
        DESCRIPTION "libreria di parsing")

add_library( parsing STATIC src/parsing.c src/generator.c)

target_include_directories( parsing PUBLIC include )

target_link_libraries( parsing PUBLIC commons model io)