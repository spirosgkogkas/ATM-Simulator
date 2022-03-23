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
			double balance;
		};

		class ATM{
		public:
			ATM();
			~ATM();
			void setTries(){this->tries > 0 ? this->tries-- : std::cerr << "0 προσπάθειες. . ." std::endl;};
			unsigned short int getTries()const{return this->tries;}
			bool validatePassword(std::string pw)const{return this->currentUser.getPassword() == pw;}
			bool setUser(std::string token);
			bool createAccount();
			void printMenu();
		private:
			unsigned short int tries;
			atmdecl::User currentUser;
			std::fstream membersDatFile;
		};	
	}
#endif