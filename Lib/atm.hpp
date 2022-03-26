/*atm.hpp*/
/*declarations*/

/*Guard Macro*/
#ifndef ATM_DECL
	#include <iostream>
	#include <fstream>
	#include <iomanip>
	#define ATM_DECL
	#define BANK_DAT_FILE "..\\test.txt"
	namespace atmdecl{
		class User{
		public:
			User();
			~User();
			std::string getPassword()const{return this->password;}
			void setToken(std::string token){this->accountNumber = token;}
			void setName(std::string name){this->name = name;}
			void setPassword(std::string pw){this->password = pw;}
			void setBalance(std::string balance){this->balance = std::stod(balance);}
			void setBalance(double balance){this->balance = balance;}
			double getBalance()const{return this->balance;}
			std::string getToken()const{return this->accountNumber;}
		private:
			std::string name;
			std::string password;
			std::string accountNumber;
			double balance;
		};

		class ATM{
		public:
			ATM();
			~ATM();
			void setTries(){this->tries ? this->tries-- : 0;};
			unsigned short int getTries()const{return this->tries;}
			bool validatePassword(std::string pw)const{return this->currentUser.getPassword() == pw;}
			bool setUser();
			bool createAccount();
			void atmMenu();
			void userMenu();
			bool transfer();
			void updateUser();
			void clearScreen(){
				#ifdef _WIN32
					system("pause && cls");
				#endif
			}
			bool deposit(double value){
				if(value > 0){
					this->currentUser.setBalance(this->currentUser.getBalance() + value);
					return true;
				}return false;}
			bool withdrawal(double value){
				if((this->currentUser.getBalance() - value) < 0)
					return false;
				this->currentUser.setBalance(this->currentUser.getBalance() - value);
				return true;
			}
		private:
			unsigned short int tries;
			atmdecl::User currentUser;
			std::fstream membersDatFile;
		};	
	}
#endif