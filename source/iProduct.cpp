#include "iProduct.h"

using namespace std;

namespace sdds
{
    ostream &operator<<(ostream &ostr, const iProduct &product)
    {
        return product.display(ostr);
    }

    istream &operator>>(istream &istr, iProduct &product)
    {
        return product.read(istr);
    }
}