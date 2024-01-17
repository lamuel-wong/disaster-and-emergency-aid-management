#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <iomanip>
#include "AidMan.h"
#include "Utils.h"
#include "Date.h"
#include "Perishable.h"

using namespace std;

namespace sdds
{

    // Returns an unsigned integer that is the user's selection of an option in the main menu of the system.
    unsigned int AidMan::menu()
    {
        // Display current date using Date class
        int year, month, day;
        ut.getSystemDate(&year, &month, &day); // Retrieve the system date
        Date currentDate(year, month, day);

        cout << "Aid Management System" << endl;
        cout << "Date: " << currentDate.formatted(true) << endl; // Display date in formatted form
        cout << "Data file: " << (m_filename ? m_filename : "No file") << endl;
        cout << "---------------------------------" << endl;
        return m_mainMenu.run();
    }

    // For saving data records in file
    void AidMan::save()
    {
        if (m_filename != nullptr)
        {
            ofstream outfile(m_filename); // Open file for writing

            // Goes through the iProduct Pointers up to the number of number of iProduct Items and
            // calls the save() of each iProduct to write them in the file.
            for (int i = 0; i < m_numProducts; ++i)
            {
                if (m_products[i] != nullptr)
                {
                    m_products[i]->save(outfile);
                    outfile << endl;
                }
            }

            outfile.close(); // Close the file
        }
    }

    // Deallocates all the memory allocated by the class
    void AidMan::deallocate()
    {
        for (int i = 0; i < m_numProducts; ++i)
        {
            delete m_products[i];
            m_products[i] = nullptr;
        }

        // Delete filename and reset product count
        delete[] m_filename;
        m_filename = nullptr;

        m_numProducts = 0;
    }

    // Loads all the records from the data file into the class.
    bool AidMan::load()
    {
        bool loaded = false;
        char firstDigit;

        // Open m_filename for reading in an ifstream
        ifstream infile(m_filename);

        // If opening the file was not successful
        if (!infile)
        {
            cout << "Failed to open " << m_filename << " for reading!" << endl;

            // Prompt to create a new file
            cout << "Would you like to create a new data file?" << endl
                 << "1- Yes!" << endl
                 << "0- Exit" << endl
                 << "> ";

            int choice;
            choice = ut.getint();

            // If the user selects yes, it will create an empty file under the same name and in any case, the function exits after.
            if (choice == 1)
            {
                ofstream outfile(m_filename);
                outfile.close();
            }
            return false;
        }

        while (infile.peek() != EOF)
        {
            firstDigit = infile.peek(); // Peek at first digit of SKU

            // Perishable
            if (firstDigit >= '1' && firstDigit <= '3')
            {
                m_products[m_numProducts] = new Perishable();
            }
            // Item
            else if (firstDigit >= '4' && firstDigit <= '9')
            {
                m_products[m_numProducts] = new Item();
            }
            // Invalid
            else
            {
                infile.ignore(1000, '\n');
            }

            // Calls the load method of the item to load the data from the file.
            // The loaded item is checked to be in a good state, if true, number of iProduct Items is added by one,
            // if false the loaded item is deleted.
            if (m_products[m_numProducts] && m_products[m_numProducts]->load(infile))
            {
                m_numProducts++;
                loaded = true;
            }
            else
            {
                delete m_products[m_numProducts];
                m_products[m_numProducts] = nullptr;
            }
        }
        cout << m_numProducts << " records loaded!" << endl;

        return loaded;
    }

    // List all the items
    int AidMan::list(const char *sub_desc)
    {
        int count = 0;

        // Header
        cout << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry" << endl;
        cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;

        for (int i = 0; i < m_numProducts; ++i)
        {
            // If sub_desc is null, print all the items in a linear format.
            // if sub_desc is not, null print only the items containing the sub_desc in their description.
            if (m_products[i] != nullptr &&
                (sub_desc == nullptr || m_products[i]->operator==(sub_desc)))
            {
                m_products[i]->linear(true);
                cout << setw(4) << i + 1 << " | ";
                m_products[i]->display(cout);
                cout << endl;
                count++;
            }
        }

        // If no items are listed
        if (count == 0)
        {
            cout << "The list is empty!" << endl;
        }
        else
        {
            // Footer
            cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;
        }

        // Return the number of iProducts printed
        return count;
    }

    // Loops through all the iProduct Pointers elements and if the SKU is a match it will return the index, otherwise it will return -1
    int AidMan::search(int sku) const
    {
        for (int i = 0; i < m_numProducts; ++i)
        {
            if (m_products[i] != nullptr && *m_products[i] == sku)
            {
                return i;
            }
        }
        return -1;
    }

    // Deletes the iProduct
    void AidMan::remove(int index)
    {
        if (index >= 0 && index < m_numProducts)
        {
            delete m_products[index]; // Delete the iProduct

            // Shift elements to the left
            for (int i = index; i < m_numProducts - 1; i++)
            {
                m_products[i] = m_products[i + 1];
            }

            m_products[m_numProducts - 1] = nullptr;
            m_numProducts--;
        }
    }

    // Construction //
    AidMan::AidMan() : m_mainMenu("List Items\tAdd Item\tRemove Item\tUpdate Quantity\tSort\tShip Items\tNew/Open Aid Database")
    {
        m_filename = nullptr;

        for (int i = 0; i < sdds_max_num_items; ++i)
        {
            m_products[i] = nullptr;
        }

        m_numProducts = 0;
    }

    AidMan::AidMan(const char *filename)
        : m_mainMenu("List Items\tAdd Item\tRemove Item\tUpdate Quantity\tSort\tShip Items\tNew/Open Aid Database")
    {
        m_filename = nullptr;
        ut.alocpy(m_filename, filename);

        for (int i = 0; i < sdds_max_num_items; ++i)
        {
            m_products[i] = nullptr;
        }
        m_numProducts = 0;
    }

    AidMan::~AidMan()
    {
        delete[] m_filename;
    }

    // Runs the whole application
    void AidMan::run()
    {
        unsigned int selection;
        do
        {
            selection = menu();

            if (selection != 0 && m_filename == nullptr && selection != 7)
            {
                selection = 7;
            }

            if (selection == 0)
            {
                cout << "Exiting Program!" << endl;
                save();
                deallocate();
                break;
            }
            cout << endl
                 << "****";

            switch (selection)
            {
            case 1:
            {
                cout << "List Items****" << endl;
                if (list() > 0)
                {
                    cout << "Enter row number to display details or <ENTER> to continue:" << endl
                         << "> ";
                    char input[1001]; // Buffer for input
                    cin.getline(input, 1000);

                    if (input[0] != '\0')
                    {
                        // Check if input is not just ENTER
                        int row;
                        if (sscanf(input, "%d", &row) == 1 && row > 0 && row <= m_numProducts)
                        {
                            m_products[row - 1]->linear(false);
                            m_products[row - 1]->display(cout); // Display the selected item in non-linear format
                            cout << endl;
                        }
                        else
                        {
                            cout << "Invalid row number. Returning to main menu.\n";
                        }
                    }
                    // If input is just ENTER, return to the main menu
                }
                break;
            }
            case 2:
            {
                cout << "Add Item****" << endl;
                if (m_numProducts >= sdds_max_num_items)
                {
                    cout << "Database full!" << endl;
                    break;
                }

                cout << "1- Perishable" << endl
                     << "2- Non-Perishable" << endl
                     << "-----------------" << endl
                     << "0- Exit" << endl
                     << "> ";

                int choice = ut.getint();

                // If the user chooses to exit:
                if (choice == 0)
                {
                    cout << "Aborted\n";
                    break;
                }

                // If the user chooses not to exit:
                iProduct *product = nullptr;

                if (choice == 1)
                {
                    product = new Perishable();
                }
                else if (choice == 2)
                {
                    product = new Item();
                }

                // The SKU of the item is read from the console and searched against already existing Items in the system.
                int sku = product->readSku(cin);
                int foundIndex = search(sku);

                if (foundIndex != -1)
                {
                    cout << "Sku: " << sku << " is already in the system, try updating quantity instead.\n";
                    delete product;
                    break;
                }

                // Read the rest of the data and add to array if in good state
                product->read(cin);
                if (*product)
                {
                    m_products[m_numProducts++] = product;
                }
                else
                {
                    product->display(cout);
                    delete product;
                }
                break;
            }
            case 3:
            {
                cout << "Remove Item****" << endl;
                cout << "Item description: ";
                char description[1001];
                cin.getline(description, 1000);

                // If any item was found and listed, ask for one of the SKU numbers listed to remove.
                if (list(description) > 0)
                {
                    cout << "Enter SKU: ";
                    int sku = ut.getint();
                    int index = search(sku);

                    if (index == -1)
                    {
                        cout << "SKU not found!" << endl;
                    }
                    else
                    {
                        // If SKU is found, print the Item, confirmation message and menu:
                        cout << "Following item will be removed: " << endl;
                        m_products[index]->linear(false); // Display the selected item in non-linear format
                        m_products[index]->display(cout);

                        cout << endl
                             << "Are you sure?" << endl
                             << "1- Yes!" << endl
                             << "0- Exit" << endl
                             << "> ";

                        int confirm = ut.getint();
                        if (confirm == 1)
                        {
                            remove(index);
                            cout << "Item removed!" << endl;
                        }
                        else
                        {
                            cout << "Aborted!" << endl;
                        }
                    }
                }
                break;
            }
            case 4:
            {
                cout << "Update Quantity****" << endl;
                cout << "Item description: ";
                char description[1001];
                cin.getline(description, 1000);

                // Lists all the iProducts containing the description
                if (list(description) == 0) // If no match is found
                {
                    cout << "No matches found!\n";
                    break;
                }

                // Ask the user for the SKU of the iProduct that needs updating
                cout << "Enter SKU: ";
                int sku = ut.getint();
                int index = search(sku); //  Search for the SKU to find the index of the iProcuct in the iProduct Pointers.

                // If a match is not found
                if (index == -1)
                {
                    cout << "SKU not found!\n";
                    break;
                }
                // If a match is found, display a menu for adding or reducing the quantity
                cout << "1- Add" << endl
                     << "2- Reduce" << endl
                     << "0- Exit" << endl
                     << "> ";
                int choice = ut.getint();

                switch (choice)
                {
                case 0:
                    cout << "Aborted!\n"; // Exit
                    break;
                case 1:
                {
                    int qtyNeeded = m_products[index]->qtyNeeded();
                    int qtyAvailable = m_products[index]->qty();

                    if (qtyAvailable >= qtyNeeded)
                    {
                        cout << "Quantity Needed already fulfilled!\n";
                        break;
                    }

                    // Value is received from 1 up to the maximum amount needed to fulfill the needed quantity
                    cout << "Quantity to add: ";
                    int qtyToAdd = ut.getint(1, qtyNeeded - qtyAvailable);
                    if (qtyToAdd <= 0)
                    {
                        cout << "No quantity added!\n";
                        break;
                    }
                    *m_products[index] += qtyToAdd; // The quantity is increased by the entered amount.
                    cout << qtyToAdd << " items added!\n";
                    break;
                }
                case 2:
                {
                    int qtyAvailable = m_products[index]->qty();
                    if (qtyAvailable == 0)
                    {
                        cout << "Quantity on hand is zero!\n";
                        break;
                    }

                    // Value is received from 1 up to the quantity on hand
                    cout << "Quantity to reduce: ";
                    int qtyToReduce = ut.getint(1, qtyAvailable); // The quantity is reduced by the entered amount.
                    *m_products[index] -= qtyToReduce;
                    cout << qtyToReduce << " items removed!\n";
                    break;
                }
                default:
                    cout << "Invalid selection.\n";
                    break;
                }
                break;
            }
            case 5:
                cout << "Sort****" << endl;

                // Sorts the items in the iProduct Pointers array, based on difference between quantity needed and quantity on hand in descending order.
                bool swapped;
                do
                {
                    swapped = false;
                    for (int i = 0; i < m_numProducts - 1; i++)
                    {
                        // Calculate differences for current items
                        int diffCurrent = m_products[i]->qtyNeeded() - m_products[i]->qty();
                        int diffNext = m_products[i + 1]->qtyNeeded() - m_products[i + 1]->qty();

                        // Check if next item's difference is greater than current item's
                        if (diffNext > diffCurrent)
                        {
                            // Swap the items
                            iProduct *temp = m_products[i];
                            m_products[i] = m_products[i + 1];
                            m_products[i + 1] = temp;
                            swapped = true;
                        }
                    }
                } while (swapped);
                cout << "Sort completed!\n";
                break;
            case 6:
            {
                cout << "Ship Items****" << endl;

                ofstream outfile("shippingOrder.txt");
                if (!outfile)
                {
                    cout << "Error: Unable to open shippingOrder.txt for writing." << endl;
                    break;
                }

                int year, month, day;
                ut.getSystemDate(&year, &month, &day);
                Date currentDate(year, month, day);

                outfile << "Shipping Order, Date: " << currentDate.formatted(true) << endl;
                outfile << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry" << endl
                        << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;

                int shippedCount = 0;
                for (int i = 0; i < m_numProducts; ++i)
                {
                    // If the quantity needed and quantity on hand of the product is a match
                    // print it in the linear format into the file and remove it from the iProduct Pointers array.
                    if (m_products[i] && m_products[i]->qtyNeeded() == m_products[i]->qty())
                    {
                        outfile << setw(4) << shippedCount + 1 << " | ";
                        m_products[i]->linear(true);
                        m_products[i]->display(outfile);
                        outfile << endl;
                        remove(i);
                        i--;
                        shippedCount++;
                    }
                }

                outfile << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;
                cout << "Shipping Order for " << shippedCount << " times saved!" << endl;
                break;
            }

            case 7:
            {
                cout << "New/Open Aid Database****" << endl;
                if (m_filename == nullptr)
                {
                    char tempFilename[1001];
                    cout << "Enter file name: ";
                    cin.getline(tempFilename, 1000);

                    if (tempFilename[0] != '\0')
                    {
                        delete[] m_filename;
                        m_filename = new char[strlen(tempFilename) + 1];
                        strcpy(m_filename, tempFilename);
                    }
                }

                // Attempt to load data
                if (m_filename != nullptr)
                {
                    if (!load())
                    {
                        cout << "No data loaded." << endl;
                    }
                }
                break;
            }
            default:
                // Continue loop to prompt menu again
                cout << "Invalid option [" << selection << "], please try again." << endl;
            }
            cout << endl;
        } while (true);
    }
}
