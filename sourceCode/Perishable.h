#ifndef SDDS_PERISHABLE_H
#define SDDS_PERISHABLE_H
#include "Item.h"
#include "Date.h"

namespace sdds
{
	class Perishable : public Item
	{
		Date expiryDate;
		char *handlingInstructions;

	public:
		// Construction
		Perishable();

		// Rule of Three
		Perishable(const Perishable &other);
		Perishable &operator=(const Perishable &other);
		virtual ~Perishable();

		// Query
		const Date &expiry() const;

		// Virtual Overrides
		virtual int readSku(std::istream &istr);
		virtual std::ofstream &save(std::ofstream &ofstr) const;
		virtual std::ifstream &load(std::ifstream &ifstr);
		virtual std::ostream &display(std::ostream &ostr) const;
		virtual std::istream &read(std::istream &istr);
	};
}

#endif // !SDDS_PERISHABLE_H
