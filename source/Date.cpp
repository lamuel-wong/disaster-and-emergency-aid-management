#include "Date.h"
#include "Utils.h"

using namespace std;
namespace sdds
{
	// Validates the year, month and day.
	bool Date::validate()
	{
		bool isValid = true;
		int sysYear, sysMon, sysDay;
		int daysInMonth = ut.daysOfMon(m_month, m_year);

		ut.getSystemDate(&sysYear, &sysMon, &sysDay); // Get current system date

		// Year validation
		if (m_year < sysYear || m_year > MAX_YEAR)
		{
			m_state = "Invalid year in date";
			m_state = 1;
			isValid = false;
		}
		// Month validation
		else if (m_month < MIN_MONTH || m_month > MAX_MONTH)
		{
			m_state = "Invalid month in date";
			m_state = 2;
			isValid = false;
		}
		// Day validation
		else if (m_day < MIN_DAY || m_day > daysInMonth)
		{
			m_state = "Invalid day in date";
			m_state = 3;
			isValid = false;
		}
		// If all checks pass, clear the state
		else
		{
			m_state.clear();
		}

		return isValid;
	}

	// Returns a unique integer value tied to the date
	int Date::uniqueDateValue() const
	{
		return m_year * 372 + m_month * 31 + m_day;
	}

	// Construction
	Date::Date()
	{
		ut.getSystemDate(&m_year, &m_month, &m_day); // Sets current system date
		m_formatted = true;
		validate();
	}

	Date::Date(int year, int month, int day)
	{
		m_year = year;
		m_month = month;
		m_day = day;
		m_formatted = true;
		validate();
	}

	// Comparison operators
	bool Date::operator==(const Date &rhs) const
	{
		return uniqueDateValue() == rhs.uniqueDateValue();
	}

	bool Date::operator!=(const Date &rhs) const
	{
		return !(*this == rhs);
	}

	bool Date::operator<(const Date &rhs) const
	{
		return uniqueDateValue() < rhs.uniqueDateValue();
	}

	bool Date::operator>(const Date &rhs) const
	{
		return uniqueDateValue() > rhs.uniqueDateValue();
	}

	bool Date::operator<=(const Date &rhs) const
	{
		return !(*this > rhs);
	}

	bool Date::operator>=(const Date &rhs) const
	{
		return !(*this < rhs);
	}

	// State accessor
	const Status &Date::state() const
	{
		return m_state;
	}

	// Formatted modifier
	Date &Date::formatted(bool flag)
	{
		m_formatted = flag;
		return *this;
	}

	// Bool conversion overload
	Date::operator bool() const
	{
		return m_state;
	}

	// Write method
	ostream &Date::write(ostream &os) const
	{
		if (m_formatted)
		{
			os << m_year << '/' << (m_month < 10 ? "0" : "") << m_month
			   << '/' << (m_day < 10 ? "0" : "") << m_day;
		}
		else
		{
			os << (m_year % 100) << (m_month < 10 ? "0" : "") << m_month
			   << (m_day < 10 ? "0" : "") << m_day;
		}
		return os;
	}

	// Read method
	istream &Date::read(istream &is)
	{
		int dateInput;

		if (is >> dateInput)
		{
			if (dateInput <= 9999)
			{
				// Set year to current year and calculate month and day
				ut.getSystemDate(&m_year, nullptr, nullptr);
				m_month = dateInput / 100;
				m_day = dateInput % 100;
			}
			else
			{
				// Calculate year, month, and day for full dates
				m_year = (dateInput / 10000) + 2000;
				m_month = (dateInput % 10000) / 100;
				m_day = dateInput % 100;
			}
			// Validate the date
			if (!validate())
			{
				// If validation fails, set badbit
				is.setstate(ios::badbit);
			}
		}
		else
		{
			// If input was not an integer, set failbit
			is.setstate(ios::failbit);
			m_state = "Invalid date value";
		}
		return is;
	}

	// Helper insertion and extraction operator overloads
	ostream &operator<<(ostream &os, const Date &date)
	{
		return date.write(os);
	}

	istream &operator>>(istream &is, Date &date)
	{
		return date.read(is);
	}
}