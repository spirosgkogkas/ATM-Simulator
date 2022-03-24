#include "..\Lib\atm.hpp"

atmdecl::ATM::ATM(){
	/*Open bank.dat file to read mode */
	system("chcp 65001 > nul"); //Utf-8 encoding for console output
	this->tries = 3;
	// this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
}

atmdecl::ATM::~ATM(){
	/*Close file at the termination of the program*/
	if(this->membersDatFile.is_open())
		this->membersDatFile.close();
}

atmdecl::User::User(){

}

atmdecl::User::~User(){
	
}

void atmdecl::ATM::printMenu(){
	/*Menu for the start of the program*/
	std::string upEdge = "\u250F";
	std::string line = "\u2501";
	std::string upEdge2 = "\u2513";
	std::string side = "\u2503";
	std::string downEdge = "\u2517";
	std::string downEdge2 = "\u251B";

	std::cout << std::setw(100)<< "ATM" << std::endl;
	std::cout << std::setw(80) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << '\n';
	std::cout << std::setw(80) << side << "[0]:Έξοδος" << std::setw(31) << side << std::endl;
	std::cout << std::setw(80) << side << "[1]:Δώσε Κωδικό" << std::setw(26) << side << std::endl;
	std::cout << std::setw(80) << side << "[2]:Δημιουργία Λογαριασμού" << std::setw(15) << side << std::endl;
	std::cout << std::setw(80) << downEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << downEdge2 << std::endl;
}

bool atmdecl::ATM::setUser(std::string token){
	std::string temp; // Temporary variable for comparison
	if(this->membersDatFile.is_open()){
		this->membersDatFile.close();
		this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
	}
	else this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
	if(!this->membersDatFile.is_open()){
		std::cerr << "Προβλημα κατά την επεξεργασία δεδομένων. . .\n";
		return false;
	}

	do{
		/*
			First getline stores the token of an existing user.
			The the condition inside the if checks if User's token is equal with the fetched token.
			if it's equal we store the info of the current user to our user obj (currentUser) and we return true.
			else we return false.
		*/
		getline(this->membersDatFile, temp, ' ');
		std::cout << "POSITION: " << this->membersDatFile.tellg() << '\n';
		if(temp == token){
			this->currentUser.setToken(token);
			getline(this->membersDatFile, temp, ' ');
			this->currentUser.setPassword(temp);
			getline(this->membersDatFile, temp, ' ');
			this->currentUser.setName(temp);
			getline(this->membersDatFile, temp, ' ');
			this->currentUser.setBalance(temp);
			this->currentUser.printALL();
			std::cout << "SUCCESS";
			this->membersDatFile.seekg(0);
			return true;
		}
	}while(!this->membersDatFile.eof());
	this->membersDatFile.clear();
	this->membersDatFile.seekg(0);
	return false;
}

bool atmdecl::ATM::createAccount(){
	std::string temp;
	if(this->membersDatFile.is_open())
		this->membersDatFile.close();
	this->membersDatFile.open(BANK_DAT_FILE, std::ios::app);
	if(!this->membersDatFile.is_open()){
		std::cerr << "Πρόβλημα κατα την δημιουργία λογαριασμού. . .\n";
		return false;
	}
	std::cout << "Δώσε αριθμό λογαριασμού: "; std::cin >> temp;
	this->currentUser.setToken(temp);
	this->membersDatFile << temp << ' ';
	std::cin.get();
	std::cout << "Δώσε Δώσε PIN: "; std::cin >> temp;
	this->currentUser.setName(temp);
	this->membersDatFile << temp << ' ';
	std::cin.get();
	std::cout << "Δώσε ονομα: "; std::cin >> temp;
	this->currentUser.setName(temp);
	this->membersDatFile << temp << ' ';
	std::cin.get();
	std::cout << "Δώσε ποσό: "; std::cin >> temp;
	this->currentUser.setBalance(temp);
	std::cin.get();
	this->membersDatFile << temp << '\n';
	std::cout << "SUCCESS";
	return true;
}