#pragma once


#ifndef GLOBALS_H
#define GLOBALS_H

#include"tcp_connection.h"
#include"Globals.h"
#include<pqxx/pqxx>

extern boost::asio::ssl::context ctx;

extern pqxx::connection conn;
extern pqxx::work txn;

#endif // !GLOBALS_H