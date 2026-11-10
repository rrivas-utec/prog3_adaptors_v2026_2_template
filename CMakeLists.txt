cmake_minimum_required(VERSION 3.10.2)
project(prog3_adaptors_v2026_2)

set(CMAKE_CXX_STANDARD 23)
if(UNIX AND NOT APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

if(MINGW OR CYGWIN)
    add_compile_options($<$<CONFIG:Release>:-O3>)
endif()

include_directories(./include ./src)
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS
        src/*.cpp
        src/*.cc
        src/*.cxx)

set(student_sources_target "${PROJECT_NAME}_student_sources")
if(NOT source_files)
    set(header_only_placeholder "${CMAKE_CURRENT_BINARY_DIR}/header_only_placeholder.cpp")
    file(WRITE "${header_only_placeholder}" "// Placeholder for header-only submissions.\n")
    list(APPEND source_files "${header_only_placeholder}")
endif()
add_library(${student_sources_target} OBJECT ${source_files})
target_include_directories(${student_sources_target} PRIVATE ./include ./src)

add_executable(${PROJECT_NAME}
        main.cpp
        $<TARGET_OBJECTS:${student_sources_target}>)

if(UNIX AND NOT APPLE)
    find_package(TBB)
    target_link_libraries(${PROJECT_NAME} TBB::tbb)
endif()

add_subdirectory(autograder EXCLUDE_FROM_ALL)

