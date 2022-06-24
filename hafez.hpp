#ifndef __hafez_discord__
#define __hafez_discord__
#include <iostream>
#include <mysql/mysql.h>

class hafez {
public:
  void start() noexcept;
  void set_token(std::string const &token) noexcept;
  void set_db(std::string const &user_name, std::string const &password,
              std::string const &db_name) noexcept;

private: // for methods
  void finish_with_error(MYSQL *con, std::string msg) noexcept;
  // generates random numbers for database
  void rand_maker() noexcept;
  // select a poem from database
  void do_mysql() noexcept;
  // send the result to discord
  void do_discord() noexcept;

private: // for variables
  std::string poem{};
  std::string interpretation{};
  std::string poem_for_sql;
  std::string interpretation_for_sql;

  std::string TOKEN{};

  std::string db_username;
  std::string db_password;
  std::string db_dbname;
};

#endif
