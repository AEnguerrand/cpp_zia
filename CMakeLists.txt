CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

################################
# Build Zia Core
################################
project(cpp_zia)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(includes api lib)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(stdc++fs UNKNOWN IMPORTED)
set_property(TARGET stdc++fs PROPERTY IMPORTED_LOCATION "/usr/lib/gcc/x86_64-linux-gnu/7/libstdc++fs.a")

add_executable(cpp_zia ${SOURCE_FILES})
target_link_libraries(cpp_zia stdc++fs)
target_link_libraries(cpp_zia ${CMAKE_DL_LIBS})

################################
# Build Modules
################################
add_subdirectory(SrcModules/Network)

################################
# Copy Modules
################################
file(COPY Modules DESTINATION .)

################################
# Testing
################################
if (test)
    if (APPLE)
        add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
        add_definitions(-D__GLIBCXX__)
    endif (APPLE)

    include(ExternalProject)
    ExternalProject_Add(googletest
            GIT_REPOSITORY https://github.com/google/googletest
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION})

    include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
    link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
    ##############
    # Unit Tests
    ##############
    file(GLOB_RECURSE TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

    set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

    add_executable(runUnitTests ${TEST_SOURCE_FILES} ${SOURCE_FILES})

    target_link_libraries(runUnitTests stdc++fs)
    target_link_libraries(runUnitTests ${CMAKE_DL_LIBS})

    add_dependencies(runUnitTests googletest)
    target_link_libraries(runUnitTests gtest gtest_main pthread)

    enable_testing()
    add_test(NAME runUnitTests COMMAND runUnitTests)
endif()
