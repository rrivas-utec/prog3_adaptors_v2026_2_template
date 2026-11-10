cmake_minimum_required(VERSION 3.10.2)
project(prog3_adaptors_v2026_2_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_DEBUG})
if(UNIX AND NOT APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

# Work Folders
include_directories(.)
include_directories(tests)
include_directories(../include)
include_directories(../src)
include_directories(../tools/catch)

# Detect source files that are still only generated skeletons. Their tests are
# skipped until the student writes code, keeping the local feedback focused.
function(question_has_implementation question header source)
    set(header_path "${CMAKE_CURRENT_SOURCE_DIR}/../include/${header}")
    set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/../src/${source}")
    set(is_incomplete FALSE)
    set(missing_symbols "")
    if(NOT EXISTS "${header_path}" OR NOT EXISTS "${source_path}")
        set(is_incomplete TRUE)
    else()
        file(READ "${header_path}" header_contents)
        file(READ "${source_path}" source_contents)
        if(source_contents MATCHES "AUTOGRADER: INCOMPLETE")
            set(is_incomplete TRUE)
        else()
            string(REGEX REPLACE "/\\*([^*]|\\*+[^*/])*\\*+/" "" source_contents "${source_contents}")
            string(REGEX REPLACE "//[^\r\n]*" "" source_contents "${source_contents}")
            string(REGEX REPLACE "#[^\r\n]*" "" source_contents "${source_contents}")
            string(STRIP "${source_contents}" source_contents)
            if("${source_contents}" STREQUAL "")
                set(is_incomplete TRUE)
            endif()
        endif()
        string(REGEX REPLACE "/\\*([^*]|\\*+[^*/])*\\*+/" "" header_contents "${header_contents}")
        string(REGEX REPLACE "//[^\r\n]*" "" header_contents "${header_contents}")
        foreach(required_symbol IN LISTS ARGN)
            string(FIND "${header_contents}" "${required_symbol}" symbol_position)
            if(symbol_position EQUAL -1)
                list(APPEND missing_symbols "${required_symbol}")
            endif()
        endforeach()
        if(missing_symbols)
            set(is_incomplete TRUE)
        endif()
    endif()
    if(is_incomplete)
        set_property(GLOBAL APPEND PROPERTY UNIMPLEMENTED_QUESTIONS "${question}")
        if(missing_symbols)
            set_property(GLOBAL APPEND PROPERTY UNIMPLEMENTED_DETAILS "#${question} (faltan: ${missing_symbols})")
        else()
            set_property(GLOBAL APPEND PROPERTY UNIMPLEMENTED_DETAILS "#${question} (archivo fuente sin implementar)")
        endif()
    endif()
endfunction()

question_has_implementation(3 "process_text_by_stack.h" "process_text_by_stack.cpp" "process_text_by_stack")
question_has_implementation(4 "process_text_by_queue.h" "process_text_by_queue.cpp" "process_text_by_queue")
question_has_implementation(5 "is_equation_balanced.h" "is_equation_balanced.cpp" "is_equation_balanced")
question_has_implementation(6 "browser_history.h" "browser_history.cpp" "BrowserMemento" "Browser" "BrowserHistory" "visit" "set_scroll" "current" "scroll" "save" "restore" "back" "forward" "utec::stack")
question_has_implementation(7 "validate_tags.h" "validate_tags.cpp" "string" "validate_tags")
question_has_implementation(8 "evaluate_postfix.h" "evaluate_postfix.cpp" "evaluate_postfix")

get_property(unimplemented_questions GLOBAL PROPERTY UNIMPLEMENTED_QUESTIONS)
get_property(unimplemented_details GLOBAL PROPERTY UNIMPLEMENTED_DETAILS)
file(GLOB_RECURSE question_test_files CONFIGURE_DEPENDS tests/question_*/test_*/*.cpp)
set(active_question_test_files "")
foreach(test_file IN LISTS question_test_files)
    if(test_file MATCHES "[/\\\\]question_([0-9]+)[/\\\\]")
        list(FIND unimplemented_questions "${CMAKE_MATCH_1}" question_is_pending)
        if(question_is_pending EQUAL -1)
            list(APPEND active_question_test_files "${test_file}")
        endif()
    endif()
endforeach()

file(GLOB shared_test_files CONFIGURE_DEPENDS tests/*.cpp)
set(project_files ${active_question_test_files} ${shared_test_files})

add_executable(${PROJECT_NAME}
        ${project_files}
        $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}_student_sources>
    )

# Do not precompile catch.hpp here. test-main.cpp defines CATCH_CONFIG_MAIN;
# a PCH would include the guarded header before that definition and omit the
# Catch2 implementation, causing linker errors on every REQUIRE/TEST_CASE.

if(UNIX AND NOT APPLE)
    find_package(TBB)
    target_link_libraries(${PROJECT_NAME} TBB::tbb)
endif()

# One runnable target per question for instructor-side local testing. The
# aggregate target above remains available for running every active question.
function(add_autograder_question_target question source)
    file(GLOB_RECURSE question_test_files CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/question_${question}/test_*/*.cpp")
    if(NOT question_test_files)
        message(FATAL_ERROR "No se encontraron pruebas para la pregunta ${question}")
    endif()

    set(question_target "autograder_question_${question}")
    set(question_sources ${question_test_files} ${shared_test_files})
    set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/../src/${source}")
    if(source AND EXISTS "${source_path}")
        list(APPEND question_sources "${source_path}")
    endif()
    add_executable(${question_target}
        ${question_sources}
    )
    if(UNIX AND NOT APPLE)
        target_link_libraries(${question_target} TBB::tbb)
    endif()
endfunction()

add_autograder_question_target(1 "")
add_autograder_question_target(2 "")
add_autograder_question_target(3 "process_text_by_stack.cpp")
add_autograder_question_target(4 "process_text_by_queue.cpp")
add_autograder_question_target(5 "is_equation_balanced.cpp")
add_autograder_question_target(6 "browser_history.cpp")
add_autograder_question_target(7 "validate_tags.cpp")
add_autograder_question_target(8 "evaluate_postfix.cpp")

if(unimplemented_questions)
    add_custom_target(${PROJECT_NAME}_pending_implementations
        COMMAND ${CMAKE_COMMAND} -E echo "AUTOGRADER: se omitieron preguntas sin implementacion o interfaz requerida: ${unimplemented_details}."
        COMMAND ${CMAKE_COMMAND} -E echo "Implemente la clase, funcion o template requerido en include/p<N>.h y el codigo en src/p<N>.cpp; luego vuelva a cargar CMake."
    )
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_pending_implementations)
endif()

# Test files
file(GLOB_RECURSE test_files CONFIGURE_DEPENDS
        tests/question_*/test_*/*.in
        tests/question_*/test_*/*.txt
    )
foreach(full_test_file_name ${test_files})
    configure_file(${full_test_file_name} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
endforeach(full_test_file_name)
