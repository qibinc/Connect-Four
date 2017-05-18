cmake_minimum_required(VERSION 3.7)
project(Strategy)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        Board.cpp
        Board.h
        Judge.cpp
        Judge.h
        Point.h
        MonteCarloTreeSearch.h
        )

#add_library(Strategy SHARED ${SOURCE_FILES})
add_executable(Strategy ${SOURCE_FILES})