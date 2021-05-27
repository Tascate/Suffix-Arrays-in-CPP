cmake_minimum_required(VERSION 3.0.0)
project(SuffixArray)

set(SOURCE_FILES main.cpp SuffixArray.cpp Skew.cpp SAIS.cpp Naive.cpp)
add_executable(SuffixArray ${SOURCE_FILES})