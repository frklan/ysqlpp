// Copyright (C) 2021-2022, Fredrik Andersson
// SPDX-License-Identifier: CC-BY-NC-4.0

#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <sqlite3.h>

#include "db.h"
#include "stmt.h"

namespace y44::ysqlpp::impl {
  [[nodiscard]] inline std::pair<Stmt, const char *> prepare(y44::ysqlpp::DB &db, std::string_view sql) {
    const char *s = sql.data();
    const char *remain = nullptr;
    sqlite3_stmt *stmt = nullptr;
    auto rc = sqlite3_prepare(db.get(), s, -1, &stmt, &remain);
    if(rc != SQLITE_OK /*&& stmt != nullptr*/) {
      const auto *msg = sqlite3_errmsg(db.get());
      auto c = sqlite3_extended_errcode(db.get());
      throw std::runtime_error(std::string{msg} + " (" + std::to_string(c) + ")");
    }

    return {Stmt{stmt}, remain};
  }
}// namespace y44::ysqlpp::impl

namespace y44::ysqlpp {
  [[nodiscard]] inline Stmt prepare_single(y44::ysqlpp::DB &db, std::string_view sql) {
    return y44::ysqlpp::impl::prepare(db, sql).first;
  }
}// namespace y44::ysqlpp
