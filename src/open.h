#pragma once

#include <filesystem>
#include <string_view>

#include <sqlite3.h>

#include "db.h"

namespace y44::ysqlpp {
  [[nodiscard]] y44::ysqlpp::DB open(std::filesystem::path path) {
    y44::ysqlpp::DB db;
    sqlite3_open(path.c_str(), db);
    return db;
  }
}// namespace y44::ysqlpp