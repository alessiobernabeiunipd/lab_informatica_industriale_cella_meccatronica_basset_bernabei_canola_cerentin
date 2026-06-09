project(lab_informatica_industriale_cella_meccatronica_basset_bernabei_canola_cerentin)

if (MSVC)   #microsoft visual C++, compiler proprietario per c, c#, c++
	# warning level 4 and all warnings as errors
	add_compile_options(/W4 /WX)
else()
	# lots of warnings and all warnings as errors
	add_compile_options(-Wall -Wextra -pedantic)
endif()

set(TARGET_GROUP test CACHE STRING "Group to build") # posso alternare tra run di test o di simulazione 
#La modalità scelta va selezionata da terminale con "cmake .. -DTARGET_GROUP=production" 
#prima di chiamare "make"

add_subdirectory(libs/commons)
#add_subdirectory(libs/parsing)

if(TARGET_GROUP STREQUAL simulation)
    add_subdirectory(app)
elseif(TARGET_GROUP STREQUAL test)
    include(CTest)
    add_subdirectory(external/Unity)
    add_subdirectory(test/test_common_structs)
else()
    message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()