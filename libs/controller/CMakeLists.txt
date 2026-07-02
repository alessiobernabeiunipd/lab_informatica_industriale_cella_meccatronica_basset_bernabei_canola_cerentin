project(controller_lib C)

add_library(controller STATIC 
    src/controller.c
)

target_include_directories(controller PUBLIC include)

target_link_libraries(controller PUBLIC model)