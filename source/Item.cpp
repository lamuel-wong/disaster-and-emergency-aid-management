#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iomanip>
#include "Item.h"
#include "Utils.h"

using namespace std;

namespace sdds
{
    const int MAX_DESC_LENGTH = 1000;

    // Protected Method
    bool Item::linear() const
    {
        return linearFlag;
    }

    // Construction
    Item::Item()
    {
        price = 0.0;
        quantityOnHand = 0;
        quantityNeeded = 0;
        description = nullptr;
        linearFlag = false;
        sku = 0;
    }

    // Rule of three
    Item::Item(const Item &other) : iProduct(other)
    {
        itemStatus = other.itemStatus;
        sku = other.sku;
        price = other.price;
        quantityOnHand = other.quantityOnHand;
        quantityNeeded = other.quantityNeeded;
        linearFlag = other.linearFlag;
        description = nullptr;

        if (other.description != nullptr)
        {
            char *tempDesc = nullptr;
            ut.alocpy(tempDesc, other.description);
            delete[] description;
            description = tempDesc;
        }
    }

    Item &Item::operator=(const Item &other)
    {
        if (this != &other)
        {
            itemStatus = other.itemStatus;
            sku = other.sku;
            price = other.price;
            quantityOnHand = other.quantityOnHand;
            quantityNeeded = other.quantityNeeded;
            linearFlag = other.linearFlag;

            char *tempDesc = nullptr;
            ut.alocpy(tempDesc, other.description);
            delete[] description;
            description = tempDesc;
        }
        return *this;
    }

    Item::~Item()
    {
        delete[] description;
    }

    // virtual public query iProduct overrides
    int Item::qtyNeeded() const
    {
        return quantityNeeded;
    }

    int Item::qty() const
    {
        return quantityOnHand;
    }

    Item::operator double() const
    {
        return price;
    }

    Item::operator bool() const
    {
        return description != nullptr && itemStatus;
    }

    // virtual public modifier iProduct overrides
    int Item::operator-=(int qty)
    {
        quantityOnHand -= qty;
        return quantityOnHand;
    }

    int Item::operator+=(int qty)
    {
        quantityOnHand += qty;
        return quantityOnHand;
    }

    void Item::linear(bool isLinear)
    {
        linearFlag = isLinear;
    }

    // public modifier
    void Item::clear()
    {
        itemStatus.clear();
    }

    // virtual operator== iProduct overrides
    bool Item::operator==(int sku) const
    {
        return this->sku == sku;
    }

    bool Item::operator==(const char *description) const
    {
        if (this->description == nullptr || description == nullptr)
            return false;

        return strstr(this->description, description) != nullptr;
    }

    // virtual Input/Output method iProduct overrides
    ofstream &Item::save(ofstream &ofstr) const
    {
        if (static_cast<bool>(itemStatus))
        {
            ofstr << sku << '\t' << description << '\t' << quantityOnHand << '\t'
                  << quantityNeeded << '\t' << fixed << setprecision(2) << price;
        }
        return ofstr;
    }

    ifstream &Item::load(ifstream &ifstr)
    {
        char tempDesc[MAX_DESC_LENGTH];

        delete[] description;
        description = nullptr;

        ifstr >> sku; // Read SKU
        ifstr.ignore(1000, '\t');

        ifstr.getline(tempDesc, MAX_DESC_LENGTH, '\t'); // Read description until next tab

        ifstr >> quantityOnHand >> quantityNeeded >> price; // Read remaining data
        ifstr.ignore();

        if (ifstr.fail())
        {
            itemStatus = "Input file stream read failed!";
        }
        else
        {
            ut.alocpy(description, tempDesc);
        }

        return ifstr;
    }

    ostream &Item::display(ostream &ostr) const
    {
        const int MAX_DESC_LENGTH = 35;

        if (!itemStatus)
        {
            ostr << itemStatus;
        }
        else
        {
            if (linearFlag)
            {
                char truncatedDesc[MAX_DESC_LENGTH + 1];

                // Linear format
                ostr << setw(5) << sku << " | ";

                // Safely handle nullptr and truncate if necessary
                if (description)
                {
                    strncpy(truncatedDesc, description, MAX_DESC_LENGTH);
                    truncatedDesc[MAX_DESC_LENGTH] = '\0'; // Ensure null-termination
                }
                else
                {
                    truncatedDesc[0] = '\0'; // Empty string if description is nullptr
                }

                ostr << setw(MAX_DESC_LENGTH) << left << truncatedDesc << right << " | ";
                ostr << setw(4) << quantityOnHand << " | ";
                ostr << setw(4) << quantityNeeded << " | ";
                ostr << setw(7) << fixed << setprecision(2) << price << " |";
            }
            else
            {
                // Descriptive format
                double neededFund = (quantityNeeded - quantityOnHand) * price;

                ostr << "AMA Item:" << endl;
                ostr << sku << ": " << (description ? description : "") << endl; // Safely handle nullptr
                ostr << "Quantity Needed: " << quantityNeeded << endl;
                ostr << "Quantity Available: " << quantityOnHand << endl;
                ostr << "Unit Price: $" << fixed << setprecision(2) << price << endl;
                ostr << "Needed Purchase Fund: $" << fixed << setprecision(2) << neededFund << endl;
            }
        }
        return ostr;
    }

    int Item::readSku(std::istream &istr)
    {
        const int MIN_SKU = 40000;
        const int MAX_SKU = 99999;
        int tempSku;

        cout << "SKU: ";

        while (true)
        {
            istr >> tempSku;
            if (istr.fail())
            {
                istr.clear();
                istr.ignore(1000, '\n');
                cout << "Invalid Integer, retry: ";
            }
            else if (tempSku < MIN_SKU || tempSku > MAX_SKU)
            {
                cout << "Value out of range [" << MIN_SKU << "<=val<=" << MAX_SKU << "]: ";
            }
            else
            {
                sku = tempSku;
                break;
            }
        }
        return sku;
    }

    istream &Item::read(istream &istr)
    {
        const int MIN_QUANTITY_NEEDED = 1;
        const int MAX_QUANTITY_NEEDED = 9999;
        const double MIN_PRICE = 0.0;
        const double MAX_PRICE = 9999.0;

        char tempDescription[MAX_DESC_LENGTH + 1];
        cout << "AMA Item:" << endl;
        cout << "SKU: " << sku << endl;

        cout << "Description: ";
        if (istr.peek() == '\n')
        {
            istr.ignore();
        }
        istr.getline(tempDescription, MAX_DESC_LENGTH);

        cout << "Quantity Needed: ";
        while (!(istr >> quantityNeeded) ||
               quantityNeeded < MIN_QUANTITY_NEEDED ||
               quantityNeeded > MAX_QUANTITY_NEEDED)
        {
            if (istr.fail())
            {
                istr.clear();
                istr.ignore(1000, '\n');
                cout << "Invalid Integer, retry: ";
            }
            else
            {
                cout << "Value out of range [" << MIN_QUANTITY_NEEDED << "<=val<=" << MAX_QUANTITY_NEEDED << "]: ";
                istr.clear();
                istr.ignore(1000, '\n');
            }
        }

        cout << "Quantity On Hand: ";
        while (!(istr >> quantityOnHand) ||
               quantityOnHand < 0 ||
               quantityOnHand > quantityNeeded)
        {
            if (istr.fail())
            {
                istr.clear();
                istr.ignore(1000, '\n');
                cout << "Invalid Integer, retry: ";
            }
            else
            {
                cout << "Value out of range [0<=val<=" << quantityNeeded << "]: ";
                istr.clear();
                istr.ignore(1000, '\n');
            }
        }

        cout << "Unit Price: $";
        while (!(istr >> price) ||
               price < MIN_PRICE ||
               price > MAX_PRICE)
        {
            if (istr.fail())
            {
                istr.clear();
                istr.ignore(1000, '\n');
                cout << "Invalid number, retry: ";
            }
            else
            {
                cout.setf(ios::fixed);
                cout.precision(2);
                cout << "Value out of range [" << MIN_PRICE << "<=val<=" << MAX_PRICE << "]: ";
                istr.clear();
                istr.ignore(1000, '\n');
            }
        }

        if (istr.fail())
        {
            itemStatus = "Console entry failed!";
            istr.clear();
        }
        else
        {
            ut.alocpy(description, tempDescription);
        }

        return istr;
    }

}