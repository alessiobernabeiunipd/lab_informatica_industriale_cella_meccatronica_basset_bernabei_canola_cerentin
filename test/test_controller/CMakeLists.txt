project(test_lib_controller
        DESCRIPTION "test per il controllore"
)
add_executable(controller_test src/test.c)

target_link_libraries( controller_test 
    PRIVATE 
    controller
    model 
    commons 
    unity
) 

add_test(controller_test test_lib_controller)