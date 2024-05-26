cmake_minimum_required(VERSION 3.20)

project(xorlist)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Common Compiler Flags
set(COMMON_CXX_FLAGS "-Wall -Wextra -Werror -pedantic")
set(DEBUG_CXX_FLAGS "-g -fsanitize=address -fno-omit-frame-pointer -O0")
set(RELEASE_CXX_FLAGS "-O3")
set(LTO_FLAGS_GNU "-flto=auto")
set(LTO_FLAGS_CLANG "-flto=thin")
set(SHRINK_SYMBOL_TABLE_FLAGS "-fdata-sections -ffunction-sections")

# Compiler-Specific Flags
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_CXX_FLAGS}")
    add_compile_options(-fcolor-diagnostics)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_CXX_FLAGS}")
    add_compile_options(-fdiagnostics-color=always)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Debug build settings
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEBUG_CXX_FLAGS}")
    if (NOT (CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64"))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEBUG_CXX_FLAGS}")
    else()
        message(WARNING "Sanitizer is not supported on ARM")
    endif()
    add_compile_definitions(DEBUG)
endif()

# Release build settings
if (CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RELEASE_CXX_FLAGS} ${SHRINK_SYMBOL_TABLE_FLAGS}")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LTO_FLAGS_GNU}")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LTO_FLAGS_CLANG}")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /GL")
        if (NOT CMAKE_CXX_FLAGS MATCHES ".*(/GL|-GL).*")
            message(WARNING "Your MSVC version does not support link time optimization")
        endif()
    endif()
    add_compile_definitions(NDEBUG)
endif()

# Fetch and include GoogleTest and GoogleBenchmark
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG main
)
set(BENCHMARK_ENABLE_TESTING off)
FetchContent_MakeAvailable(googletest googlebenchmark)

# Enable testing
enable_testing()
include(GoogleTest)

# Add test sources
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/tests/*.cpp)
foreach(source ${TEST_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(t_${name} ${source})
    target_link_libraries(t_${name} GTest::gtest_main)
    gtest_discover_tests(t_${name})
endforeach()

# Add benchmark sources
file(GLOB_RECURSE BENCHMARK_SOURCES ${CMAKE_SOURCE_DIR}/benchmark/*.cpp)
foreach(source ${BENCHMARK_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(b_${name} ${source})
    target_link_libraries(b_${name} benchmark::benchmark)
endforeach()
