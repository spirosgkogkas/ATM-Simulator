#include "../Lib/atm.hpp"

atmdecl::ATM::ATM(){
	#ifdef _WIN32
		system("chcp 65001 > nul"); //Utf-8 encoding
	#endif
	this->tries = 3;
	/*Άνοιγμα του αρχείου bank.dat σε read mode */
	this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
	/*Αν δεν υπάρχει το αρχείο τότε δημιουργούμε το αρχείο*/
	if(!this->membersDatFile.is_open()){
		this->membersDatFile.open(BANK_DAT_FILE, std::ios::out);
		this->membersDatFile.close();
		this->membersDatFile.open(BANK_DAT_FILE, std::ios::in);
		if(!this->membersDatFile.is_open())
			std::cerr << "Πρόβλημα κατά την δημιουργία του αρχείου. . ." << std::endl;
	}
	this->LOG_FP.open(LOG_FILE, std::ios::app);
	if(!this->LOG_FP.is_open())
		std::cerr << "Πρόβλημα κατά το άνοιγμα του αρχείου. . ." << std::endl;
}

atmdecl::ATM::~ATM(){
	/*Κλέισημο τον αρχείων*/
	if(this->membersDatFile.is_open())
		this->membersDatFile.close();
	this->LOG_FP.close();
}

atmdecl::User::User(){

}

atmdecl::User::~User(){
	
}

void atmdecl::ATM::atmMenu(){
	/*Menu για τις αρχικές επιλογες*/
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

bool atmdecl::ATM::setUser(std::string token){
	/*Προσωρινές μεταβλητές*/
	std::string tempToken, tempPin, tempName, tempBalance;
	if(!this->membersDatFile.is_open()){
		std::cerr << "Πρόβλημα κατά την εκτέλεση. . .\n";
		return false;
	}
	/*Ελεγχος για να δούμε αν το αρχείο έχει δεδομένα.*/
	if(this->membersDatFile.peek() == EOF) return false;

	do{
		/*Αποθηκεύουμε τα στοιχεία της γραμμής στις προσωρινές μεταβλητές*/
		getline(this->membersDatFile, tempToken, ' ');
		getline(this->membersDatFile, tempPin, ' ');
		getline(this->membersDatFile, tempName, ' ');
		getline(this->membersDatFile, tempBalance);
		/*
			Αν ο κωδικός λογαριασμού ταιριάζει με αυτόν που έδωσε ο χρήστης τότε αποθηκεύουμε τα στοιχεία του χρήστη
			και βάζουμε τον pointer στην αρχή του αρχείου επιστρέφοντας την τιμη true.
			Διαφορετικά η διαδικασία θα τρέχει μέχρι να φτάσουμε στο τέλος του αρχείου.
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
	/*Καθάρίζουμε τά flags του αρχείου και δείχνουμε πάλι στην αρχή.*/
	this->membersDatFile.clear();
	this->membersDatFile.seekg(0, std::ios::beg);
	return false;
}

bool atmdecl::ATM::createAccount(){
	std::string temp, temp2;
	/*Δημιουργία χρήστη.*/
	std::cout << "Δώσε αριθμό λογαριασμού: "; std::cin >> temp;
	if(this->membersDatFile.is_open()){
		/*Έλεγχος για το αν ο χρήστης υπάρχει ήδη*/
		while(this->membersDatFile.peek() != EOF){
			getline(this->membersDatFile, temp2);
			if(temp == temp2.substr(0, temp2.find(' '))){
				std::cout << "Ο λογαριασμός υπάρχει ήδη . ." << std::endl;
				return false;
			}
		}
		this->membersDatFile.close();
	}
	
	this->membersDatFile.open(BANK_DAT_FILE, std::ios::app);
	if(!this->membersDatFile.is_open()){
		std::cerr << "Πρόβλημα κατα την δημιουργία λογαριασμού. . .\n";
		return false;
	}
	
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
	/*Menu Χρήστη*/
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
	/*Μεταφορά απο λογαριασμό σε λογαριασμό*/
	std::string transferToken, temp;
	std::ifstream rfp(BANK_DAT_FILE, std::ios::in);
	std::ofstream wfp("temp.dat", std::ios::out);
	double value;
	bool found = false;
	/*Ανοίγουμε 2 αρχεία, 1 για ανάγνωση και 1 για τα ανανεωμένα δεδομένα μας*/
	if(!rfp.is_open()){
		std::cerr << "Πρόβλημα κατά την ανάγνωση δεδομένων. . .\n";
		return false;
	}
	std::cout << "Παρακαλώ πληκτρολογήστε τον κωδικό λογαριασμού μεταφοράς (Q/q για ακύρωση): "; std::cin >> transferToken;
	std::cin.get();
	if(transferToken == "Q" || transferToken == "q") return false;
	else if(transferToken == this->currentUser.getToken()){
		std::cerr << "Μη αποδεκτή ενέργεια. . ." << std::endl;
		return false;
	}

	do{
		std::cout << "Υπόλοιπο: " << this->currentUser.getBalance() << '\n';
		std::cout << "Παρακαλώ πληκτρολογήστε ποσό μεταφοράς (0 για ακύρωση): "; std::cin >> value;
		std::cin.get();
		/*Ελεγχος σε περιπτωση ακύρωσης*/
		if(!value) return false;
		/*Ελεγχος σε περίπτωση μη επιτρεπτού ποσού.*/
		if(value > this->currentUser.getBalance())
			std::cerr << "Μη αποδεκτό ποσό μεταφοράς. . .\n";
	}while(value > this->currentUser.getBalance());

	/*Διαβασμα και επεξεργασία δεδομένων*/
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

	rfp.open("temp.dat", std::ios::in);
	if(!rfp.is_open()){
		std::cerr << "Παρουσιάστηκε πρόβλημα, επικοινωνηστε με τον διαχειριστή. . .\n";
		exit(0);
	}

	wfp.open(BANK_DAT_FILE, std::ios::out);
	if(!wfp.is_open()){
		std::cerr << "Παρουσιάστηκε πρόβλημα, επικοινωνηστε με τον διαχειριστή. . .\n";
		exit(0);
	}
	while(rfp.peek() != EOF){
		getline(rfp, temp);
		wfp << temp << '\n';
	}
	rfp.close();
	wfp.close();
	return found;
}

void atmdecl::ATM::updateUser(){
	/*Update User*/
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
	/*Delete το προσωρινό αρχείο*/
	if(std::remove("temp.dat"))
		std::cerr << "Το αρχείο δεν μπόρεσε να διαγραφτεί. . ." << '\n';
}

void atmdecl::ATM::transactions(){
	/*Συναλλαγές.*/
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
	if(lines.empty()){
		std::cout << "Ο λογαριασμός [" << token << "] δεν έχει συναλλαγές. . ." << std::endl;
		return;
	}
	std::reverse(lines.begin(), lines.end());
	for(size_t i = 0; i < count && i < lines.size(); i++)
		std::cout << "Συναλλαγή: " << lines[i] << std::endl;
}

void atmdecl::ATM::userInfo(){
	/*Πληροφορίες χρήστη.*/
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
	std::cout << std::setw(80) << side << "[Υπόλοιπο    ]: " << std::setw(22) << std::fixed << std::setprecision(2) << this->currentUser.getBalance() << side << '\n';
	std::cout << std::setw(80) << downEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << downEdge2 << std::endl;
}

bool atmdecl::ATM::changePassword(){
	/*Αλλαγή ΠΙΝ χρήστη.*/
	std::string line, password;
	std::ifstream rfp(BANK_DAT_FILE, std::ios::in);
	std::ofstream wfp("temp.dat", std::ios::out);
	if(!rfp.is_open()){
		std::cerr << "Πρόβλημα ανόιγματος αρχείου. . .\n";
		return false;
	}
	if(!wfp.is_open()){
		std::cerr << "Πρόβλημα ανόιγματος αρχείου. . .\n";
		return false;
	}
	std::cout << "Δώσε καινούργιο PIN (Q/q για ακύρωση): "; std::cin >> password;
	std::cin.get();
	if(password == "Q" || password == "q") return false;
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
		std::cerr << "Πρόβλημα ανόιγματος αρχείου. . .\n";
		return false;
	}
	if(!wfp.is_open()){
		std::cerr << "Πρόβλημα ανόιγματος αρχείου. . .\n";
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
		std::cerr << "Το αρχείο δεν μπόρεσε να διαγραφτεί. . ." << '\n';
	return true;
}