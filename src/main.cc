#include "..\Lib\atm.hpp"

int main(int argc, char const *argv[])
{
	int choice, choice2 = 1;
	double value;
	std::string token, pin;
	atmdecl::ATM atm;
	do{
		atm.atmMenu();
		std::cout << "Δώσε επιλογή: "; std::cin >> choice;
		std::cin.get();
		switch(choice){
			case 0:
				choice2 = 0;
				break;
			case 1:
				bool signIn;
				while(!(signIn = atm.setUser())){
					std::cout << "Λάθος κωδικός. . .\n";
				}
				do{
					std::cout << "Δώσε PIN: "; std::cin >> pin;
					std::cin.get();
					if(!atm.validatePassword(pin)){
						std::cout << "Λάθος PIN. . .\n";
						atm.setTries();
						atm.getTries() > 0 ? std::cout << "Απομένουν " << atm.getTries() << " προσπάθειες" << std::endl \
						: std::cout << "Κλείδωμα λογαριασμού. . ." << std::endl;
						atm.clearScreen();
					}
					else{
						std::cout << "Επιτυχής σύνδεση.\n";
						break;
					}
					if(!atm.getTries()){
						std::cout << "Εξοδος προγράμματος. . ." << std::endl;
						exit(0);
					}
				}while(atm.getTries());
				int userAction;
				do{
					atm.userMenu();
					std::cout << "Δώσε επιλογή: "; std::cin >> userAction;
					std::cin.get();
					switch(userAction){
						case 1:
							std::cout << "Δώσε ποσό κατάθεσης: "; std::cin >> value;
							std::cin.get();
							if(atm.deposit(value))
								std::cout << "Η κατάθεση ολοκληρώθηκε με επιτυχία.\n";
							else std::cout << "Μη αποδεκτή κατάθεση. . .\n";
							atm.updateUser();
							break;
						case 2:
							std::cout << "Δώσε ποσό ανάληψης: "; std::cin >> value;
							std::cin.get();
							if(atm.withdrawal(value))
								std::cout << "Η ανάληψη ολοκληρώθηκε με επιτυχία.\n";
							else std::cout << "Μη αποδεκτή ανάληψη. . .\n";
							atm.updateUser();
							break;
						case 3:
							atm.transfer();
							atm.updateUser();
							break;
						case 4:
							atm.transactions();
							break;
						case 5:
							atm.userInfo();
							break;
					}
				}while(userAction != 7);
					break;
			case 2:
				atm.createAccount();
				break;
			default:
				std::cout << "Λάθος επιλογή. . .\n";
		}
	}while(choice2);
	return 0;
}