project("util")

set(CMAKE_CXX_FLAGS "-fno-exceptions")
set(CMAKE_SHARED_LINKER_FLAGS "-framework IOKit -framework CoreFoundation")

set(source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp)

add_library(util SHARED ${source_files})
target_include_directories(util PUBLIC include)