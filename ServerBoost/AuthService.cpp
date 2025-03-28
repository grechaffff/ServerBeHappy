#include "AuthService.h"
#include"DataBase.h"
#include"Globals.h"
#include"tcp_connection.h"

void AuthService::recvData(string& data , ssl::stream<tcp::socket >& socket_)
{


    stringstream ss(data);

    getline(ss, responseChoice, '|');
    getline(ss, username, '|');
    getline(ss, email, '|');
    getline(ss, password, '|');


    
	if (strcmp(responseChoice.c_str(), "Register") == 0) {

		Registration(socket_);
	}

	if (strcmp(responseChoice.c_str(), "Login") == 0) {

		Login();

	}

}

void AuthService::Registration(ssl::stream<tcp::socket >& socket_)
{
    vector<uint8_t> salt(16);

	generate_salt(salt.data(), salt.size());

   
   


	int result = argon2id_hash_raw(t_cost, m_cost, parallelism,
		password.c_str(), strlen(password.c_str()),
		salt.data(), salt.size(),
		hash, hashlen);

	//std::cout << "Response Choice: " << responseChoice << std::endl;
	std::cout << "Username: " << username << std::endl;
	//std::cout << "Email: " << email << std::endl;



	std::ostringstream hashedPasswordStream;

	if (result == ARGON2_OK) {

		//cout << "PasswordHash: ";
		for (uint8_t i : hash) {
			//printf("%02x", i);
			hashedPasswordStream << std::hex << std::setw(2) << std::setfill('0') << (int)i;

		}
		cout << endl;

		string hashedPassword = hashedPasswordStream.str();

		DataBase db;
		db.ConnectBase();
		db.SaveToDataBaseInfo(username, email, hashedPassword, salt, t_cost, m_cost, parallelism , connection_);

		std::string response = "Registration succesfull\n";

        txn.commit();

		connection_->send_message(response);

        response.clear();
       

	}



	else {
		cerr << "Error Argon2" << argon2_error_message(result) << endl;
	}

	

	
	

}

bool AuthService::Login()
{
    
    

    try
    {

        cout << "ResponseChoice:" << responseChoice << endl;
        cout << "Username: " << username << endl;
       
        DataBase db;
        
        db.ConnectBase();

        pqxx::result result = txn.exec(
            pqxx::zview("SELECT id , password_hash, salt, t_cost, m_cost, parallelism, role FROM users WHERE username = $1"),
            pqxx::params{ username }
        );


        cout << "Loading" << endl;
       

        if (strlen(password.c_str()) == 0 && strlen(username.c_str()) == 0) {
            std::cout << "Please username and password: " << std::endl;

            string response = "username and password empty";
            connection_->send_message(response);

            return false;
        }
        
       
        // Проверка результата
        if (result.empty())
        {
            cerr << "User not found: " << username << endl;
            string response = "User not found";
            connection_->send_message(response);
            return false;
        }

        // Получение данных пользователя
        int userID = result[0]["id"].as<int>();
        std::string stored_hash = result[0]["password_hash"].c_str();
       // cout << "Stored_hash:" << stored_hash << endl;
        std::string stored_salt = result[0]["salt"].c_str();
        uint32_t t_cost = result[0]["t_cost"].as<uint32_t>();
        uint32_t m_cost = result[0]["m_cost"].as<uint32_t>();
        uint32_t parallelism = result[0]["parallelism"].as<uint32_t>();
        std::string role = result[0]["role"].as<string>();
        std::cout << "Role:" << role << std::endl;

        std::cout << "UserID:" << userID << std::endl;
       
        std::string ID = to_string(userID);

        // Преобразование соли из HEX в байты
        std::vector<uint8_t> salt_bytes;
        for (size_t i = 0; i < stored_salt.length(); i += 2)
        {
            salt_bytes.push_back(std::stoi(stored_salt.substr(i, 2), nullptr, 16));
        }


      

       
        
      

        int result_code = argon2id_hash_raw(t_cost, m_cost, parallelism,
            password.c_str(), strlen(password.c_str()),
            salt_bytes.data(), salt_bytes.size(),
            hash, hashlen);


        std::ostringstream saltStream;
        for (uint8_t i : salt_bytes) {
            saltStream << std::hex << std::setw(2) << std::setfill('0') << (int)i;
        }
        std::string saltHex = saltStream.str();

        //cout << "SaltHex:" << saltHex << endl;

        if (result_code != ARGON2_OK)
        {
            cerr << "Argon2 error: " << argon2_error_message(result_code) << endl;
            return false;
        }

        // Преобразование хеша в строку (HEX)
        std::ostringstream new_hashed_stream;
        for (uint8_t byte : hash)
        {
            new_hashed_stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        std::string new_hashed_password = new_hashed_stream.str();

        //cout << "newHash:" << new_hashed_password << endl;

       

        if (new_hashed_password == stored_hash) {
            cout << "Successfull\n";
        }
        else {

            cout << "Incorrect password\n";
            string response = "incorrect password";
            connection_->send_message(response);
        }

       
        string response = "Login Successfull";

        string message = response + "|" + role + "|" + ID;
        connection_->send_message(message);


       

       
        // Сравнение хешей
        return (new_hashed_password == stored_hash);

      
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database error: " << e.what() << std::endl;
        return false;
    }
}

void AuthService::generate_salt(uint8_t* salt, size_t length)
{
	if (!salt) {
		throw std::invalid_argument("Salt pointer is null");
	}

	if (!RAND_bytes(salt, salt_length)) {
		cout << "Error rand salt";
		throw std::runtime_error("Error generating random salt");
	}
}

