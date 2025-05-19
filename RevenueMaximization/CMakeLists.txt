cmake_minimum_required(VERSION 3.13)
project(Rev)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(Rev main.cpp generate_random_data.h utilityfunction.h OneStream.h MultiStream.h SmkStream.h
        Offline.h
        KnapStream.h)