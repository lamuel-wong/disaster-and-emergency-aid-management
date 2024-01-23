#ifndef SDDS_DATE_H
#define SDDS_DATE_H
#include <iostream>
#include "Status.h"

namespace sdds
{
    const int MAX_YEAR = 2030;
    const int MIN_MONTH = 1;
    const int MAX_MONTH = 12;
    const int MIN_DAY = 1;

    class Date
    {
        int m_year;
        int m_month;
        int m_day;
        Status m_state;
        bool m_formatted;

        // Validates the year, month and day.
        bool validate();

        // Returns a unique integer value tied to the date
        int uniqueDateValue() const;

    public:
        // Construction
        Date();
        Date(int year, int month, int day);

        // Comparison operators
        bool operator==(const Date &rhs) const;
        bool operator!=(const Date &rhs) const;
        bool operator<(const Date &rhs) const;
        bool operator>(const Date &rhs) const;
        bool operator<=(const Date &rhs) const;
        bool operator>=(const Date &rhs) const;

        // State accessor
        const Status &state() const;

        // Formatted modifier
        Date &formatted(bool flag);

        // Bool conversion overload
        operator bool() const;

        // Write method
        std::ostream &write(std::ostream &os) const;

        // Read method
        std::istream &read(std::istream &is);
    };

    // Helper insertion and extraction operator overloads
    std::ostream &operator<<(std::ostream &os, const Date &date);
    std::istream &operator>>(std::istream &is, Date &date);
}

#endif