#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <cstring>
#include "Utils.h"
using namespace std;
namespace sdds
{
    Utils ut;
    void Utils::testMode(bool testmode)
    {
        m_testMode = testmode;
    }
    void Utils::getSystemDate(int *year, int *mon, int *day)
    {
        if (m_testMode)
        {
            if (day)
                *day = sdds_testDay;
            if (mon)
                *mon = sdds_testMon;
            if (year)
                *year = sdds_testYear;
        }
        else
        {
            time_t t = std::time(NULL);
            tm lt = *localtime(&t);
            if (day)
                *day = lt.tm_mday;
            if (mon)
                *mon = lt.tm_mon + 1;
            if (year)
                *year = lt.tm_year + 1900;
        }
    }
    int Utils::daysOfMon(int month, int year) const
    {
        int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1};
        int mon = (month >= 1 && month <= 12 ? month : 13) - 1;
        return days[mon] + int((mon == 1) * ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
    }

    void Utils::alocpy(char *&destination, const char *source)
    {
        delete[] destination;
        if (source != nullptr)
        {
            destination = new char[strlen(source) + 1];
            strcpy(destination, source);
        }
        else
        {
            destination = nullptr;
        }
    }

    int Utils::getint(const char *prompt)
    {
        int value;

        if (prompt != nullptr)
        {
            cout << prompt;
        }

        while (!(cin >> value))
        {
            cin.clear();            // Clear error flags
            cin.ignore(1000, '\n'); // Clear the buffer before retry
            cout << "Invalid Integer, retry: ";
        }
        cin.ignore(1000, '\n');
        return value;
    }

    int Utils::getint(int min, int max, const char *prompt, const char *errMes)
    {
        int value;
        bool validInput = false;
        do
        {
            value = getint(prompt); // Get the initial input
            if (value < min || value > max)
            {
                cout << (errMes != nullptr ? errMes : "Value out of range") << " [" << min << "<=val<=" << max << "]: ";
            }
            else
            {
                validInput = true; // Valid input, break the loop
            }
        } while (!validInput);
        return value;
    }
}