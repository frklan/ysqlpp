cmake_minimum_required(VERSION 3.21)

project(ysqlpp CXX)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
if(NOT TARGET project_options)
  add_library(project_options INTERFACE)
endif()


if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
if(NOT TARGET project_warnings)
  add_library(project_warnings INTERFACE)
endif()

# enable cache system
include(cmake/Cache.cmake)

# Add linker configuration
include(cmake/Linker.cmake)
configure_linker(project_options)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
  #
  # consider breaking this out per project as necessary
  target_precompile_headers(
    project_options
    INTERFACE
    <vector>
    <string>
    <map>
    <utility>)
endif()

option(ENABLE_CONAN "Use Conan for dependency management" ON)
if(ENABLE_CONAN)
  include(cmake/Conan.cmake)
  run_conan()
endif()

option(ENABLE_TESTING "Build ysqlpp Tests" OFF)
if(ENABLE_TESTING)
  enable_testing()
  message("Building Tests")
  add_subdirectory(test)
endif()

option(ENABLE_FUZZING "Build ysqlpp Fuzz Tests" OFF)
if(ENABLE_FUZZING)
  message("Building Fuzz Tests")
  add_subdirectory(fuzz_test)
endif()

option(ENABLE_EXAMPLES "Build ysqlpp example programs" OFF)
if(ENABLE_EXAMPLES)
  message("Building Examples")
  add_subdirectory(examples/simple)
endif()


add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(
  ${PROJECT_NAME}
  INTERFACE
    project_options
    project_warnings
)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/>"
    "$<INSTALL_INTERFACE:include>")

# Ugly hack to make static analyzers work..
if(ENABLE_CPPCHECK OR ENABLE_CLANG_TIDY OR ENABLE_INCLUDE_WHAT_YOU_USE)
  find_package(SQLite3)
  add_executable(dummy src/main.cpp)
  target_compile_features(dummy PRIVATE cxx_std_20)
  target_link_libraries(
    dummy
    PRIVATE
      project_options
      project_warnings
      SQLite3
      ${PROJECT_NAME}
  )
endif()
