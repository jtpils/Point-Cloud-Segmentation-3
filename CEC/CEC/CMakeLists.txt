﻿cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(conditional_euclidean_clustering)

find_package(PCL 1.7 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (conditional_euclidean_clustering CEC.cpp)
target_link_libraries (conditional_euclidean_clustering ${PCL_LIBRARIES})