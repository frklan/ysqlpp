find_package(spdlog)
find_package(SQLite3)

add_executable(${PROJECT_NAME}-simple main.cpp)

target_compile_features(${PROJECT_NAME}-simple PRIVATE cxx_std_20)

target_link_libraries(
  ${PROJECT_NAME}-simple
  PRIVATE
    project_options
    project_warnings
    spdlog::spdlog
    SQLite3
    ${PROJECT_NAME}
)
