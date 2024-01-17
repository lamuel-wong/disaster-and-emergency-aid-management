#include "Status.h"
#include "Utils.h"

using namespace std;
namespace sdds
{
	// Construction
	Status::Status(const char *description)
	{
		m_description = nullptr;
		m_code = 0;
		if (description)
		{
			ut.alocpy(m_description, description);
		}
	}

	Status::~Status()
	{
		delete[] m_description;
	}

	// Copy constructor
	Status::Status(const Status &other)
	{
		m_description = nullptr;
		m_code = other.m_code;
		if (other.m_description)
		{
			ut.alocpy(m_description, other.m_description);
		}
	}

	// Assignment operator overloads
	Status &Status::operator=(const Status &other)
	{
		if (this != &other) // Check for self-assignment
		{
			ut.alocpy(m_description, other.m_description);
			m_code = other.m_code;
		}
		return *this;
	}
	Status &Status::operator=(const char *description)
	{
		ut.alocpy(m_description, description);
		return *this;
	}
	Status &Status::operator=(int code)
	{
		m_code = code;
		return *this;
	}

	// Type Conversion Overloads

	// Returns the code
	Status::operator int() const
	{
		return m_code;
	}

	// Returns the description
	Status::operator const char *() const
	{
		return m_description;
	}

	// Return true if the description is null and false if it is not.
	// If the Status has no description, the owner class is in a good (true) state. (No news is good news.)
	Status::operator bool() const
	{
		return m_description == nullptr;
	}

	// The clear() Method
	// Safely deallocates the description and sets the code to zero
	Status &Status::clear()
	{
		delete[] m_description;
		m_description = nullptr;
		m_code = 0;
		return *this;
	}

	// Prints Status only if it is in a bad (false) state.
	ostream &operator<<(ostream &os, const Status &status)
	{
		if (!status) // If status object is in a "bad" state
		{
			// Using operator conversion
			int code = status;
			const char *description = status;

			if (code != 0)
			{
				os << "ERR#" << code << ": ";
			}
			if (description != nullptr)
			{
				os << description;
			}
		}
		return os;
	}
}