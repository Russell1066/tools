#include "stdafx.h"

#include<math.h>

unsigned DivideU(unsigned lhs, unsigned rhs)
{
    // Do not allow "negative" values
    //assert((lhs & (1 << 31) == 0) || (lhs == (1 << 31)));

    unsigned retv = 0;
    int msb = 0;

    if (rhs > lhs)
    {
        return retv;
    }

    while (rhs < lhs)
    {
        ++msb;
        rhs <<= 1;
    }

    for (; msb >= 0 && lhs != 0; --msb)
    {
        if (rhs <= lhs)
        {
            lhs -= rhs;
            retv |= 1 << msb;
        }

        rhs >>= 1;
    }

    return retv;
}

int Divide(int lhs, int rhs)
{
    if (rhs == 0)
    {
        throw "divide by zero error";
    }

    if (lhs < rhs)
    {
        return 0;
    }

    bool sL = lhs > 0;
    bool sR = rhs > 0;

    bool sign = !(sL ^ sR);
    unsigned div = DivideU(static_cast<unsigned>(abs(lhs)), static_cast<unsigned>(abs(rhs)));
    return sign ? div : -static_cast<int>(div);
}

void TestDivision()
{
    auto zero = Divide(12, 13);

    auto one = Divide(15, 15);
    auto minusOne = Divide(-15, 15);
    minusOne = Divide(15, -15);
    one = Divide(-15, -15);

    auto three = Divide(22, 7);
    three = Divide(21, 7);
    auto ten = Divide(100, 10);
    ten = Divide(109, 10);
}
