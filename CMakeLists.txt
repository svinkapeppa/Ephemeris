cmake_minimum_required(VERSION 3.9)
project(Ephemeris)

set(CMAKE_CXX_STANDARD 17)

add_executable(Ephemeris main.cpp EphemerisLibrary.h EphemerisLibrary.cpp)