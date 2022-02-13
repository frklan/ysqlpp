from conans import ConanFile

class YsqlppProject(ConanFile):
    options = {
    }
    name = "ysqlpp"
    version = "0.1"
    requires = (
        "spdlog/1.9.2",
        "sqlite3/3.36.0",
        "catch2/2.13.8",
        "fmt/8.1.1",
        "spdlog/1.9.2"
    )
    generators = "cmake", "gcc", "txt", "cmake_find_package"
    