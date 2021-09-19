#pragma once

#include <utility>

#include <sqlite3.h>

#include "step.h"

namespace y44::ysqlpp {
  template <typename F>
  void for_each(sqlite3_stmt *stmt, F &&f) {
    while(auto r = step(stmt, std::forward<F>(f))) {
    }

    sqlite3_reset(stmt);
  }
}// namespace y44::ysqlpp