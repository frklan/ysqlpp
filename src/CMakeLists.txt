
find_package(fmt)
find_package(spdlog)
find_package(docopt)
find_package(SQLite3)

#Generic test that uses conan libs
add_library(ysqlpp STATIC ysqlpp.cpp)
target_link_libraries(
  ysqlpp
  PRIVATE project_options
  project_warnings
  docopt::docopt
  fmt::fmt
  spdlog::spdlog
  ${SQLite3_LIBRARIES})
  
set_target_properties(ysqlpp PROPERTIES PUBLIC_HEADER ysqlpp.h)
#target_include_directories(ysqlpp SYSTEM PRIVATE ${SQLITE3_INCLUDE_DIRS})
target_include_directories(ysqlpp PRIVATE .)  
