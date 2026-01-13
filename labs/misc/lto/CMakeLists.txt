cmake_minimum_required(VERSION 3.3)

project(lab)
cmake_policy(SET CMP0069 NEW)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(VALIDATE_ARGS "${CMAKE_CURRENT_SOURCE_DIR}/golden.ppm")

string(REGEX MATCH "^(.*)[\\/]labs[\\/].*$" repo "${CMAKE_CURRENT_SOURCE_DIR}")
include(${CMAKE_MATCH_1}/tools/labs.cmake)
