#pragma once

#include <string_view>

#include <sqlite3.h>

#include "db.h"

namespace y44::ysqlpp {
  [[nodiscard]] y44::ysqlpp::DB open(std::string_view path) {
    y44::ysqlpp::DB db;
    sqlite3_open(path.data(), db);
    return db;
  }
}// namespace y44::ysqlpp