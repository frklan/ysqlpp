// Copyright (C) 2022, Fredrik Andersson
// SPDX-License-Identifier: CC-BY-NC-4.0

#pragma once

#include <memory>

#include "sqlite3.h"

namespace y44::ysqlpp {
  class Stmt {
    public:
      Stmt() = delete;

      explicit Stmt(sqlite3_stmt *stmt) : m_stmt(stmt, &sqlite3_finalize) {}

      // move
      Stmt(Stmt &&rhs) noexcept : m_stmt(nullptr, &sqlite3_finalize) {
        std::swap(m_stmt, rhs.m_stmt);
      }

      // copy
      Stmt(const Stmt &) = delete;

      ~Stmt() noexcept = default;

      // copy assignment operator
      Stmt operator=(const Stmt &) = delete;

      // move asignment operator
      Stmt &operator=(Stmt &&rhs) noexcept {
        std::swap(m_stmt, rhs.m_stmt);
        return *this;
      }

      explicit operator sqlite3_stmt *() {
        return m_stmt.get();
      }

      sqlite3_stmt *get() noexcept {
        return m_stmt.get();
      }

    private:
      std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> m_stmt;
  };

}// namespace y44::ysqlpp