// Copyright (C) 2021-2022, Fredrik Andersson
// SPDX-License-Identifier: CC-BY-NC-4.0

#pragma once

#include <memory>

#include <sqlite3.h>

namespace y44::ysqlpp {
  class DB {
    public:
      DB() : m_db(nullptr, &sqlite3_close) {}

      explicit DB(sqlite3 *db) : m_db(db, &sqlite3_close) {}

      // move
      DB(DB &&rhs)
      noexcept : m_db(nullptr, &sqlite3_close) {
        std::swap(m_db, rhs.m_db);
      }

      // copy
      DB(const DB &) = delete;

      ~DB() noexcept = default;

      // copy assignment operator
      DB operator=(const DB &) = delete;

      // move asignment operator
      DB &operator=(DB &&rhs) noexcept {
        std::swap(m_db, rhs.m_db);
        return *this;
      }

      sqlite3 *get() noexcept {
        return m_db.get();
      }

    private:
      std::unique_ptr<sqlite3, decltype(&sqlite3_close)> m_db;
  };


}// namespace y44::ysqlpp
