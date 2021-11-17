// Copyright (C) 2021, Fredrik Andersson
// SPDX-License-Identifier: CC-BY-NC-4.0

#include <exception>
#include <filesystem>
#include <functional>
#include <iostream>
#include <random>

#include <spdlog/spdlog.h>

#include <ysqlpp/ysqlpp.h>


int main(int /*argc*/, const char ** /*argv*/) {
  spdlog::info("Hello, {}!", "World");

  const std::filesystem::path db_file{"test.db"};

  try {
    auto db = y44::ysqlpp::open(db_file);
    exec(db, "create table if not exists ROOM (NAME text, VALUE float);");

    std::random_device rd;
    std::uniform_real_distribution<double> dist{1.0, 100.0};
    exec(db, "insert into ROOM (NAME, VALUE) values ('Fredrik', " + std::to_string(dist(rd)) + ");");
    exec(db, "insert into ROOM (NAME, VALUE) values ('Hanna', " + std::to_string(dist(rd)) + ");");
    exec(db, "insert into ROOM (NAME, VALUE) values ('Emma', " + std::to_string(dist(rd)) + ");");
    exec(db, "insert into ROOM (NAME, VALUE) values ('Liza', " + std::to_string(dist(rd)) + ");");

    auto *stmt = y44::ysqlpp::prepare_single(db, "select * from ROOM;");
    y44::ysqlpp::for_each(stmt, [](const std::string &name, double val) {
      spdlog::info("{}:{}\n", name, val);
    });
  } catch(std::runtime_error &e) {
    spdlog::error("{}", e.what());
  }

  return 0;
}
