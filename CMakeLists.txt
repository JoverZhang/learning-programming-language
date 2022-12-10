cmake_minimum_required (VERSION 3.2)
project (LEARNING_PROGRAMMING_LANGURAGE)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

include_directories(./include)

add_subdirectory(./calc)
