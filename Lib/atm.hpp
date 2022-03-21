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
			User(const User &obj);
		private:
			char *holderName;
			char password[4];
			char accountNumber[16];
			long long int balance;
		};

		class ATM{
		public:
			ATM();
			~ATM();
			void setUser();// ---------------------------
			bool createAccount();
			void printMenu();
		private:
			atmdecl::User currentUser;
			std::fstream membersDatFile;
		};

		
	}
#endif