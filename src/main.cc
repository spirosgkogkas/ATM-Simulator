#include "..\Lib\atm.hpp"

int main(int argc, char const *argv[])
{
	int choice;
	std::string token;
	atmdecl::ATM atm;
	do{
		atm.printMenu();
		std::cout << "Δώσε επιλογή: "; std::cin >> choice;
		std::cin.get();
		switch(choice){
			case 0:
				break;
			case 1:
				std::cout << "Δώσε κωδικό λογαριασμού: "; std::cin >> token;
				std::cin.get();
				if(!atm.setUser(token)){
					std::cout << "Λάθος κωδικός. . .\n";
					break;
				}
				break;
			case 2:
				atm.createAccount();
				break;
			default:
				std::cout << "Λάθος επιλογή. . .\n";
		}
	}while(1);
	return 0;
}