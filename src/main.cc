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
				while(true){
					std::cout << "Παρακαλώ πληκτρολογήστε τον κωδικό λογαριασμού (Πάτησε Q/q για έξοδο): "; std::cin >> token;
					std::cin.get();
					if(token == "q" || token == "Q") break;
					if(!atm.setUser(token))
						std::cout << "Μη αποδεκτός κωδικός λογαριασμού. . .\n";
					else break;
				}
				if(token == "q" || token == "Q") break;
				do{
					std::cout << "Δώσε PIN: "; std::cin >> pin;
					std::cin.get();
					if(!atm.validatePassword(pin)){
						std::cout << "Λάθος PIN. . .\n";
						atm.setTries(atm.getTries() - 1);
						atm.getTries() > 0 ? std::cout << "Απομένουν " << atm.getTries() << " προσπάθειες" << std::endl \
						: std::cout << "Κλείδωμα λογαριασμού. . ." << std::endl;
						std::cout << "Πάτησε enter για να συνεχίσεις. . .";
						std::cin.ignore();
						atm.clearScreen();
					}
					else{
						atm.clearScreen();
						atm.setTries(3);
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
					atm.clearScreen();
					switch(userAction){
						case 1:
							std::cout << "Δώσε ποσό κατάθεσης (0 για ακύρωση): "; std::cin >> value;
							std::cin.get();
							atm.clearScreen();
							if(!value) break;
							if(atm.deposit(value))
								std::cout << "Η κατάθεση ολοκληρώθηκε με επιτυχία.\n";
							else std::cout << "Μη αποδεκτή κατάθεση. . .\n";
							atm.updateUser();
							break;
						case 2:
							do{
								std::cout << "Δώσε ποσό ανάληψης (0 για ακύρωση): "; std::cin >> value;
								std::cin.get();
								atm.clearScreen();
								if(!value) break;
								if(atm.withdrawal(value))
									std::cout << "Η ανάληψη ολοκληρώθηκε με επιτυχία.\n";
								else std::cout << "Μη αποδεκτή ανάληψη. . .\n";
								atm.updateUser();
							}while(!atm.withdrawal(value));
							break;
						case 3:
							if(atm.transfer()){
								std::cout << "Επιτυχής μεταφορά." << std::endl;
								atm.updateUser();
							}
							else
								std::cout << "Η μεταφορά δεν ολοκληρώθηκε. . ." << std::endl;
							std::cout << "Πάτησε enter για να συνεχίσεις. . .";
							std::cin.ignore();
							atm.clearScreen();
							break;
						case 4:
							atm.transactions();
							std::cout << "Πάτησε enter για να συνεχίσεις. . .";
							std::cin.ignore();
							atm.clearScreen();
							break;
						case 5:
							atm.userInfo();
							std::cout << "Πάτησε enter για να συνεχίσεις. . .";
							std::cin.ignore();
							atm.clearScreen();
							break;
						case 6:
							atm.changePassword();
							break;
					}
				}while(userAction != 7);

				atm.clearScreen();
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