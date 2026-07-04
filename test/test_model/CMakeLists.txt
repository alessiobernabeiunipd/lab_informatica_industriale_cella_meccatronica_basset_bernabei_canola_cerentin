project(test_model C)
add_executable(model_lib_test src/test.c)

target_link_libraries( model_lib_test 
    PRIVATE  
    model 
    commons
    io
    Unity
) 
add_test(model_lib_test model_lib_test)