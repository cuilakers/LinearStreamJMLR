cmake_minimum_required(VERSION 3.17)
project(Coverage)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(Coverage main.cpp utilityfunction.h SmkStream.h
        OneStream.h
        MultiStream.h
        DynamicMRT.h)

