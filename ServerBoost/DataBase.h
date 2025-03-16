#pragma once
#include"Globals.h"

class DataBase {

private:

    unique_ptr<pqxx::connection> conn;
  

public:
    DataBase() = default;
    ~DataBase() { CloseConnection(); }

   

	void ConnectBase();

    void CloseConnection();
	void SaveToDataBaseInfo(string& username, string& email, string& hashedPassword, const std::vector<uint8_t>& salt, uint32_t t_cost, uint32_t m_cost, uint32_t parallelism , tcp_connection::pointer connection);
};