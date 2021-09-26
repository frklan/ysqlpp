// Copyright (C) 2021, Fredrik Andersson
// SPDX-License-Identifier: CC-BY-NC-4.0

#pragma once

#include <sqlite3.h>

namespace y44::ysqlpp {
  class DB {
    public:
      DB() = default;

      explicit DB(sqlite3 *db) : m_db(db) {}

      // move
      DB(DB &&rhs)
      noexcept : m_db(rhs.m_db) {
        rhs.m_db = nullptr;
      }

      // copy
      DB(const DB &) = delete;

      // copy assignment operator
      DB operator=(const DB &) = delete;

      // move asignment operator
      DB &operator=(DB &&rhs) noexcept {
        reset();
        m_db = rhs.m_db;
        rhs.m_db = nullptr;
        return *this;
      }

      /*operator sqlite3 *() {
        return m_db;
      }

      operator sqlite3 **() {
        return &m_db;
      }*/

      sqlite3 *get() noexcept {
        return m_db;
      }
      ~DB() noexcept {
        close();
      }

      void close() noexcept {
        if(m_db == nullptr) {
          return;
        }
        sqlite3_close(m_db);
        m_db = nullptr;
      }

      void reset() noexcept {
        close();
        m_db = nullptr;
      }

    private:
      sqlite3 *m_db = nullptr;
  };


}// namespace y44::ysqlpp
