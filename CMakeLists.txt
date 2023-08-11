cmake_minimum_required(VERSION 3.4...3.18)
project(cdt3d)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "" FORCE)
  message(STATUS "Set build type to default: ${CMAKE_BUILD_TYPE}")
else()
  message(STATUS "Your build type: ${CMAKE_BUILD_TYPE}")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

include_directories(${PROJECT_SOURCE_DIR}/headers/include)

set(CMAKE_CXX_STANDARD 11)
set(PYBIND11_CPP_STANDARD -std=c++11)

add_subdirectory(pybind11)
pybind11_add_module(_cdt3d cdt_bindings.cpp)

target_compile_definitions(_cdt3d PRIVATE VERSION_INFO=${CDT3D_VERSION_INFO})
