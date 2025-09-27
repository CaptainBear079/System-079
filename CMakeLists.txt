cmake_minimum_required(VERSION 3.10)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output-Directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libraries/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libraries/dll)

# Build-Typen
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# BUILD CONFIGURATION

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  # DEBUG
  project(TBaGB079__DEBUG)
  message(STATUS "Build type: Debug")
  # Executables
  add_executable(${PROJECT_NAME} main.cpp)
  target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_BUILD)
  target_compile_options(${PROJECT_NAME} PRIVATE -g)
  
  # Libraries Test Program
  add_library(TBaGB079_LIB STATIC main.cpp)
  add_executable(LIB_TEST lib_test.cpp)
  target_link_libraries(LIB_TEST PRIVATE TBaGB079_LIB__DEBUG)
  
  add_library(TBaGB079_DLL SHARED main.cpp)
  add_executable(DLL_TEST dll_test.cpp)
  target_link_libraries(DLL_TEST PRIVATE TBaGB079_LIB__DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  project(TBaGB079)
  message(STATUS "Build type: Release")
  add_executable(${PROJECT_NAME} main.cpp)
  target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
  target_compile_options(${PROJECT_NAME} PRIVATE -O3)
  add_library(TBaGB079_LIB STATIC main.cpp)
  add_library(TBaGB079_DLL SHARED main.cpp)
endif()