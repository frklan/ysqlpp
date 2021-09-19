
find_package(fmt)
find_package(spdlog)
find_package(docopt)

#Generic test that uses conan libs
#add_executable(ysqlpp main.cpp)
add_executable(ysqlpp-simple main.cpp)
target_link_libraries(
  ysqlpp-simple
  PRIVATE project_options
  project_warnings
  docopt::docopt
  fmt::fmt
  spdlog::spdlog
  ysqlpp)

target_include_directories(ysqlpp-simple PRIVATE ${PROJECT_SOURCE_DIR}/../src)