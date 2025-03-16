#include "DataBase.h"
#include"tcp_connection.h"

void DataBase::ConnectBase()
{

    try {
        if (!conn || !conn->is_open()) {
          
            // Your data for connecting to the database
            conn = make_unique<pqxx::connection>("host=localhost port=6969 dbname=UsersBeHappy user=postgres password=pass123 options='-c client_encoding=UTF8'");
            if (conn->is_open()) {
                cout << "Connected to database: " << conn->dbname() << endl;
            }
            else {
                throw runtime_error("Failed to open database connection.");
            }
        }
    }
    catch (const exception& e) {
        cerr << "Database connection error: " << e.what() << endl;
        throw;
    }
}

void DataBase::CloseConnection()
{
    if (conn && conn->is_open()) {
        std::cout << "Closing database connection..." << endl;
        conn->close();
        conn.reset();  // Уничтожаем объект соединения
    }
}





void DataBase::SaveToDataBaseInfo(string& username, string& email, string & hashedPassword, const std::vector<uint8_t>& salt, uint32_t t_cost , uint32_t m_cost , uint32_t parallelism , tcp_connection::pointer connection) 
{

    try
    {
        ConnectBase();  // Подключаемся к базе, если не подключены

        pqxx::work txn(*conn);  // Создаём транзакцию


        std::ostringstream saltStream;
        for (uint8_t i : salt) {
            saltStream << std::hex << std::setw(2) << std::setfill('0') << (int)i;
        }
        std::string saltHex = saltStream.str();
       
       // cout << "saltHex:" << saltHex << endl;

        txn.exec(
            "INSERT INTO users (username, email , password_hash , salt , t_cost , m_cost , parallelism) VALUES ($1, $2, $3 , $4 , $5 , $6 , $7)",
            pqxx::params(username, email, hashedPassword , saltHex , t_cost , m_cost , parallelism) // Передача параметров через pqxx::params
        );
        
       
        
        txn.commit();
        cout << "User data saved successfully." << endl;
    }
    catch (const std::exception& e)
    {
        cerr << "ErrorSave: " << e.what() << endl;
        std::string response = "User not uniq";
        connection->send_message(response);
       // exit(EXIT_FAILURE);
        return;
    }

}
