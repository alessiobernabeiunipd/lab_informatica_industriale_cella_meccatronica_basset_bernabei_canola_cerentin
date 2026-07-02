project(test_model
        LANGUAGES C
        DESCRIPTION "test per la libreria model"
)
add_executable(model_lib_test src/test.c)

target_link_libraries( model_lib_test 
    PRIVATE  
    model 
    commons
    Unity
) 
