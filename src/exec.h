#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

#include <sqlite3.h>

#include "db.h"
#include "prepare.h"
#include "step.h"

namespace y44::ysqlpp::impl {
  std::string trim(std::string str) {
    auto start = str.find_first_not_of(' ');
    if(start == std::string::npos) {
      return "";
    }
    auto end = str.find_last_not_of(' ');
    auto s = str.substr(start, end - start + 1);
    return s;
  }
}// namespace y44::ysqlpp::impl

namespace y44::ysqlpp {
  void exec(y44::ysqlpp::DB &db, std::string_view sql) {
    std::string s{sql};

    do {
      auto ss = impl::trim(s);
      auto [stmt, remainin] = impl::prepare(db, ss);

      step(stmt, []() {});
      s = remainin;
    } while(s[0]);
  }
}// namespace y44::ysqlpp