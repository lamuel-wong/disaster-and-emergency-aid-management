#ifndef SDDS_STATUS_H
#define SDDS_STATUS_H
#include <iostream>

namespace sdds
{
	class Status
	{
		char *m_description;
		int m_code;

	public:
		// Construction
		Status(const char *description = nullptr);
		~Status();

		// Copy constructor
		Status(const Status &other);

		// Assignment operator overloads
		Status &operator=(const Status &other);		// Assignment from another Status
		Status &operator=(const char *description); // Assignment from a C-string
		Status &operator=(int code);				// Assignment from an integer code

		// Type Conversion Overloads

		// Returns the code
		operator int() const;

		// Returns the description
		operator const char *() const;

		// Return trues if the description is null and false if it is not.
		// If the Status has no description, the owner class is in a good (true) state. (No news is good news.)
		operator bool() const;

		// The clear() Method
		// Safely deallocates the description and sets the code to zero
		Status &clear();
	};

	// Helper Insertion Operator Overload

	// Prints Status only if it is in a bad (false) state.
	std::ostream &operator<<(std::ostream &os, const Status &status);
}

#endif