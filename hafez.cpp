#include "hafez.hpp"
#include <dpp/dpp.h>
#include <random>
#include <string>

// error handler for mysql
void hafez::finish_with_error(MYSQL *con, std::string msg) noexcept {
  std::cout << "Error ! : " << msg << " " << mysql_error(con) << std::endl;
  mysql_close(con);
  exit(1);
}
// set bot token
void hafez::set_token(std::string const &token) noexcept { TOKEN = token; }

void hafez::set_db(std::string const &user_name, std::string const &password,
                   std::string const &db_name) noexcept {
  db_username = user_name;
  db_password = password;
  db_dbname = db_name;
}

// generate random numbers to choose from database
void hafez::rand_maker() noexcept {
  std::random_device rd;
  std::uniform_int_distribution<int> dist(0, 495);
  auto number{dist(rd)};
  poem_for_sql =
      "SELECT Poem FROM faals WHERE id=" + std::to_string(number) + ";";
  interpretation_for_sql =
      "SELECT Interpretation FROM faals WHERE id=" + std::to_string(number) +
      ";";
}

void hafez::do_mysql() noexcept {
  MYSQL *conn;
  if ((conn = mysql_init(NULL)) == NULL) {
    finish_with_error(conn, "mysql_init");
  }

  if (mysql_real_connect(conn, "127.0.0.1", db_username.c_str(),
                         db_password.c_str(), db_dbname.c_str(), 0, NULL,
                         0) == NULL) {

    finish_with_error(conn, "real_connect");
  }

  if (mysql_query(conn, poem_for_sql.c_str()) != 0) {
    finish_with_error(conn, "query");
  }

  MYSQL_RES *result = mysql_store_result(conn);

  if (result == NULL) {
    finish_with_error(conn, "store result");
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;

  while ((row = mysql_fetch_row(result))) {
    for (int i = 0; i < num_fields; i++) {
      poem += row[i] ? row[i] : "NULL";
    }

    poem += " \n";
  }

  mysql_free_result(result);

  /////////////////////////////////////////////////////////////////////
  // query for interpretation

  if (mysql_query(conn, interpretation_for_sql.c_str()) != 0) {
    finish_with_error(conn, "query2");
  }

  auto result2 = mysql_store_result(conn);

  if (result2 == NULL) {
    finish_with_error(conn, "store result2");
  }

  auto num_fields2 = mysql_num_fields(result2);

  MYSQL_ROW row2;

  while ((row2 = mysql_fetch_row(result2))) {
    for (int i = 0; i < num_fields2; i++) {
      interpretation += row2[i] ? row2[i] : "NULL";
    }

    poem += " \n";
  }

  mysql_free_result(result2);

  mysql_close(conn);
}

void hafez::do_discord() noexcept {

  dpp::cluster bot(TOKEN);

  bot.on_log(dpp::utility::cout_logger());

  bot.on_slashcommand([&](const dpp::slashcommand_t &event) {
    if (event.command.get_command_name() == "فال") {
      rand_maker();
      do_mysql();
      event.reply("شعر : \n\n" + poem + " تعبیر : \n\n\n" + interpretation);
      poem.clear();
      interpretation.clear();
    }
  });

  bot.on_ready([&bot](const dpp::ready_t &event) {
    if (dpp::run_once<struct register_bot_commands>()) {
      bot.global_command_create(
          dpp::slashcommand("فال", "دریافت فال حافظ", bot.me.id));
    }
  });

  bot.start(false);
}

// cool interface isn't it ?
void hafez::start() noexcept { do_discord(); }