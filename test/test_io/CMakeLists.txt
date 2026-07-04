project(test_lib_io
        DESCRIPTION "test per la libreria io"
)
add_executable(io_lib_test src/test.c)
target_link_libraries( io_lib_test
PRIVATE  
    io 
    Unity
)
add_test(io_test io_lib_test)