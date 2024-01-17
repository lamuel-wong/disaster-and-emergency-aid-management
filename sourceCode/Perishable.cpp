#define _CRT_SECURE_NO_WARNINGS
#include "Perishable.h"
#include "Utils.h"
#include <cstring>
#include <iomanip>

using namespace std;

namespace sdds
{
	const int MAX_DESC_LENGTH = 1000;

	// Construction
	Perishable::Perishable() : Item()
	{
		expiryDate = Date(0, 0, 0);
		handlingInstructions = nullptr;
	}

	// Rule of Three
	Perishable::Perishable(const Perishable &other) : Item(other)
	{
		expiryDate = other.expiryDate;

		handlingInstructions = nullptr;
		ut.alocpy(handlingInstructions, other.handlingInstructions);
	}

	Perishable &Perishable::operator=(const Perishable &other)
	{
		if (this != &other)
		{
			Item::operator=(other);

			expiryDate = other.expiryDate;

			delete[] handlingInstructions;
			handlingInstructions = nullptr;
			ut.alocpy(handlingInstructions, other.handlingInstructions);
		}
		return *this;
	}

	Perishable::~Perishable()
	{
		delete[] handlingInstructions;
	}

	// Query
	const Date &Perishable::expiry() const
	{
		return expiryDate;
	}

	// Virtual Overrides
	int Perishable::readSku(istream &istr)
	{
		const int MIN_SKU = 10000;
		const int MAX_SKU = 39999;

		cout << "SKU: ";
		sku = ut.getint(MIN_SKU, MAX_SKU, nullptr, "Value out of range");

		return sku;
	}

	ofstream &Perishable::save(ofstream &ofstr) const
	{
		if (static_cast<bool>(itemStatus))
		{
			Item::save(ofstr);

			ofstr << '\t';
			if (handlingInstructions != nullptr)
			{
				ofstr << handlingInstructions;
			}
			ofstr << '\t';

			Date tempDate = expiryDate;
			tempDate.formatted(false);
			ofstr << tempDate;
		}
		return ofstr;
	}

	ifstream &Perishable::load(ifstream &ifstr)
	{
		char tempInstructions[MAX_DESC_LENGTH];

		Item::load(ifstr);

		// Check for if handling instructions are empty
		if (ifstr.peek() == '\t')
		{
			ifstr.ignore(); // Ignore tab if handling instructions are empty
			strcpy(tempInstructions, "");
		}
		else
		{
			ifstr.getline(tempInstructions, MAX_DESC_LENGTH, '\t');
		}

		ut.alocpy(handlingInstructions, tempInstructions);

		ifstr >> expiryDate;
		ifstr.ignore(1000, '\n');

		if (ifstr.fail())
		{
			itemStatus = "Input file stream read (perishable) failed!";
		}
		return ifstr;
	}

	ostream &Perishable::display(ostream &ostr) const
	{
		if (!itemStatus)
		{
			ostr << itemStatus;
		}
		else
		{
			if (this->linear())
			{
				Item::display(ostr);

				// if handling instructions are not null and not empty a single asterisk ('*') is printed otherwise a single space (' ') is printed.
				ostr << (handlingInstructions != nullptr && handlingInstructions[0] != '\0' ? '*' : ' ');
				ostr << expiryDate;
			}
			else
			{
				ostr << "Perishable ";
				Item::display(ostr);
				ostr << "Expiry date: " << expiryDate << endl;

				if (handlingInstructions != nullptr && handlingInstructions[0] != '\0')
				{
					ostr << "Handling Instructions: " << handlingInstructions << endl;
				}
			}
		}
		return ostr;
	}

	istream &Perishable::read(istream &istr)
	{
		Item::read(istr);

		delete[] handlingInstructions;
		handlingInstructions = nullptr;

		cout << "Expiry date (YYMMDD): ";
		istr >> expiryDate;
		istr.ignore(1000, '\n');

		cout << "Handling Instructions, ENTER to skip: ";
		if (istr.peek() != '\n')
		{
			char tempInstructions[MAX_DESC_LENGTH];
			istr.getline(tempInstructions, MAX_DESC_LENGTH);
			ut.alocpy(handlingInstructions, tempInstructions);
		}

		if (istr.fail())
		{
			itemStatus = "Perishable console date entry failed!";
		}

		return istr;
	}

}