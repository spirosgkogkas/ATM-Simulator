#include "../Lib/atm.hpp"

atmdecl::ATM::ATM(){
	#ifdef _WIN32
		system("chcp 65001 > nul"); //Utf-8 encoding
	#endif
	this->tries = 3;
	/*Open bank.dat with read mode */
	this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
	/*if no file exists we make it*/
	if(!this->membersDatFile.is_open()){
		this->membersDatFile.open(BANK_DAT_FILE, std::ios::out);
		this->membersDatFile.close();
		this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
		if(!this->membersDatFile.is_open())
			std::cerr << "An error occurred at creation of the file. . ." << std::endl;
	}
	this->LOG_FP.open(LOG_FILE, std::ios::app);
	if(!this->LOG_FP.is_open())
		std::cerr << "An error ocurred while opening the file. . ." << std::endl;
}

atmdecl::ATM::~ATM(){
	/*Deconstructor closes files.*/
	if(this->membersDatFile.is_open())
		this->membersDatFile.close();
	this->LOG_FP.close();
}

atmdecl::User::User(){

}

atmdecl::User::~User(){
	
}

void atmdecl::ATM::atmMenu(){
	/*Menu*/
	std::string upEdge = "\u250F";
	std::string line = "\u2501";
	std::string upEdge2 = "\u2513";
	std::string side = "\u2503";
	std::string downEdge = "\u2517";
	std::string downEdge2 = "\u251B";

	#ifdef _WIN32
		HWND console = GetConsoleWindow();
		// Get the current window size.
		RECT size = {0};
		GetWindowRect(console, &size);
		// Set the new window size.
		size.right = size.left + 640;
		size.bottom = size.top + 480;
		MoveWindow(console, size.left, size.top, size.right - size.left, size.bottom - size.top, TRUE);
		DWORD style = GetWindowLong(console, GWL_STYLE);
	    style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	    SetWindowLong(console, GWL_STYLE, style);
	#else
	    struct winsize size;
	    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	    size.ws_col = targetWidth;
	    size.ws_row = targetHeight;
	    ioctl(STDOUT_FILENO, TIOCSWINSZ, &size);
	#endif
	    
	std::cout << std::setw(38)<< "ATM" << '\n';
	std::cout << std::setw(20) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << '\n';
	std::cout << std::setw(20) << side << "[0]:Exit" << std::setw(33) << side << '\n';
	std::cout << std::setw(20) << side << "[1]:Login" << std::setw(32) << side << '\n';
	std::cout << std::setw(20) << side << "[2]:Register" << std::setw(29) << side << '\n';
	std::cout << std::setw(20) << downEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << downEdge2 << std::endl;
}

bool atmdecl::ATM::setUser(std::string token){
	/*Προσωρινές μεταβλητές*/
	std::string tempToken, tempPin, tempName, tempBalance;
	if(!this->membersDatFile.is_open()){
		std::cerr << "An error occurred. . .\n";
		return false;
	}
	/*Check to see if the file has data.*/
	if(this->membersDatFile.peek() == EOF) return false;

	do{
		/*We store the line items in the temporary variables*/
		getline(this->membersDatFile, tempToken, ' ');
		getline(this->membersDatFile, tempPin, ' ');
		getline(this->membersDatFile, tempName, ' ');
		getline(this->membersDatFile, tempBalance);
		/*
			If the account code matches the one provided by the user then we store the user's details
			and we put the pointer at the beginning of the file returning the value true.
			Otherwise the process will run until we reach the end of the file.
		*/
		if(tempToken == token){
			this->currentUser.setToken(token);
			this->currentUser.setPassword(tempPin);
			this->currentUser.setName(tempName);
			this->currentUser.setBalance(tempBalance);
			this->membersDatFile.seekg(0, std::ios::beg);
			return true;
		}
	}while(this->membersDatFile.peek() != EOF);
	/*We clear the flags of the file and show again at the beginning.*/
	this->membersDatFile.clear();
	this->membersDatFile.seekg(0, std::ios::beg);
	return false;
}

bool atmdecl::ATM::createAccount(){
	std::string temp, temp2;

	/*Check for existing user*/
	std::cout << "Please give a Bank-ID: "; std::cin >> temp;
	if(!this->membersDatFile.is_open()) this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
	while(this->membersDatFile.peek() != EOF){
		getline(this->membersDatFile, temp2);
		if(temp == temp2.substr(0, temp2.find(' '))){
			std::cout << "Bank-ID already exists . ." << std::endl;
			std::cin.get();
			this->membersDatFile.close();
			return false;
		}
	}
	this->membersDatFile.close();

	/*--- Register new user ---*/
	this->membersDatFile.open(BANK_DAT_FILE, std::ios::app);
	if(!this->membersDatFile.is_open()){
		std::cerr << "An error occurred while creating new user. . .\n";
		return false;
	}
	
	this->currentUser.setToken(temp);
	this->membersDatFile << temp << ' ';
	std::cin.get();
	std::cout << "Enter PIN: "; std::cin >> temp;
	this->currentUser.setName(temp);
	this->membersDatFile << temp << ' ';
	std::cin.get();
	std::cout << "Enter your name: "; std::cin >> temp;
	this->currentUser.setName(temp);
	this->membersDatFile << temp << ' ';
	std::cin.get();
	std::cout << "Enter Balance: "; std::cin >> temp;
	this->currentUser.setBalance(temp);
	std::cin.get();
	this->membersDatFile << temp << '\n';
	std::cout << "Account created successfully.\n";
	this->membersDatFile.close();
	this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
	return true;
}

void atmdecl::ATM::userMenu(){
	/*Menu Χρήστη*/
	std::string upEdge = "\u250F";
	std::string line = "\u2501";
	std::string upEdge2 = "\u2513";
	std::string side = "\u2503";
	std::string downEdge = "\u2517";
	std::string downEdge2 = "\u251B";

	std::cout << std::setw(41)<< "User Menu" << '\n';
	std::cout << std::setw(20) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << '\n';
	std::cout << std::setw(20) << side << "[1]:Cash deposit" << std::setw(25) << side << '\n';
	std::cout << std::setw(20) << side << "[2]:Cash withdrawal" << std::setw(22) << side << '\n';
	std::cout << std::setw(20) << side << "[3]:Transfer" << std::setw(29) << side << '\n';
	std::cout << std::setw(20) << side << "[4]:Latest transactions" << std::setw(18) << side << '\n';
	std::cout << std::setw(20) << side << "[5]:Account information" << std::setw(18) << side << '\n';
	std::cout << std::setw(20) << side << "[6]:Change Password" << std::setw(22) << side << '\n';
	std::cout << std::setw(20) << side << "[7]:Exit" << std::setw(33) << side << '\n';
	std::cout << std::setw(20) << downEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << downEdge2 << std::endl;
}

bool atmdecl::ATM::transfer(){
	/*Transfer from one User to another*/
	std::string transferToken, temp;
	std::ifstream rfp(BANK_DAT_FILE, std::ios::in);
	std::ofstream wfp("temp.dat", std::ios::out);
	double value;
	bool found = false;
	/*We open 2 files, 1 for reading and 1 for our refreshed data*/
	if(!rfp.is_open()){
		std::cerr << "An error occurred while reading data. . .\n";
		return false;
	}
	std::cout << "Please enter the Bank-ID to transfer (Q/q to cancel): "; std::cin >> transferToken;
	std::cin.get();
	if(transferToken == "Q" || transferToken == "q") return false;
	else if(transferToken == this->currentUser.getToken()){
		std::cerr << "Invalid action. . ." << std::endl;
		return false;
	}

	do{
		std::cout << "Balance: " << this->currentUser.getBalance() << '\n';
		std::cout << "Please enter transfer amount (0 to cancel): "; std::cin >> value;
		std::cin.get();
		/*Cancel*/
		if(!value) return false;
		/*Check amount */
		if(value > this->currentUser.getBalance())
			std::cerr << "Invalid transfer amount. . .\n";
	}while(value > this->currentUser.getBalance());

	/*Read and write data*/
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
			this->LOG_FP << this->currentUser.getToken() << " Transfer to the Bank-ID: " << transferToken << " the amount -> " << value << std::endl;
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

	rfp.open("temp.dat", std::ios::in);
	if(!rfp.is_open()){
		std::cerr << "A problem occurred, contact the administrator. . .\n";
		exit(0);
	}

	wfp.open(BANK_DAT_FILE, std::ios::out);
	if(!wfp.is_open()){
		std::cerr << "A problem occurred, contact the administrator. . .\n";
		exit(0);
	}
	while(rfp.peek() != EOF){
		getline(rfp, temp);
		wfp << temp << '\n';
	}
	rfp.close();
	wfp.close();
	if(std::remove("temp.dat"))
		std::cerr << "An error occurred while deleting the file. . ." << '\n';
	return found;
}

void atmdecl::ATM::updateUser(){
	/*Update User*/
	std::string line;
	std::ifstream rfp(BANK_DAT_FILE, std::ios::in);
	std::ofstream wfp("temp.dat", std::ios::out);
	if(!rfp.is_open()){
		//Ensure that wfp will close if it got opened.
		wfp.close();
		std::cerr << "There was an error while opening the file. . .\n";
		return;
	}
	if(!wfp.is_open()){
		//Ensure that rfp will close if it got opened.
		rfp.close();
		std::cerr << "There was an error while opening the file. . .\n";
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
		std::cerr << "There was an error while opening the file. . .\n";
		return;
	}
	if(!wfp.is_open()){
		std::cerr << "There was an error while opening the file. . .\n";
		return;
	}
	while(rfp.peek() != EOF){
		getline(rfp, line);
		wfp << line << '\n';
	}
	rfp.close();
	wfp.close();
	/*Delete temporary file*/
	if(std::remove("temp.dat"))
		std::cerr << "An error occurred while deleting the file. . ." << '\n';
}

void atmdecl::ATM::transactions(){
	/*Συναλλαγές.*/
	unsigned short count = 5;
	std::string line, token = this->currentUser.getToken();
	std::vector<std::string> lines;
	std::ifstream rfp(LOG_FILE, std::ios::out);
	
	if(!rfp.is_open()){
		std::cerr << "No transactions found. . .\n";
		return;
	}

	while(rfp.peek() != EOF){
		getline(rfp, line);
		if(token == line.substr(0, line.find(' ')))
			lines.push_back(line);
	}
	rfp.close();
	if(lines.empty()){
		std::cout << "Bank-ID [" << token << "] does not have any transactions. . ." << std::endl;
		return;
	}
	std::reverse(lines.begin(), lines.end());
	for(size_t i = 0; i < count && i < lines.size(); i++)
		std::cout << "transaction: " << lines[i] << std::endl;
}

void atmdecl::ATM::userInfo(){
	/*User information.*/
	std::string upEdge = "\u250F";
	std::string line = "\u2501";
	std::string upEdge2 = "\u2513";
	std::string side = "\u2503";
	std::string downEdge = "\u2517";
	std::string downEdge2 = "\u251B";
	std::string bank_id = this->currentUser.getToken();

	std::cout << std::setw(45)<< "User information" << '\n';
	std::cout << std::setw(20) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << '\n';
	std::cout << std::setw(20) << side << "[Bank-ID]: " << std::setw(27) << bank_id << side << '\n';
	std::cout << std::setw(20) << side << "[Balance]: " << std::setw(27) << std::fixed << std::setprecision(2) << this->currentUser.getBalance() << side << '\n';
	std::cout << std::setw(20) << downEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << downEdge2 << std::endl;
}

bool atmdecl::ATM::changePassword(){
	/*Change Password.*/
	std::string line, password;
	std::cout << "Enter new PIN (Q/q to cancel): "; std::cin >> password;
	std::cin.get();
	if(password == "Q" || password == "q") return false;
	std::ifstream rfp(BANK_DAT_FILE, std::ios::in);
	std::ofstream wfp("temp.dat", std::ios::out);
	if(!rfp.is_open()){
		//Ensure that wfp will close if it got opened.
		wfp.close();
		std::cerr << "There was an error while opening the file. . .\n";
		return false;
	}
	if(!wfp.is_open()){
		//Ensure that rfp will close if it got opened.
		rfp.close();
		std::cerr << "There was an error while opening the file. . .\n";
		return false;
	}
	while(rfp.peek() != EOF){
		getline(rfp, line, ' ');
		wfp << line << ' ';
		if(line == this->currentUser.getToken()){
			getline(rfp, line, ' ');
			wfp << password << ' ';
			this->currentUser.setPassword(password);
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
		std::cerr << "There was an error while opening the file. . .\n";
		return false;
	}
	if(!wfp.is_open()){
		std::cerr << "There was an error while opening the file. . .\n";
		return false;
	}
	while(rfp.peek() != EOF){
		getline(rfp, line);
		wfp << line << '\n';
	}
	rfp.close();
	wfp.close();
	/*Delete το προσωρινό αρχείο*/
	if(std::remove("temp.dat"))
		std::cerr << "An error occurred while deleting the file. . ." << '\n';
	return true;
}