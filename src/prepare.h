#pragma once

#include <fmt/format.h>
#include <string_view>
#include <utility>

#include <sqlite3.h>

#include "db.h"

namespace y44::ysqlpp::impl {
  [[nodiscard]] std::pair<sqlite3_stmt *, const char *> prepare(y44::ysqlpp::DB &db, std::string_view sql) {
    const char *s = sql.data();
    const char *remain = nullptr;
    sqlite3_stmt *stmt = nullptr;
    auto rc = sqlite3_prepare(db, s, -1, &stmt, &remain);
    if(rc != SQLITE_OK) {
      auto *msg = sqlite3_errmsg(db);
      auto c = sqlite3_extended_errcode(db);
      throw std::runtime_error(fmt::format("{}, {}", msg, c));
    }
    return { stmt, remain };
  }
}// namespace y44::ysqlpp::impl

namespace y44::ysqlpp {

  [[nodiscard]] sqlite3_stmt *prepare_single(y44::ysqlpp::DB &db, std::string_view sql) {
    return y44::ysqlpp::impl::prepare(db, sql).first;
  }
}// namespace y44::ysqlpp
