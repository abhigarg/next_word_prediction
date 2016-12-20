cmake_minimum_required(VERSION 3.6)
project(testTrie)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp trie.h)
add_executable(testTrie ${SOURCE_FILES})