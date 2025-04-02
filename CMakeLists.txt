cmake_minimum_required(VERSION 3.28)
project(etbit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND SRC ${CMAKE_SOURCE_DIR}/src/main.cpp
                ${CMAKE_SOURCE_DIR}/src/lex/lexer.cc
                ${CMAKE_SOURCE_DIR}/src/parse/parser.cc
                )
list(APPEND HDR ${CMAKE_SOURCE_DIR}/include/)

# add_executable(etbit ${SRC})
# target_include_directories(etbit PUBLIC ${HDR})
#
# add_executable(lexer_test ${CMAKE_SOURCE_DIR}/tests/lex/lex_test.cc ${CMAKE_SOURCE_DIR}/src/lex/lexer.cc)
# target_include_directories(lexer_test PUBLIC ${HDR})

add_executable(parser_test ${CMAKE_SOURCE_DIR}/src/parse/parser.cc
                            ${CMAKE_SOURCE_DIR}/src/lex/lexer.cc
                            ${CMAKE_SOURCE_DIR}/tests/parse/parse_test.cc
                            )
target_include_directories(parser_test PUBLIC ${HDR})
