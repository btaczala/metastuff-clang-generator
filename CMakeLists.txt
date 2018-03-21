cmake_minimum_required(VERSION 3.0)
project(metastuff-generator CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_subdirectory(3rdParty/fmt)
add_subdirectory(src)
