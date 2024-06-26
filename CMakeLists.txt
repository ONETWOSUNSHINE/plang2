cmake_minimum_required(VERSION 2.8)
project(PLang)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive")

find_library(GMPXXLIB gmpxx)

if (NOT GMPXXLIB)
    message(FATAL_ERROR "GMPXXLIB not found.")
endif()

include(FindPkgConfig)

set(CVC3 "auto" CACHE STRING "Use CVC3 solver")

if (NOT CVC3 STREQUAL "0")
    pkg_check_modules(CVC3LIB cvc3)
    if (NOT CVC3LIB_FOUND AND CVC3 STREQUAL "1")
        message(FATAL_ERROR "CVC3LIB not found.")
    endif()
    if (CVC3LIB_FOUND)
        include_directories(${CVC3LIB_INCLUDE_DIRS})
    endif()
endif()

add_subdirectory(src)
add_subdirectory(tests)
