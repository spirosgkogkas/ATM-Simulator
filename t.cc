#include <iostream>
#include <iomanip>

int main(int argc, char const *argv[])
{
	system("chcp 65001");
std::string upEdge = "\u250F";
	std::string line = "\u2501";
	std::string upEdge2 = "\u2513";
	std::string side = "\u2503";
	std::string downEdge = "\u2517";
	std::string downEdge2 = "\u251B";

	std::cout << std::setw(129) << "ΔΗΜΙΟΥΡΓΙΑ ΛΟΓΑΡΙΑΣΜΟΥ" << std::endl;
	std::cout << std::setw(80) << upEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << upEdge2 << '\n';
	std::cout << std::setw(80) << side << "[1]:Κατάθεση μετρητών" << std::setw(20) << side << std::endl;
	std::cout << std::setw(80) << side << "[2]:Ανάληψη μετρητών" << std::setw(21) << side << std::endl;
	std::cout << std::setw(80) << side << "[3]:Μεταφορά" << std::setw(29) << side << std::endl;
	std::cout << std::setw(80) << side << "[4]:Τελευταίες συναλλαγές" << std::setw(16) << side << std::endl;
	std::cout << std::setw(80) << side << "[5]:Πληροφορίες λογαριασμού" << std::setw(14) << side << std::endl;
	std::cout << std::setw(80) << side << "[6]:Αλλαγή κωδικού" << std::setw(23) << side << std::endl;
	std::cout << std::setw(80) << side << "[7]:Έξοδος" << std::setw(31) << side << std::endl;
	std::cout << std::setw(80) << downEdge;
	for(int i = 0; i < 38; i++)
		std::cout << line;
	std::cout << downEdge2 << std::endl;
	return 0;
}