cmake_minimum_required(VERSION 3.1.5)
project(plugin_iv)

# C++20
if (MSVC_VERSION GREATER_EQUAL "1900")
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
  if (_cpp_latest_flag_supported)
    add_compile_options("/std:c++latest")
  endif()
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

add_library("plugin_iv" SHARED ${SOURCES})

# Properties
set_target_properties("plugin_iv" PROPERTIES SUFFIX ".asi")
target_link_libraries("plugin_iv" PUBLIC dbghelp)
target_compile_definitions("plugin_iv" PUBLIC NOMINMAX)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

else()
  target_compile_features("plugin_iv" PRIVATE cxx_auto_type)
endif()

#56 8b 74 24 0c 8b 04 b5 ? ? ? ? 8b c8 c1 e9 18
