#ifndef SDDS_MENU_H
#define SDDS_MENU_H

namespace sdds
{
    const int MAX_NUM_OF_OPTIONS = 15;

    class Menu
    {
        char *m_content; // Dynamically allocated C-string that contains the list of options

    public:
        // Construction //
        Menu(const char *content);
        Menu(const Menu &other) = delete;
        Menu &operator=(const Menu &other) = delete;
        ~Menu();

        // Methods //
        // Counts the number of options based on number of tabs in the menu content
        int countOptions() const;

        // First displays the menu content in a menu format, prints a separator line and then prints "0- Exit" and finally goes to a new line.
        // Then it will display "> " as a prompt and waits for the user to enter an integer between 0 and the number of options.
        unsigned int run() const;
    };
}

#endif // !SDDS_MENU_H
