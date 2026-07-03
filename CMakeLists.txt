cmake_minimum_required(VERSION 3.10)
project(lab_informatica_industriale_cella_meccatronica_basset_bernabei_canola_cerentin C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -g)
endif()

set(TARGET_GROUP test CACHE STRING "Group to build") # posso alternare tra run di test o di simulazione 
# La modalità scelta va selezionata da terminale con "cmake .. -DTARGET_GROUP=opzione_desiderata" dentro la cartella di build
# prima di chiamare "make"

add_subdirectory(libs/commons)
add_subdirectory(libs/model)
add_subdirectory(libs/parsing)
add_subdirectory(libs/io)
add_subdirectory(libs/stats)
add_subdirectory(libs/controller)

if(TARGET_GROUP STREQUAL simulation)
    add_subdirectory(app)
elseif(TARGET_GROUP STREQUAL test)
    include(CTest)
    add_subdirectory(external/Unity)
    add_subdirectory(test/test_buffer)
    add_subdirectory(test/test_common_structs)
    add_subdirectory(test/test_model)
    add_subdirectory(test/test_io)
    add_subdirectory(test/test_stats)
else()
    message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()
