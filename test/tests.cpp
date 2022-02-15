// Copyright (C) 2021-2022, Fredrik Andersson
// SPDX-License-Identifier: CC-BY-NC-4.0

#include <algorithm>
#include <filesystem>
#include <fmt/core.h>
#include <iostream>
#include <random>
#include <vector>

#include <catch2/catch.hpp>
#include <fmt/format.h>

#include "../include/ysqlpp/ysqlpp.h"
#include "ysqlpp/for_each.h"
#include "ysqlpp/prepare.h"

std::filesystem::path get_random_file() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist{1000L, 9999L};// NOLINT

  auto f = std::filesystem::temp_directory_path();
  do {
    f /= std::to_string(dist(gen));
  } while(std::filesystem::directory_entry(f).exists());

  return f;
}

y44::ysqlpp::DB create_db(const std::filesystem::path &db_path) {
  auto db = y44::ysqlpp::open(db_path);

  y44::ysqlpp::exec(db, "create table if not exists MYTABLE (NAME text, VALUE float);");

  y44::ysqlpp::exec(db, "insert into MYTABLE (NAME, VALUE) values ('Name0', 0 );");
  y44::ysqlpp::exec(db, "insert into MYTABLE (NAME, VALUE) values ('Name1', 1 );");
  y44::ysqlpp::exec(db, "insert into MYTABLE (NAME, VALUE) values ('Name2', 2 );");

  return db;
}

TEST_CASE("Create DB", "[CREATE DB]") {
  const auto db_path = get_random_file();
  {
    auto db = create_db(db_path);
    REQUIRE(std::filesystem::directory_entry(db_path).exists());
  }
  std::filesystem::remove(db_path);
}

TEST_CASE("step", "[step]") {
  const auto db_path = get_random_file();
  {
    auto db = create_db(db_path);

    auto stmt = y44::ysqlpp::prepare_single(db, "select count(NAME) from MYTABLE;");

    int64_t count_rows{0};
    y44::ysqlpp::step(stmt, [&count_rows](int64_t c) {
      count_rows = c;
    });

    REQUIRE(count_rows == 3);
  }
  std::filesystem::remove(db_path);
}

TEST_CASE("For Each Row", "[for_each]") {
  const auto db_path = get_random_file();
  {
    auto db = create_db(db_path);

    auto stmt = y44::ysqlpp::prepare_single(db, "select NAME from MYTABLE;");
    std::vector<std::string> vec;
    y44::ysqlpp::for_each(stmt, [&vec](const std::string &name) {
      vec.push_back(name);
    });

    REQUIRE(vec.size() == 3);
    REQUIRE(vec[0] == "Name0");
    REQUIRE(vec[1] == "Name1");
    REQUIRE(vec[2] == "Name2");
  }
  std::filesystem::remove(db_path);
}


TEST_CASE("pass db as shared_ptr", "[shared_ptr]") {
  const auto db_path = get_random_file();
  {
    auto db = std::make_shared<y44::ysqlpp::DB>(create_db(db_path));

    auto stmt = y44::ysqlpp::prepare_single(*db, "select count(NAME) from MYTABLE;");

    int64_t count_rows{0};
    y44::ysqlpp::step(stmt, [&count_rows](int64_t c) {
      count_rows = c;
    });

    REQUIRE(count_rows == 3);
  }
  std::filesystem::remove(db_path);
}


TEST_CASE("Many db write and reads", "[READ/Write DB]") {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<double> dist{0.0, 999999.0};// NOLINT
  dist(gen);

  // std::vector<double> data_values(100'000);
  // std::generate(data_values.begin(), data_values.end(), [&dist, &gen]() { return dist(gen); });
  const size_t COUNT_DATA_VALUES = 100'000;
  const auto db_path = get_random_file();
  {
    auto db = y44::ysqlpp::open(db_path);
    y44::ysqlpp::exec(db, "create table if not exists MYTABLE (NAME text, VALUE float);");

    for(size_t i = 0; i < COUNT_DATA_VALUES; i++) {
      auto sql = fmt::format("insert into MYTABLE (NAME, VALUE) values ('Name0', {} );", dist(gen));
      y44::ysqlpp::exec(db, sql);
    }

    /*********/

    auto stmt = y44::ysqlpp::prepare_single(db, "select VALUE from MYTABLE");
    size_t row_count{};
    y44::ysqlpp::for_each(stmt, [&row_count](double /*v*/) {
      ++row_count;
    });
    REQUIRE(row_count == COUNT_DATA_VALUES);
  }

  std::filesystem::remove(db_path);
}