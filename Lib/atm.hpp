/*atm.hpp*/
/*declarations*/

/*Guard Macro*/
#ifndef ATM_DECL
	#include <iostream>
	#include <fstream>
	#include <iomanip>
	#define ATM_DECL
	#define BANK_DAT_FILE "bank.dat"
	namespace atmdecl{
		class User{
		public:
			User();
			~User();
			printMenu(); // -----------------------------
			std::string getPassword()const{return this->password;}
		private:
			std::string name;
			std::string password;
			std::string accountNumber;
			long long int balance;
		};

		class ATM{
		public:
			ATM();
			~ATM();
			bool validatePassword(std::string pw)const{return this->currentUser.getPassword() == pw;}
			bool checkAccountNumber();
			void setUser();//---------------------------
			bool createAccount();
			void printMenu();
		private:
			atmdecl::User currentUser;
			std::fstream membersDatFile;
		};	
	}
#endif