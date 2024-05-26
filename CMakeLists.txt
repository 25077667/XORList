cmake_minimum_required(VERSION 3.20)

project(scc_allocator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
# Windows MSVC is a bit different, there might some error in the header files,
# so we disable the Wx flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

include_directories(${CMAKE_SOURCE_DIR})
# Enable debug symbols, sanitizer, and disable optimization for debug build
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Disable sanitizer if compiler is ARM or target is ARM
        if (NOT (CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64"))
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer -O0")
        else ()
            message(WARNING "Sanitizer is not supported on ARM")
        endif ()
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        # Enable sanitizer if MSVC version supports it
        if (CMAKE_CXX_FLAGS MATCHES ".*(/sdl|-sdl).*")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address")
        else ()
            message(WARNING "Your MSVC version does not support sanitizer")
        endif ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /Od")
    endif ()
    # Define DEBUG macro
    add_compile_definitions(DEBUG)
endif ()

# If it is release build, we will enable optimization, also link time optimization if it is supported
if (CMAKE_BUILD_TYPE MATCHES "Release")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
        if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=auto")
        elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=thin")
        endif()
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
        if (CMAKE_CXX_FLAGS MATCHES ".*(/GL|-GL).*") # If it is support link time optimization, we will enable it
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
        else ()
            message(WARNING "Your MSVC version does not support link time optimization")
        endif()
    endif()
    # Define NDEBUG macro
    add_compile_definitions(NDEBUG)
endif()

# Shrink symbol table if it is release build
if (CMAKE_BUILD_TYPE MATCHES "Release")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdata-sections -ffunction-sections")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gy")
    endif()
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
endif ()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(BENCHMARK_ENABLE_TESTING off)
FetchContent_Declare(googlebenchmark
                     GIT_REPOSITORY https://github.com/google/benchmark.git
                     GIT_TAG main
)

FetchContent_MakeAvailable(
        googletest
        googlebenchmark)

enable_testing()
include(GoogleTest)

# use shell to generate source list, enum all cpp files in test
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

foreach(source ${SOURCES})
    message(STATUS "add test: ${source}")
# set the name of executable to the name of source file with leading "t_"
    get_filename_component(name ${source} NAME_WE)
    add_executable(t_${name} ${source})
    target_link_libraries(t_${name} GTest::gtest_main)
    gtest_discover_tests(t_${name})
endforeach()

file(GLOB_RECURSE BENCHMARK_SOURCES
    ${CMAKE_SOURCE_DIR}/benchmark/*.cpp
)

foreach(source ${BENCHMARK_SOURCES})
    message(STATUS "add benchmark: ${source}")
    get_filename_component(name ${source} NAME_WE)
    add_executable(b_${name} ${source})
    target_link_libraries(b_${name} benchmark::benchmark)
endforeach()
