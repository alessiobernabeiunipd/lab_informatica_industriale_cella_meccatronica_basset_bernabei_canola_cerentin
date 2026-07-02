project(test_buffer
        LANGUAGES C
        DESCRIPTION "test per la libreria di buffer"
)
add_executable(buffer_lib_test src/test_piu_buffer.c)

target_link_libraries( buffer_lib_test 
    PRIVATE  
    model 
    commons
) 