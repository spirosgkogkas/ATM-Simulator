#include <iostream>
#include <fstream>
#include <iomanip>

class User{
		public:
			User();
			~User();
			printMenu(); // -----------------------------
			User(const User &obj);
			void setAcc(std::string acc);
			void setPassword(std::string pw);
			void setName(std::string name);
			void setBalance(std::string balance);
			void getAll() const;
		private:
			std::string name;
			std::string password;
			std::string accountNumber;
			double balance;
};

bool getAccountNumber(std::string token){
	std::string temp;
	std::fstream fp;
	User currentUser;

	fp.open("test.txt", std::ios::in);
	if(!fp.is_open())
		std::cerr << "File not found . . ." << std::endl;
	do{
		getline(fp, temp, ' ');
		if(temp == token){
			currentUser.setAcc(token);
			getline(fp, temp, ' ');
			currentUser.setPassword(temp);
			getline(fp, temp, ' ');
			currentUser.setName(temp);
			getline(fp, temp, ' ');
			currentUser.setBalance(temp);
			currentUser.getAll();
			return true;
		}
	}while(!fp.eof());
	return false;
}

int main(int argc, char const *argv[]){
	User p1;
	// std::fstream fp;
	// std::string a;
	// fp.open("test.txt", std::ios::in);
	// if(!fp.is_open())
	// 	std::cerr << "File not found . . ." << std::endl;
	// while(!fp.eof()){
	// 	getline(fp, a, ' ');
	// }
	std::cout << getAccountNumber("GR12345678911111");
	return EXIT_SUCCESS;
}

void User::setAcc(std::string acc){this->accountNumber = acc;}
void User::setPassword(std::string pw){this->password = pw;}
void User::setName(std::string name){this->name = name;}
void User::setBalance(std::string balance){this->balance = std::stod(balance.c_str());}
void User::getAll() const{
	std::cout << this->name << ' ' << this->password << ' ' << this->accountNumber << ' ' << this->balance << std::endl;
}

User::User(){

}
User::~User(){

}