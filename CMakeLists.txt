cmake_minimum_required(VERSION 3.1.5)
project(rainbomizer)

# C++20
if (MSVC_VERSION GREATER_EQUAL "1900")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  # Because CMAKE_MSVC_RUNTIME_LIBRARY doesn't work for whatever reason
  set(CompilerFlags
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
    )
  foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endforeach()
endif()

set (CMAKE_CXX_STANDARD 17)

# Sources
include_directories(include lib)
file(GLOB SOURCES src/*.cc src/*.cpp lib/*.cc lib/Patterns/*.cpp)

add_library("rainbomizer" SHARED ${SOURCES})

# Properties
set_target_properties("rainbomizer" PROPERTIES SUFFIX ".asi")
target_link_libraries("rainbomizer" PUBLIC dbghelp)
target_compile_definitions("rainbomizer" PUBLIC NOMINMAX)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

else()
  target_compile_features("rainbomizer" PRIVATE cxx_auto_type)
endif()

#56 8b 74 24 0c 8b 04 b5 ? ? ? ? 8b c8 c1 e9 18
