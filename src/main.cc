#include "..\Lib\atm.hpp"

int main(int argc, char const *argv[])
{
	int choice;
	std::string token;
	atmdecl::ATM atm;
	do{
		atm.printMenu();
		std::cout << "Δώσε επιλογή: "; std::cin >> choice;
		switch(choice){
			case 0:
				break;
			case 1:
				std::cout << "Δώσε κωδικό λογαριασμού: "; std::cin >> token;
				if(!atm.setUser(token)){
					std::cout << "Λάθος κωδικός. . .\n";
					break;
				}
			case 2:
				break;
			default:
				std::cout << "Λάθος επιλογή. . .\n";
		}
		break;
	}while(1);
	return 0;
}