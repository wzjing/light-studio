cmake_minimum_required(VERSION 3.19)
project(Lightning-driver)
set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk")

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(driver)
include_directories(driver/include)

add_subdirectory(util)
include_directories(util/include)

add_executable(Lightning-driver main.cpp util/include/log.hpp util/src/log.cpp)

target_link_libraries(Lightning-driver driver)