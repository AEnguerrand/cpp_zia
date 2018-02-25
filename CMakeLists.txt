CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

################################
# Init of project
################################
project(cpp_zia)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

################################
# Build Modules
################################
add_subdirectory(SrcModules/Network)
add_subdirectory(SrcModules/Php)
add_subdirectory(SrcModules/Router)

################################
# Build Zia Core
################################

if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")
endif()

include_directories(includes api lib)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

if (UNIX)
    add_library(stdc++fs UNKNOWN IMPORTED)
    set_property(TARGET stdc++fs PROPERTY IMPORTED_LOCATION "/usr/lib/gcc/x86_64-linux-gnu/7/libstdc++fs.a")

    find_package (Threads)
endif()

add_executable(cpp_zia ${SOURCE_FILES})

if (UNIX)
    target_link_libraries(cpp_zia stdc++fs)
    target_link_libraries(cpp_zia ${CMAKE_DL_LIBS})
endif()

target_link_libraries (cpp_zia ${CMAKE_THREAD_LIBS_INIT})

################################
# Testing
################################
if (BUILD_TESTS)
    if (APPLE)
        add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
        add_definitions(-D__GLIBCXX__)
    endif (APPLE)

    file(COPY CMakeLists.txt.in DESTINATION .)

    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
            "${CMAKE_BINARY_DIR}/googletest-build")

    ##############
    # Unit Tests
    ##############
    file(GLOB_RECURSE TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
    file(GLOB_RECURSE T_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

    list(REMOVE_ITEM T_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

    set(CMAKE_CXX_FLAGS "-g --coverage -O0 -Wall -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

    if(MSVC)
    	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    endif(MSVC)

    add_executable(runUnitTests ${TEST_SOURCE_FILES} ${T_SOURCE_FILES})

    if (UNIX AND NOT APPLE)
    	target_link_libraries(runUnitTests stdc++fs)
    	target_link_libraries(runUnitTests ${CMAKE_DL_LIBS})
    endif()

if (UNIX)
    target_link_libraries(runUnitTests gtest gtest_main pthread)
elseif (MSVC)
    target_link_libraries(runUnitTests gtest gtest_main)
endif()

    enable_testing()
    add_test(NAME runUnitTests COMMAND runUnitTests)
endif()
