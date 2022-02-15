// Copyright (C) 2021-2022, Fredrik Andersson
// SPDX-License-Identifier: CC-BY-NC-4.0

#pragma once

#include <utility>

#include <sqlite3.h>

#include "step.h"
#include "stmt.h"

namespace y44::ysqlpp {
  template <typename F>
  void for_each(Stmt &stmt, F &&f) {
    while(auto r = step(stmt, std::forward<F>(f))) {
    }

    sqlite3_reset(stmt.get());
  }
}// namespace y44::ysqlpp
