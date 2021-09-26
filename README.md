# YSQLPP

My take at a C++20 header only wrapper for SQLlite3; to better learn and understand a bit more complex c++20 I used [msqlite](https://github.com/ricardocosme/msqlite.git) as a template/inspiration for my own driver. I must sya that I'm pretty happy with the outcome so far, although this code has a long way to go before its production ready.

**Note** Beware of bugs and UB.

## Dependencies

### Necessary Dependencies

* A C++ compiler that supports C++20.
* [SQLite3](https://sqlite.org)
* [Conan](https://conan.io/)
* [CMake](https://cmake.org/)

### Optional Dependencies

* [Doxygen](http://doxygen.nl/)
* [ccache](https://ccache.dev/)
* [Cppcheck](http://cppcheck.sourceforge.net/)
* [include-what-you-use](https://include-what-you-use.org/)

## Using

API documentation does no really exists yet, however the goal is to have a clean and simple inferface:

```c++
#include <filesystem>
#include <iostream>

#include <ysqlpp/ysqlpp.h>

int main(int /*argc*/, const char ** /*argv*/) {
  const std::filesystem::path db_file{"test.db"};

  try {
    auto db = y44::ysqlpp::open(db_file);

    exec(db, "create table if not exists MY_TABLE (NAME text, VALUE float);");
    exec(db, "insert into MY_TABLE (NAME, VALUE) values ('Name1', " + 123 + ");");
    exec(db, "insert into MY_TABLE (NAME, VALUE) values ('Name2', " + 321 + ");");

    auto *stmt = y44::ysqlpp::prepare_single(db, "select * from MY_TABLE;");
    y44::ysqlpp::for_each(stmt, [](const std::string &name, double val) {
      std::clog << name << '\t' val << '\n';
    });
  } catch(std::runtime_error &e) {
    /* Handle error */
  }
  return 0;
}
```

ysqlpp must be added CMakeFile.txt like so

```cmake
find_package(SQLite3)
include(FetchContent)

FetchContent_Declare(ysqlpp
  GIT_REPOSITORY "https://github.com/frklan/ysqlpp"
  GIT_TAG ""
)
FetchContent_MakeAvailable(ysqlpp)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE 
    ...
    SQLite3
    ysqlpp
)
```

Trigger a build by issuing the following commands:

```bash
$ take build
$ cmake .. -DENABLE_FUZZING=OFF -DENABLE_TESTING=OFF -DWARNINGS_AS_ERRORS=OFF -DENABLE_DOXYGEN=OfF  -DCMAKE_BUILD_TYPE=RelWithDebInfo  -DENABLE_CACHE=ON -DENABLE_CPPCHECK=ON -DENABLE_CLANG_TIDY=ON -DENABLE_INCLUDE_WHAT_YOU_USE=OFF

$ make -j
```

Useful cmake options are:

```bash
-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON 
-DCMAKE_BUILD_TYPE=DEBUG
-DENABLE_IPO:BOOL=ON
-DENABLE_COVERAGE:BOOL=ON 
-DENABLE_SANITIZER_ADDRESS:BOOL=ON 
-DENABLE_SANITIZER_LEAK:BOOL=ON 
-DENABLE_SANITIZER_UNDEFINED_BEHAVIOR:BOOL=ON  
-DENABLE_SANITIZER_MEMORY:BOOL=ON
```

## Contributing

Contributions are always welcome!

When contributing to this repository, please first discuss the change you wish to make via the issue tracker, email, or any other method with the owner of this repository before making a change.

Please note that we have a code of conduct, you are required to follow it in all your interactions with the project.

## Versioning

We use SemVer for versioning. For the versions available, see the tags on the repository.

## Authors

- Fredrik Andersson - frklan


## License

This project is licensed under the CC BY-NC-SA License - see the [LICENSE](License) file for details

For commercial/proprietary licensing, please contact the project owner
