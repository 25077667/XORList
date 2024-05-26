cmake_minimum_required(VERSION 3.20)

project(scc_allocator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_PATH
    ${CMAKE_SOURCE_DIR}
)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(${INCLUDE_PATH})
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /W4 /WX /EHsc /Zi")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall -Wextra -Werror -fexceptions -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
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
FetchContent_MakeAvailable(googletest)

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
