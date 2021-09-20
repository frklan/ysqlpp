#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <variant>

#include <sqlite3.h>

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


  value_t value(sqlite3_stmt *stmt, int col) {
    auto type = sqlite3_column_type(stmt, col);
    // const unsigned char *s = nullptr;
    switch(type) {
    case SQLITE_INTEGER:
      return {sqlite3_column_int64(stmt, col)};
    case SQLITE_FLOAT:
      return {sqlite3_column_double(stmt, col)};
    case SQLITE_BLOB:
      throw new std::runtime_error("Blobs are not supported");
    case SQLITE_NULL:
      throw new std::runtime_error("null are not supported");
    default:
      auto s = reinterpret_cast<const char *>(sqlite3_column_text(stmt, col));
      return {s ? s : ""};
    }
  }

  template <typename F, std::size_t... idx>
  void call(F &&f, sqlite3_stmt *stmt, std::index_sequence<idx...>) {
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
  bool step(sqlite3_stmt *stmt, F &&f) {
    if(sqlite3_step(stmt) != SQLITE_ROW) {
      return false;
    }

    auto c = std::make_index_sequence<impl::arity<std::remove_reference_t<decltype(f)>>::count>{};
    impl::call(std::forward<F>(f), stmt, c);
    return true;
  }

}// namespace y44::ysqlpp