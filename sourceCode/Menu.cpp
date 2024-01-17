#include <iostream>
#include "Menu.h"
#include "Utils.h"

using namespace std;

namespace sdds
{
    // Construction //
    Menu::Menu(const char *content)
    {
        m_content = nullptr;
        ut.alocpy(m_content, content);
    }

    Menu::~Menu()
    {
        delete[] m_content;
    }

    // Methods //
    // Counts the number of options based on number of tabs in the menu content
    int Menu::countOptions() const
    {
        int count = 1;
        const char *str;

        if (m_content)
        {
            for (str = m_content; *str; ++str)
            {
                if (*str == '\t')
                {
                    ++count;
                }
            }
        }
        return count;
    }

    // First displays the menu content in a menu format, prints a separator line and then prints "0- Exit" and finally goes to a new line.
    // Then it will display "> " as a prompt and waits for the user to enter an integer between 0 and the number of options.
    unsigned int Menu::run() const
    {
        int numOfOptions = countOptions();
        int optionNumber = 1;
        const char *str;

        if (!m_content || countOptions() > MAX_NUM_OF_OPTIONS)
        {
            cout << "Invalid Menu!\n";
            return 0;
        }

        // Displaying menu options
        cout << optionNumber << "- ";
        for (str = m_content; *str; ++str)
        {
            if (*str != '\t')
            {
                cout << *str;
            }
            else
            {
                cout << '\n'
                     << ++optionNumber << "- ";
            }
        }

        cout << "\n---------------------------------\n0- Exit\n> ";

        return static_cast<unsigned int>(
            ut.getint(0, numOfOptions));
    }
}