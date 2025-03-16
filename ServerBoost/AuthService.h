#pragma once
#include"Globals.h"

class AuthService 
{


public:

    explicit AuthService(tcp_connection::pointer connection) : connection_(move(connection)) {}

	void recvData(string& data , ssl::stream<tcp::socket > &socket_);

    void Registration(ssl::stream<tcp::socket >& socket_);
    bool Login();
    void generate_salt(uint8_t* salt, size_t salt_length);

private:


    string responseChoice;
    string username;
    string password;
    string email;

    tcp_connection::pointer connection_;

    uint32_t t_cost = 4;
    uint32_t m_cost = 65536;
    uint32_t parallelism = 2;
    const size_t salt_length = 16;

    
    uint8_t hash[32];
    uint32_t hashlen = 32;
   
};