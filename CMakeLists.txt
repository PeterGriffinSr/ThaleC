cmake_minimum_required(VERSION 3.31)

project(thale 
    VERSION 0.0.1.0
    LANGUAGES C
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/source/include/config.in.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    @ONLY
)

set(SOURCES
    source/thale.c
    source/error.c
    source/help.c
    source/lex.c
)

add_executable(thale ${SOURCES})

target_include_directories(thale PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_library(thale_lib
    source/thale.c
    source/error.c
    source/help.c
    source/lex.c
)

target_include_directories(thale_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(thale PRIVATE thale_lib)

enable_testing()

file(GLOB TEST_SOURCES "tests/*.c")
foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_include_directories(${test_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/include
        ${CMAKE_CURRENT_BINARY_DIR}
    )
    target_link_libraries(${test_name} PRIVATE thale_lib)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()