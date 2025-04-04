cmake_minimum_required(VERSION 3.28)
project(etbit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GEN_PARSE_TEST OFF)
set(GEN_LEX_TEST OFF)

list(APPEND SRC ${CMAKE_SOURCE_DIR}/src/main.cpp
                ${CMAKE_SOURCE_DIR}/src/lex/lexer.cc
                ${CMAKE_SOURCE_DIR}/src/parse/parser.cc
                )
list(APPEND HDR ${CMAKE_SOURCE_DIR}/include/)

add_executable(etbit ${SRC})
target_include_directories(etbit PUBLIC ${HDR})

target_compile_options(etbit PUBLIC -Wno-deprecated -Wall -Wextra -O2 -Wshadow -Wpedantic -Wconversion -funroll-loops -Wno-deprecated-declarations)

if (GEN_LEX_TEST)
    add_executable(lexer_fn_test ${CMAKE_SOURCE_DIR}/tests/lex/lex_test.cc ${CMAKE_SOURCE_DIR}/src/lex/lexer.cc)
    target_include_directories(lexer_fn_test PUBLIC ${HDR})

    add_executable(lexer_ut ${CMAKE_SOURCE_DIR}/tests/lex/lex_ut.cc ${CMAKE_SOURCE_DIR}/src/lex/lexer.cc)
    target_include_directories(lexer_ut PUBLIC ${HDR})
endif()

if(GEN_PARSE_TEST)
    add_executable(parser_test ${CMAKE_SOURCE_DIR}/src/parse/parser.cc
                            ${CMAKE_SOURCE_DIR}/src/lex/lexer.cc
                            ${CMAKE_SOURCE_DIR}/tests/parse/parse_test.cc
                            )
    target_include_directories(parser_test PUBLIC ${HDR})

    add_executable(parser_ut ${CMAKE_SOURCE_DIR}/src/parse/parser.cc
                        ${CMAKE_SOURCE_DIR}/src/lex/lexer.cc
                        ${CMAKE_SOURCE_DIR}/tests/parse/parser_ut.cc
                        )
    target_include_directories(parser_ut PUBLIC ${HDR})
endif()
unset(GEN_LEX_TEST CACHE)
unset(GEN_PARSE_TEST CACHE)

