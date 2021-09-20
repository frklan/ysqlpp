
find_package(SQLite3)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE .)

target_link_libraries(
  ${PROJECT_NAME}
  INTERFACE
  project_options
  project_warnings
  ${SQLite3_LIBRARIES})