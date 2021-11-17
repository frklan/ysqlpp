from conans import ConanFile

class YsqlppProject(ConanFile):
    options = {
    }
    name = "ysqlpp"
    version = "0.1"
    requires = (
        "spdlog/1.9.2",
        "sqlite3/3.36.0"
    )
    generators = "cmake", "gcc", "txt", "cmake_find_package"
    