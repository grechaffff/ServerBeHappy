#include "globals.h"


boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv13);

// Your data for connecting to the database
pqxx::connection conn("host=localhost port=6969 dbname=UsersBeHappy user=postgres password=256869 options='-c client_encoding=UTF8 ' ");
pqxx::work txn(conn);