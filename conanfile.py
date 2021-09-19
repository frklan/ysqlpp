from conans import ConanFile

class YsqlppProject(ConanFile):
    # Note: options are copied from CMake boolean options.
    # When turned off, CMake sometimes passes them as empty strings.
    options = {
    }
    name = "ysqlpp"
    version = "0.1"
    requires = (
        "catch2/2.13.7",
        "docopt.cpp/0.6.2",
        "fmt/8.0.1",
        "spdlog/1.9.2",
        "sqlite3/3.36.0"
    )
    generators = "cmake", "gcc", "txt", "cmake_find_package"
    