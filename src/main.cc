#include "../Lib/atm.hpp"

int main(int argc, char const *argv[])
{
	int choice, choice2 = 1;
	bool withdawalState = false;
	double value;
	
	std::string token, pin;
	atmdecl::ATM atm;
	do{
		atm.clearScreen();
		atm.atmMenu();
		std::cout << "Enter Choice: "; std::cin >> choice;
		std::cin.get();
		switch(choice){
			case 0:
				choice2 = 0;
				break;
			case 1:
				while(true){
					atm.clearScreen();
					std::cout << "Please enter your Bank-ID(Enter Q or q to quit): "; std::cin >> token;
					std::cin.get();
					atm.clearScreen();
					if(token == "q" || token == "Q") break;
					if(!atm.setUser(token)){
						std::cout << "Invalid Bank-ID. . .\n";
						std::cin.get();
						atm.clearScreen();
					}
					else break;
				}
				if(token == "q" || token == "Q") break;
				do{
					std::cout << "Enter PIN: "; std::cin >> pin;
					std::cin.get();
					if(!atm.validatePassword(pin)){
						std::cout << "Invalid PIN. . .\n";
						atm.setTries(atm.getTries() - 1);
						atm.getTries() > 0 ? std::cout << "Tries left " << atm.getTries() << std::endl \
						: std::cout << "Your account got suspended. . ." << std::endl;
						std::cout << "Press enter to continue. . .";
						std::cin.ignore();
						atm.clearScreen();
					}
					else{
						atm.clearScreen();
						atm.setTries(3);
						std::cout << "Successfully connected.\n";
						break;
					}
					if(!atm.getTries()){
						std::cout << "Exit program. . ." << std::endl;
						exit(0);
					}
				}while(atm.getTries());
				int userAction;
				do{
					atm.userMenu();
					std::cout << "Enter choice: "; std::cin >> userAction;
					std::cin.get();
					atm.clearScreen();
					switch(userAction){
						case 1:
							std::cout << "How much would you like to deposit (0 to cancel)?: "; std::cin >> value;
							std::cin.get();
							atm.clearScreen();
							if(!value) break;
							if(atm.deposit(value))
								std::cout << "Deposit completed successfully.\n";
							else std::cout << "Your balance is insufficient to make the deposit.\n";
							atm.updateUser();
							break;
						case 2:
							do{
								std::cout << "Balance: " << std::setprecision(2) << atm.getUserBalance() << '\n';
								std::cout << "Enter withdrawal amount (0 to cancel): "; std::cin >> value;
								std::cin.get();
								atm.clearScreen();
								if(!value) break;
								if(atm.withdrawal(value)){
									std::cout << "Withdrawal completed successfully.\n";
									withdawalState = true;
								}
								else std::cout << "The amount of your withdrawal is greater than the balance of your account.\nTry again...\n";
								atm.updateUser();
							}while(!withdawalState);
							break;
						case 3:
							if(atm.transfer()){
								std::cout << "Successfully transfer." << std::endl;
								atm.updateUser();
							}
							std::cout << "Please press enter to continue. . .";
							std::cin.ignore();
							atm.clearScreen();
							break;
						case 4:
							atm.transactions();
							std::cout << "Please press enter to continue. . .";
							std::cin.ignore();
							atm.clearScreen();
							break;
						case 5:
							atm.userInfo();
							std::cout << "Please press enter to continue. . .";
							std::cin.ignore();
							atm.clearScreen();
							break;
						case 6:
							if(atm.changePassword())
								std::cout << "Pin successfully changed." << std::endl;
							else
								std::cout << "There was an error while changing your PIN. . ." << std::endl;
							break;
					}
				}while(userAction != 7);
				atm.clearScreen();
				break;
			case 2:
				atm.clearScreen();
				atm.createAccount();
				std::cin.get();
				std::cout << "Press enter to continue. . .\n";
				atm.clearScreen();
				break;
			default:
				std::cout << "Invalid choice. . .\n";
				std::cin.get();
				atm.clearScreen();
		}
	}while(choice2);
	return 0;
}