cmake_minimum_required(VERSION 3.16)
project(PLang)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fpermissive -Wno-deprecated -Wno-cast-qual -g -O0") # -g -O0 - для отладки 

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

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(LLVM_LIBS ${LLVM_LIBRARIES})
set(LLVM_INCLUDE_DIRS ${LLVM_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(tests)
