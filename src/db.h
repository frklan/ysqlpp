#pragma once

#include <sqlite3.h>

namespace y44::ysqlpp {
  class DB {
    public:
      DB() = default;

      explicit DB(sqlite3 *db) : m_db(db) {}

      DB(DB&& rhs) noexcept :
      m_db(rhs.m_db) {
        rhs.m_db = nullptr;
      }

      operator sqlite3*() {
        return m_db;
      }

      operator sqlite3**() {
        return &m_db;
      }

      DB& operator=(DB &&rhs) noexcept {
        reset();
        m_db = rhs.m_db;
        rhs.m_db = nullptr;
        return *this;
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



} // namespace y44::ysqlpp
