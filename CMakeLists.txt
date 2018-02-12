CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
project(cpp_zia)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(includes api lib)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(stdc++fs UNKNOWN IMPORTED)
set_property(TARGET stdc++fs PROPERTY IMPORTED_LOCATION "/usr/lib/gcc/x86_64-linux-gnu/7/libstdc++fs.a")

find_package (Threads)


add_executable(cpp_zia ${SOURCE_FILES})
target_link_libraries(cpp_zia stdc++fs)
target_link_libraries(cpp_zia ${CMAKE_DL_LIBS})
target_link_libraries (cpp_zia ${CMAKE_THREAD_LIBS_INIT})

# Build Modules
add_subdirectory(SrcModules/Network)

# Copie Modules
file(COPY Modules DESTINATION .)
