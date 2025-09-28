cmake_minimum_required(VERSION 3.13)
project(Rev)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(Rev main.cpp utilityfunction.h OneStream.h MultiStream.h SmkStream.h KnapStream.h Offline.h)