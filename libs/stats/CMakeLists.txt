project(stats_lib
        LANGUAGES C
        DESCRIPTION "libreria per calcolo metriche/stats report")

add_library(stats STATIC src/metrics.c)
target_include_directories(stats PUBLIC include)
target_link_libraries(stats PUBLIC commons)
add_library(LII::stats ALIAS stats) 
