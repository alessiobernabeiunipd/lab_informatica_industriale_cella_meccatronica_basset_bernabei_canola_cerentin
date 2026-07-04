project(test_lib_commons C)
add_executable(commons_lib_test src/test.c)

target_link_libraries( commons_lib_test 
    PRIVATE  
    commons
    io
    Unity
) 

add_test(commons_test commons_lib_test)