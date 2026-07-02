project(test_lib_commons
        LANGUAGES C
        DESCRIPTION "test per la libreria common structs"
)
add_executable(commons_lib_test src/test.c)

target_link_libraries( commons_lib_test 
    PRIVATE  
    commons 
    unity
) 

add_test(commons_test commons_lib_test)