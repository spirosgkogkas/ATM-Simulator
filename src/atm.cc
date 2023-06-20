/*
	Author: Spiros Gkogkas
	Date: March 22, 2022
*/

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
	#endif
	    
	std::cout << std::setw(38)<< "ATM" << std::endl;
	std::cout << std::setw(20) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << std::endl;
	std::cout << std::setw(20) << side << "[0]:Exit" << std::setw(33) << side << std::endl;
	std::cout << std::setw(20) << side << "[1]:Login" << std::setw(32) << side << std::endl;
	std::cout << std::setw(20) << side << "[2]:Register" << std::setw(29) << side << std::endl;
	std::cout << std::setw(20) << downEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << downEdge2 << std::endl;
}

bool atmdecl::ATM::setUser(std::string token){
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
	double balance;
	int ch = 0;
	bool found_character = false;
	/*Check for existing user*/
	std::cout << "Please give a Bank-ID: "; std::cin >> temp;
	if(!this->membersDatFile.is_open()) this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
	while(this->membersDatFile.peek() != EOF){
		getline(this->membersDatFile, temp2);
		if(temp == temp2.substr(0, temp2.find(' '))){
			std::cout << "Bank-ID already exists. . ." << std::endl;
			std::cin.get();
			this->membersDatFile.seekg(0, std::ios::beg);
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
	while(true){
		try{
			std::cout << "Enter PIN: ";
			this->chkPin(temp);
			break;
		}catch(const char * exc){
			this->clearScreen();
			std::cout << exc << '\n';
		}
	}
	this->currentUser.setPassword(temp);
	this->membersDatFile << temp << ' ';
	//We will not remove any character from std::cin because chkPin removes it.
	while(true){
		try{
			std::cout << "Enter your name: ";
			this->chkName(temp);
			break;
		}catch(const char * exc){
			this->clearScreen();
			std::cout << exc << '\n';
		}
	}
	this->currentUser.setName(temp);
	this->membersDatFile << temp << ' ';

	while(true){
		std::cout << "Enter Balance: ";
		while(std::cin.peek() == '\n' || std::cin.peek() == ' ' || std::cin.peek() == '\t'){
			this->clearScreen();
			std::cout << "Please enter valid balance. . .\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter Balance: ";
		}
		if(!(std::cin >> balance)){
			/*                                    -User input validation for balance-
				+---------------------------------------------------------------------------------------------------------------------+
				| We check first for the std::cin return value from operation with >> operator                                        |
				|if the return value is false it means that the user gave invalid input..                                             |
				|We clear the state of the std::cin and then clear anything left inside the buffer and the user needs to input again. |
				+---------------------------------------------------------------------------------------------------------------------+
                                                        -First check issues-
				+---------------------------------------------------------------------------------------------------------------------+
				| There would be a problem though if we used only this approach since the return value would be true if the operation |
				|could manage to store atleast a value into the balance variable.                                                     |
				| In Example:                                                                                                         |
				|   - std::cin > balance -                                                                                            |
				|   User input: 1#$asd2a                                                                                              |
				|   Return value: true                                                                                                |
				|                                                                                                                     |
				| So we will need to check if there are any characters left inside the buffer and if there are any we will prompt     |
				|the user again to input the balance..                                                                                |
				+---------------------------------------------------------------------------------------------------------------------+
				                                      -Check buffer leftovers-
				+---------------------------------------------------------------------------------------------------------------------+
				| The code below reads until it reaches \n character or until it finds a character between 32 and 126 Ascii Codes     |
				|if it finds any it will clear the rest of the buffer and will set the boolean found_character to true to input again.|
				| In case of not finding any character it will simply break out the loop and continue it's purpose.                   |
				+---------------------------------------------------------------------------------------------------------------------+
			*/
			this->clearScreen();
			std::cout << "Please provide only numbers. . .\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else{
			while((ch = std::cin.get()) != '\n'){
				if(ch >= 32 && ch <= 126){
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');	
					found_character = true;
					break;
				}
			}
			if(found_character){
				found_character = false;
				std::cout << "Please provide only numbers. . .\n";
				continue;
			}
			break;
		}
	}
	this->currentUser.setBalance(balance);
	this->membersDatFile << balance << '\n';
	this->clearScreen();
	std::cout << "Account created successfully.\n";
	this->membersDatFile.close();
	this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
	if(!this->membersDatFile.is_open())
		std::cout << "Never opened.";
	return true;
}

void atmdecl::ATM::userMenu(){
	/*User Menu*/
	std::string upEdge = "\u250F";
	std::string line = "\u2501";
	std::string upEdge2 = "\u2513";
	std::string side = "\u2503";
	std::string downEdge = "\u2517";
	std::string downEdge2 = "\u251B";

	std::cout << std::setw(41)<< "User Menu" << std::endl;
	std::cout << std::setw(20) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << std::endl;
	std::cout << std::setw(20) << side << "[1]:Cash deposit" << std::setw(25) << side << std::endl;
	std::cout << std::setw(20) << side << "[2]:Cash withdrawal" << std::setw(22) << side << std::endl;
	std::cout << std::setw(20) << side << "[3]:Transfer" << std::setw(29) << side << std::endl;
	std::cout << std::setw(20) << side << "[4]:Latest transactions" << std::setw(18) << side << std::endl;
	std::cout << std::setw(20) << side << "[5]:Account information" << std::setw(18) << side << std::endl;
	std::cout << std::setw(20) << side << "[6]:Change Password" << std::setw(22) << side << std::endl;
	std::cout << std::setw(20) << side << "[7]:Exit" << std::setw(33) << side << std::endl;
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
	if(transferToken[0] == 'Q' || transferToken[0] == 'q'){
		wfp.close();
		rfp.close();
		if(std::remove("temp.dat"))
		std::cerr << "An error occurred while deleting the file. . ." << '\n';
		return false;
	}
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

	std::cout << std::setw(45)<< "User information" << std::endl;
	std::cout << std::setw(20) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << std::endl;
	std::cout << std::setw(20) << side << "[Bank-ID]: " << std::setw(27) << bank_id << side << std::endl;
	std::cout << std::setw(20) << side << "[Name   ]: " << std::setw(27) << this->currentUser.getName() << side << std::endl;
	std::cout << std::setw(20) << side << "[Balance]: " << std::setw(27) << std::fixed << std::setprecision(2) << this->currentUser.getBalance() << side << std::endl;
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
	/*Remove temp file*/
	if(std::remove("temp.dat"))
		std::cerr << "An error occurred while deleting the file. . ." << '\n';
	return true;
}

bool atmdecl::ATM::chkPin(std::string& pin){
	/*
		+---------------------------------------------------------------------+
		|       This method checks for the correct pin format.                |
		+---------------------------------------------------------------------+
		| First check will check if the user typed an empty PIN or if the     |
		|first character is a whitespace character, if it is it will throw    |
		|an exception depending on what the first character is..              |
		|Next check will check if the pin had any whitespace character between|
		+---------------------------------------------------------------------+
		| The last check will check the length of the pin and if it's shorter |
		|than 4 characters or longer than 20 characters.                      |
		+---------------------------------------------------------------------+ 
		| Keep in mind every check will clear anything in the std::cin buffer |
		|and at the end of the method the '\n' character will be removed..    |
		+---------------------------------------------------------------------+
	*/
	while(std::cin.peek() == '\n' || std::cin.peek() == ' ' || std::cin.peek() == '\t'){
		if(std::cin.peek() == '\n'){
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			throw "Please provide a valid PIN. . .";
		}
		else{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			throw "Unable to use whitespace characters at the start of the PIN. . .";
		}
	}
	std::cin >> pin;
	if(std::cin.peek() == ' ' || std::cin.peek() == '\t'){
		pin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw "Please remove tabs/spaces. . .";
	}
	std::cin.get();

	if((pin.length() < 4) || (pin.length() > 20)){
		pin.clear();
		throw "The pin must be between 4 to 20 length.";
	}
	return true;
}

bool atmdecl::ATM::chkName(std::string &name){
	while(std::cin.peek() == '\n' || std::cin.peek() == ' ' || std::cin.peek() == '\t'){
		if(std::cin.peek() == '\n'){
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			throw "Please provide a valid name. . .";
		}
		else{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			throw "Unable to use whitespace characters at the start of the name. . .";
		}
	}
	std::cin >> name;
	if(std::cin.peek() == ' ' || std::cin.peek() == '\t'){
		name.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw "Please remove tabs/spaces. . .";
	}

	for(char c : name)
		if(!std::isalpha(c)){
			name.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			throw "Please provide only letters. . .";
		}
	std::cin.get();
	return true;
}