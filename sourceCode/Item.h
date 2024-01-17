#ifndef SDDS_ITEM_H
#define SDDS_ITEM_H
#include "iProduct.h"
#include "Status.h"

namespace sdds
{
    class Item : public iProduct
    {
        double price;
        int quantityOnHand;
        int quantityNeeded;
        char *description;
        bool linearFlag;

    protected:
        Status itemStatus;
        int sku;
        bool linear() const;

    public:
        // Construction
        Item();

        // Rule of three
        Item(const Item &other);
        Item &operator=(const Item &other);
        virtual ~Item();

        // virtual public query iProduct overrides
        virtual int qtyNeeded() const;
        virtual int qty() const;
        virtual operator double() const;
        virtual operator bool() const;

        // virtual public modifier iProduct overrides
        virtual int operator-=(int qty);
        virtual int operator+=(int qty);
        virtual void linear(bool isLinear);

        // public modifier
        void clear();

        // virtual operator== iProduct overrides
        virtual bool operator==(int sku) const;
        virtual bool operator==(const char *description) const;

        // virtual Input/Output method iProduct overrides
        virtual std::ofstream &save(std::ofstream &ofstr) const;
        virtual std::ifstream &load(std::ifstream &ifstr);
        virtual std::ostream &display(std::ostream &ostr) const;
        virtual int readSku(std::istream &istr);
        virtual std::istream &read(std::istream &istr);
    };
}

#endif // !SDDS_ITEM_H
