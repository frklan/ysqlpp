// Copyright (C) 2021-2022, Fredrik Andersson
// SPDX-License-Identifier: CC-BY-NC-4.0

#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <variant>

#include <sqlite3.h>

#include "stmt.h"

namespace y44::ysqlpp::impl {

  struct value_t : public std::variant<int64_t, double, std::string_view> {
      operator int64_t() const {
        return std::get<int64_t>(*this);
      }

      operator double() const {
        return std::get<double>(*this);
      }

      operator std::string_view() {
        return std::get<std::string_view>(*this);
      }

      operator std::string() {
        return std::string{std::get<std::string_view>(*this)};
      }
  };

  inline value_t value(Stmt &stmt, int col) {
    auto type = sqlite3_column_type(static_cast<sqlite3_stmt *>(stmt), col);
    // const unsigned char *s = nullptr;
    switch(type) {
    case SQLITE_INTEGER:
      return {sqlite3_column_int64(static_cast<sqlite3_stmt *>(stmt), col)};
    case SQLITE_FLOAT:
      return {sqlite3_column_double(static_cast<sqlite3_stmt *>(stmt), col)};
    case SQLITE_BLOB:
      throw std::runtime_error("Blobs are not supported");
    case SQLITE_NULL:
      throw std::runtime_error("null are not supported");
    default:
      const auto *s = reinterpret_cast<const char *>(sqlite3_column_text(stmt.get(), col));
      return {s != nullptr ? s : ""};
    }
  }

  template <typename F, std::size_t... idx>
  void call(F &&f, Stmt &stmt, std::index_sequence<idx...>) {
    f(value(stmt, idx)...);
  }

  template <typename F>
  struct arity : arity<decltype(&F::operator())> {};

  template <typename F, typename R, typename... Args>
  struct arity<R (F::*)(Args...) const> {
      static const int count = sizeof...(Args);
  };
}// namespace y44::ysqlpp::impl

namespace y44::ysqlpp {
  template <typename F>
  bool step(Stmt &stmt, F &&f) {

    if(sqlite3_step(static_cast<sqlite3_stmt *>(stmt)) != SQLITE_ROW) {
      return false;
    }

    auto c = std::make_index_sequence<impl::arity<std::remove_reference_t<decltype(f)>>::count>{};
    impl::call(std::forward<F>(f), stmt, c);
    return true;
  }

}// namespace y44::ysqlpp
