cmake_minimum_required(VERSION 3.21)

project(ysqlpp-simple CXX)

find_package(fmt)
find_package(spdlog)
find_package(docopt)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE project_options
  project_warnings
  docopt::docopt
  fmt::fmt
  spdlog::spdlog)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  ysqlpp)