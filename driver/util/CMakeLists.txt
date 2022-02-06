project("include")

set(CMAKE_CXX_FLAGS "-fno-exceptions")
set(CMAKE_SHARED_LINKER_FLAGS "-framework IOKit -framework CoreFoundation")

file(GLOB source_files
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.c)

add_library(util SHARED ${source_files})
target_include_directories(util PUBLIC include)