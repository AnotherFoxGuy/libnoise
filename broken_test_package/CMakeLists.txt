cmake_minimum_required(VERSION 2.8.12)
project(PackageTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(example ${CMAKE_SOURCE_DIR}/../examples/texturegranite.cpp)
target_link_libraries(example CONAN_PKG::libnoise)

# CTest is a testing tool that can be used to test your project.
enable_testing()
add_test(NAME example
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
         COMMAND example)
