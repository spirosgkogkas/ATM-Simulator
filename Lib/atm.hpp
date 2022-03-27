/*atm.hpp*/
/*declarations*/

/*Guard Macro*/
#ifndef ATM_DECL
	#include <iostream>
	#include <cstdio>
	#include <fstream>
	#include <iomanip>
	#include <string>
	#include <algorithm>
	#include <vector>
	#define ATM_DECL
	#define BANK_DAT_FILE "..\\bank.dat"
	#define LOG_FILE "..\\log.dat"
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
			std::string getName()const{return this->name;}
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
			void setTries(int value){this->tries = value;};
			unsigned short int getTries()const{return this->tries;}
			bool validatePassword(std::string pw)const{return this->currentUser.getPassword() == pw;}
			bool setUser(std::string token);
			bool createAccount();
			void atmMenu();
			void userMenu();
			bool transfer();
			void updateUser();
			void transactions();
			void userInfo();
			bool changePassword();
			void clearScreen(){
				#ifdef _WIN32
					system("cls");
				#endif
			}
			bool deposit(double value){
				if(value > 0){
					this->currentUser.setBalance(this->currentUser.getBalance() + value);
					this->LOG_FP << this->currentUser.getToken() << " Κατάθεση ποσού: " << value << std::endl;
					return true;
				}return false;}
			bool withdrawal(double value){
				if((this->currentUser.getBalance() - value) < 0)
					return false;
				this->currentUser.setBalance(this->currentUser.getBalance() - value);
				this->LOG_FP << this->currentUser.getToken() << " Ανάληψη ποσού: " << value << std::endl;
				return true;
			}
		private:
			unsigned short int tries;
			atmdecl::User currentUser;
			std::fstream membersDatFile;
			std::fstream LOG_FP;
		};	
	}
#endif