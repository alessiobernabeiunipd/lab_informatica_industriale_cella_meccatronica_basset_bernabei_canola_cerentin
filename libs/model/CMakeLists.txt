project(model_lib C)

add_library(model STATIC 
    src/pezzo.c
    src/cella.c
    src/buffer.c
)

target_include_directories(model PUBLIC include)

target_link_libraries( model PUBLIC commons)