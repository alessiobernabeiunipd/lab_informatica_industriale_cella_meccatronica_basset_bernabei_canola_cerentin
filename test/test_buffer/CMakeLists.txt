project(test_buffer C)
add_executable(buffer_lib_test src/test_buffer.c)

target_link_libraries( buffer_lib_test 
    PRIVATE  
    model 
    commons
    io
    Unity
) 
add_test(buffer_lib_test buffer_lib_test)