project(test_lib_stats
        DESCRIPTION "test per la libreria stats"
)
add_executable(stats_lib_test src/test.c)
target_link_libraries( stats_lib_test 
    PRIVATE  
    stats
    unity
)
add_test(stats_test stats_lib_test)