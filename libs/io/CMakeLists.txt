project(io_lib
        LANGUAGES C
        DESCRIPTION "libreria per logging su file e stdout e scrittura report metriche")

add_library(io STATIC src/logger.c src/report.c src/comparator.c)
target_include_directories(io PUBLIC include)
target_link_libraries(io PUBLIC commons)      
add_library(LII::io ALIAS io) 