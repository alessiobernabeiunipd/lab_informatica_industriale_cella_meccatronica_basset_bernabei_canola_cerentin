project(controller_lib
       DESCRIPTION "libreria del controllore logico della cella")

add_library(controller STATIC 
    src/controller.c
)

target_include_directories(controller PUBLIC include)

target_link_libraries(controller PUBLIC model)

target_link_libraries(controller PUBLIC io)

target_link_libraries(controller PUBLIC stats)