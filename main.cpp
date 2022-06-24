#include "hafez.hpp"

int main() {

  hafez h;

  // h.set_token(your token);
  // first of first create a mysql database then import faals.sql
  // h.set_db("db_username", "your password", " db_name for example hafez_discord");
  h.start();

  return 0;
}
