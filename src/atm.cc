#include "..\Lib\atm.hpp"

atmdecl::ATM::ATM(){
	/*Open bank.dat file to read mode */
	system("chcp 65001 > nul"); //Utf-8 encoding for console output
	this->tries = 3;
	this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
	this->LOG_FP.open(LOG_FILE, std::ios::app);
}

atmdecl::ATM::~ATM(){
	/*Close files at the termination of the program*/
	if(this->membersDatFile.is_open())
		this->membersDatFile.close();
	this->LOG_FP.close();
}

atmdecl::User::User(){

}

atmdecl::User::~User(){
	
}

void atmdecl::ATM::atmMenu(){
	/*Menu for the start of the program*/
	std::string upEdge = "\u250F";
	std::string line = "\u2501";
	std::string upEdge2 = "\u2513";
	std::string side = "\u2503";
	std::string downEdge = "\u2517";
	std::string downEdge2 = "\u251B";

	std::cout << std::setw(98)<< "ATM" << '\n';
	std::cout << std::setw(80) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << '\n';
	std::cout << std::setw(80) << side << "[0]:Έξοδος" << std::setw(31) << side << '\n';
	std::cout << std::setw(80) << side << "[1]:Δώσε Κωδικό" << std::setw(26) << side << '\n';
	std::cout << std::setw(80) << side << "[2]:Δημιουργία Λογαριασμού" << std::setw(15) << side << '\n';
	std::cout << std::setw(80) << downEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << downEdge2 << std::endl;
}

bool atmdecl::ATM::setUser(){
	std::string token, tempToken, tempPin, tempName, tempBalance; // Temporary variables
	if(!this->membersDatFile.is_open()){
		std::cerr << "Πρόβλημα κατά την εκτέλεση. . .\n";
		return false;
	}
	std::cout << "Παρακαλώ πληκτρολογήστε τον κωδικό λογαριασμού: "; std::cin >> token;
	std::cin.get();
	do{
		/*
			First getline stores the token of an existing user.
			The the condition inside the if checks if User's token is equal with the fetched token.
			if it's equal we store the info of the current user to our user obj (currentUser) and we return true.
			else we return false.
		*/
		getline(this->membersDatFile, tempToken, ' ');
		getline(this->membersDatFile, tempPin, ' ');
		getline(this->membersDatFile, tempName, ' ');
		getline(this->membersDatFile, tempBalance);
		if(tempToken == token){
			this->currentUser.setToken(token);
			this->currentUser.setPassword(tempPin);
			this->currentUser.setName(tempName);
			this->currentUser.setBalance(tempBalance);
			this->membersDatFile.seekg(0, std::ios::beg);
			return true;
		}
	}while(this->membersDatFile.peek() != EOF);
	this->membersDatFile.clear();
	this->membersDatFile.seekg(0, std::ios::beg);
	return false;
}

bool atmdecl::ATM::createAccount(){
	std::string temp;
	if(this->membersDatFile.is_open()){
		std::cout << "CLOSED FILE" << std::endl;
		this->membersDatFile.close();
	}
	this->membersDatFile.open(BANK_DAT_FILE, std::ios::app);
	if(!this->membersDatFile.is_open()){
		std::cerr << "Πρόβλημα κατα την δημιουργία λογαριασμού. . .\n";
		return false;
	}
	std::cout << "Δώσε αριθμό λογαριασμού: "; std::cin >> temp;
	this->currentUser.setToken(temp);
	this->membersDatFile << temp << ' ';
	std::cin.get();
	std::cout << "Δώσε PIN: "; std::cin >> temp;
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
	std::cout << "Ο λογαριασμός δημιουργήθηκε με επιτυχία.\n";
	this->membersDatFile.close();
	this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
	return true;
}

void atmdecl::ATM::userMenu(){
	/*Menu for the start of the program*/
	std::string upEdge = "\u250F";
	std::string line = "\u2501";
	std::string upEdge2 = "\u2513";
	std::string side = "\u2503";
	std::string downEdge = "\u2517";
	std::string downEdge2 = "\u251B";

	std::cout << std::setw(109)<< "Menu χρήστη" << '\n';
	std::cout << std::setw(80) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << '\n';
	std::cout << std::setw(80) << side << "[1]:Κατάθεση μετρητών" << std::setw(20) << side << '\n';
	std::cout << std::setw(80) << side << "[2]:Ανάληψη μετρητών" << std::setw(21) << side << '\n';
	std::cout << std::setw(80) << side << "[3]:Μεταφορά" << std::setw(29) << side << '\n';
	std::cout << std::setw(80) << side << "[4]:Τελευταίες συναλλαγές" << std::setw(16) << side << '\n';
	std::cout << std::setw(80) << side << "[5]:Πληροφορίες λογαριασμού" << std::setw(14) << side << '\n';
	std::cout << std::setw(80) << side << "[6]:Αλλαγή κωδικού" << std::setw(23) << side << '\n';
	std::cout << std::setw(80) << side << "[7]:Έξοδος" << std::setw(31) << side << '\n';
	std::cout << std::setw(80) << downEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << downEdge2 << std::endl;
}

bool atmdecl::ATM::transfer(){
	double value;
	bool found = false;
	std::string transferToken, temp;
	std::ifstream rfp(BANK_DAT_FILE, std::ios::in);
	std::ofstream wfp("temp.txt", std::ios::out);

	if(!rfp.is_open()){
		std::cerr << "Πρόβλημα κατά την ανάγνωση δεδομένων. . .\n";
		return false;
	}
	std::cout << "Παρακαλώ πληκτρολογήστε τον κωδικό λογαριασμού μεταφοράς: "; std::cin >> transferToken;
	std::cin.get();
	do{
		std::cout << "Υπόλοιπο: " << this->currentUser.getBalance() << '\n';
		std::cout << "Παρακαλώ πληκτρολογήστε ποσό μεταφοράς: "; std::cin >> value;
		std::cin.get();
		if(value > this->currentUser.getBalance())
			std::cerr << "Μη αποδεκτό ποσό μεταφοράς. . .\n";
	}while(value > this->currentUser.getBalance());

	while(rfp.peek() != EOF){
		getline(rfp, temp, ' ');
		wfp << temp << ' ';
		if(temp == transferToken){
			found = true;
			getline(rfp, temp, ' ');
			wfp << temp << ' ';
			getline(rfp, temp, ' ');
			wfp << temp << ' ';
			getline(rfp, temp);
			wfp << std::fixed << std::setprecision(2) << (std::stod(temp) + value) << '\n';
			this->currentUser.setBalance(this->currentUser.getBalance() - value);
			this->LOG_FP << this->currentUser.getToken() << " Μεταφορά στον λογαριασμό: " << transferToken << " το ποσό -> " << value << std::endl;
		}
		else{
			getline(rfp, temp, ' ');
			wfp << temp << ' ';
			getline(rfp, temp, ' ');
			wfp << temp << ' ';
			getline(rfp, temp);
			wfp << temp << '\n';
		}
	}
	rfp.close();
	wfp.close();
	rfp.open("temp.txt", std::ios::in);
	wfp.open(BANK_DAT_FILE, std::ios::out);
	while(rfp.peek() != EOF){
		getline(rfp, temp);
		wfp << temp << '\n';
	}
	rfp.close();
	wfp.close();
	return found;
}

void atmdecl::ATM::updateUser(){
	std::string line;
	std::ifstream rfp(BANK_DAT_FILE, std::ios::in);
	std::ofstream wfp("temp.dat", std::ios::out);
	if(!rfp.is_open()){
		std::cerr << "Πρόβλημα ανόιγματος αρχείου. . .\n";
		return;
	}
	if(!wfp.is_open()){
		std::cerr << "Πρόβλημα ανόιγματος αρχείου. . .\n";
		return;
	}
	while(rfp.peek() != EOF){
		getline(rfp, line, ' ');
		wfp << line << ' ';
		if(line == this->currentUser.getToken()){
			getline(rfp, line, ' ');
			wfp << this->currentUser.getPassword() << ' ';
			getline(rfp, line, ' ');
			wfp << line << ' ';
			getline(rfp, line);
			wfp << this->currentUser.getBalance() << '\n';
		}
		else{
			getline(rfp, line, ' ');
			wfp << line << ' ';
			getline(rfp, line, ' ');
			wfp << line << ' ';
			getline(rfp, line);
			wfp << line << '\n';
		}
	}

	rfp.close();
	wfp.close();
	rfp.open("temp.dat", std::ios::in);
	wfp.open(BANK_DAT_FILE, std::ios::out);
	if(!rfp.is_open()){
		std::cerr << "Πρόβλημα ανόιγματος αρχείου. . .\n";
		return;
	}
	if(!wfp.is_open()){
		std::cerr << "Πρόβλημα ανόιγματος αρχείου. . .\n";
		return;
	}
	while(rfp.peek() != EOF){
		getline(rfp, line);
		wfp << line << '\n';
	}
	rfp.close();
	wfp.close();
}

void atmdecl::ATM::transactions(){
	unsigned short count = 5;
	std::string line, token = this->currentUser.getToken();
	std::vector<std::string> lines;
	std::ifstream rfp(LOG_FILE, std::ios::out);
	
	if(!rfp.is_open()){
		std::cerr << "Δεν βρέθηκαν συναλλαγές. . .\n";
		return;
	}

	while(rfp.peek() != EOF){
		getline(rfp, line);
		if(token == line.substr(0, line.find(' ')))
			lines.push_back(line);
	}
	rfp.close();
	std::reverse(lines.begin(), lines.end());
	for(size_t i = 0; i < count && i < lines.size(); i++)
		std::cout << "Συναλλαγή: " << lines[i] << std::endl;
}

void atmdecl::ATM::userInfo(){
		/*Menu for the start of the program*/
	std::string upEdge = "\u250F";
	std::string line = "\u2501";
	std::string upEdge2 = "\u2513";
	std::string side = "\u2503";
	std::string downEdge = "\u2517";
	std::string downEdge2 = "\u251B";
	std::string name = this->currentUser.getName();

	std::cout << std::setw(120)<< "Πληροφορίες χρήστη" << '\n';
	std::cout << std::setw(80) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << '\n';
	std::cout << std::setw(80) << side << "[Ονομα χρήστη]: " << std::setw(22) << name << side << '\n';
	std::cout << std::setw(80) << side << "[Υπόλοιπο    ]: " << std::setw(22) << this->currentUser.getBalance() << side << '\n';
	std::cout << std::setw(80) << downEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << downEdge2 << std::endl;
}