cmake_minimum_required(VERSION 3.15)

project(ThaleCompiler
    VERSION 0.0.1.0
    LANGUAGES C
)

if (NOT MSVC)
    message(FATAL_ERROR "This Windows build requires Microsoft Visual C (MSVC).")
endif()

option(ENABLE_DEBUG "Enable debug build (default: OFF)" OFF)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/source/include/config.in.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    @ONLY
)

set(COMMON_WARNINGS
    /W4
    /WX
    /permissive-
    /Zc:preprocessor
    /Zc:inline
    /Zc:forScope
    /nologo
)

if (ENABLE_DEBUG)
    message(STATUS "Building in DEBUG mode")
    add_compile_options(
        ${COMMON_WARNINGS}
        /Od
        /Zi
        /RTC1
        /DDEBUG
        /MDd
    )
    add_link_options(/DEBUG)
else()
    message(STATUS "Building in RELEASE mode")
    add_compile_options(
        ${COMMON_WARNINGS}
        /O2
        /DNDEBUG
        /MD
    )
endif()

set(SOURCES
    source/thale.c
    source/error.c
    source/help.c
    source/lex.c
)

add_library(thale_lib STATIC ${SOURCES})

target_include_directories(thale_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(thale ${SOURCES})

target_include_directories(thale PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(thale PRIVATE thale_lib)

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

message(STATUS "")
message(STATUS "───────────────────────────────")
message(STATUS " Project: ThaleCompiler")
message(STATUS " Version: ${PROJECT_VERSION}")
message(STATUS " Compiler: ${CMAKE_C_COMPILER}")
message(STATUS " Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS " Debug: ${ENABLE_DEBUG}")
message(STATUS "───────────────────────────────")
message(STATUS "")