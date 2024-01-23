#ifndef SDDS_AIDMAN_H
#define SDDS_AIDMAN_H
#include "Menu.h"
#include "iProduct.h"

namespace sdds
{
    const int sdds_max_num_items = 100;

    class AidMan
    {
        char *m_filename;
        Menu m_mainMenu;

        iProduct *m_products[sdds_max_num_items];
        int m_numProducts;

        // Returns an unsigned integer that is the user's selection of an option in the main menu of the system.
        unsigned int menu();

        // For saving data records in file
        void save();

        // Deallocates all the memory allocated by the class
        void deallocate();

        // Loads all the records from the data file into the class.
        bool load();

        // List all the items
        int list(const char *sub_desc = nullptr);

        // Loops through all the iProduct Pointers elements and if the SKU is a match it will return the index, otherwise it will return -1
        int search(int sku) const;

        // Deletes the iProduct
        void remove(int index);

    public:
        // Construction //
        AidMan();
        AidMan(const char *filename);
        AidMan(const AidMan &other) = delete;
        AidMan &operator=(const AidMan &other) = delete;
        ~AidMan();

        // Runs the whole application
        void run();
    };
}

#endif // !SDDS_AIDMAN_H
