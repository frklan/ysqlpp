// Copyright (C) 2021, Fredrik Andersson
// SPDX-License-Identifier: CC-BY-NC-4.0

#pragma once
#include <filesystem>
#include <string_view>

#include <sqlite3.h>

#include "db.h"

namespace y44::ysqlpp {
  [[nodiscard]] inline y44::ysqlpp::DB open(const std::filesystem::path &path) {
    sqlite3 *s_db = nullptr;
    sqlite3_open(path.c_str(), &s_db);
    y44::ysqlpp::DB db{s_db};
    return db;
  }
}// namespace y44::ysqlpp
