CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
project(cpp_zia)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(includes api lib)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(cpp_zia ${SOURCE_FILES})