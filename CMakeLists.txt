cmake_minimum_required(VERSION 3.10)
project(lab_meccatronica C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -g)
endif()

# Aggiunge le librerie e l'applicazione principale
add_subdirectory(libs/commons)
add_subdirectory(app)
