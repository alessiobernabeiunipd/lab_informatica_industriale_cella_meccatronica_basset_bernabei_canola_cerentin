project(test_parsing C)
add_executable(parsing_lib_test src/test.c)

target_link_libraries( parsing_lib_test
    PRIVATE
    parsing
    commons
    unity
)

add_test(parsing_lib_test parsing_lib_test)
