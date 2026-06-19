cmake_minimum_required( VERSION 3.10.2 )

project( lamination_buffer
	VERSION 0.2
    LANGUAGES C
	DESCRIPTION "Libreria di gestione buffer pre laminazione."
	)

add_library(buffer STATIC src/buffer.c)

target_include_directories(buffer
	PUBLIC include
	)

add_library(LII::buffer ALIAS buffer)

add_executable( buffer_app_test src/test_buffer.c )

target_link_libraries( buffer_app_test
	PUBLIC
	LII::buffer
)
