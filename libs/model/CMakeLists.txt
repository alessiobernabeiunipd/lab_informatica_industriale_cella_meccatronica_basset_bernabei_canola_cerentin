project(model_lib
       DESCRIPTION "libreria di funzioni per agire su tipi e strutture")

add_library(model STATIC src/utils.c)

target_include_directories(model 
        PUBLIC include  
)
