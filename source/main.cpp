#include <iostream>
#include <fstream>
#include "AidMan.h"
#include "Utils.h"
using namespace sdds;
using namespace std;

void copyfile(const char *to, const char *from);
void displayFile(const char *file);

// Simulate execution of Aid Management System
int main()
{
	copyfile("data.dat", "data56.dat");
	ut.testMode();
	AidMan().run();
	displayFile("data.dat");
	cout << endl
		 << "------------------------------------------------" << endl;
	displayFile("shippingOrder.txt");
	return 0;
}

// Display contents of file
void displayFile(const char *file)
{
	ifstream in(file);
	char ch;
	cout << "File: " << file << endl;
	while (in.get(ch))
		cout << ch;
}

// Copies contents from one file to another for reading
void copyfile(const char *to, const char *from)
{
	std::ifstream in(from);
	std::ofstream out(to);
	char ch;
	while (in.get(ch))
		out.put(ch);
}