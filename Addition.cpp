#include "stdafx.h"

namespace
{
    int Adc(bool bitCarry, bool bitLhs, bool bitRhs)
    {
        // Seemingly eight possible outcomes, actually only 4
        // Truth table
        // c l r   c b
        // 0 0 0 | 0 0 | 0
        // 0 0 1 | 0 1 | 1
        // 0 1 0 | 0 1 | 1
        // 0 1 1 | 1 0 | 2
        // 1 0 0 | 0 1 | 1
        // 1 0 1 | 1 0 | 2
        // 1 1 0 | 1 0 | 2
        // 1 1 1 | 1 1 | 3

        // if we don't approve of indexing
        // c = bitLhs ^ bitRhs ^ bitCarry
        // ((bitLhs && bitRhs) || (bitLhs && bitCarry) || (bitRhs && bitCarry))
        // (bitLhs && bitRhs) || (bitCarry && (bitLhs || bitRhs))

        // Now, if we are lazy about our math we can cheat
        static int retv[] = { 0, 1, 1, 2, 1, 2, 2, 3 };
        int carry = bitCarry ? 1 : 0;
        int lhs = bitLhs ? 1 : 0;
        int rhs = bitRhs ? 1 : 0;
        int index = (((carry << 1) | lhs) << 1) | rhs;

        return retv[index];
    }

    unsigned Add1(unsigned lhs, unsigned rhs)
    {
        // If we take the lsb of lhs & rhs and do an adc, that gives us the lsb and the carry of the sum
        // using a mask, we can do this for each bit until we are done

        bool carry = false;
        unsigned sum = 0;
        unsigned mask = 1;
        for (; mask != 0 && !(lhs == 0 && rhs == 0); mask <<= 1)
        {
            int adc = Adc(carry, static_cast<bool>(lhs & mask), static_cast<bool>(rhs & mask));
            sum |= (adc & 1) ? mask : 0;
            carry = (adc & 2) ? 1 : 0;
            lhs &= ~mask; // Strip off this bit
            rhs &= ~mask;
        }

        if (carry && (!mask))
        {
            throw "Hey, overflow happened here!";
        }

        // If we have hit overflow this will just leave the carry
        // So our value fails if we hit overflow
        // Because we used unsigned this will also overflow into negative
        sum |= carry ? mask : 0;

        return sum;
    }

    // this is not strictly better
    // Carry propagation may take just as long
    unsigned Add(unsigned lhs, unsigned rhs)
    {
        static const unsigned invalidCarry = 1 << 31;
        unsigned sum = lhs;
        unsigned carry = rhs;

        while (carry != 0)
        {
            lhs = sum;
            rhs = carry;

            sum = lhs ^ rhs;
            carry = (lhs & rhs);

            if (carry & invalidCarry)
            {
                throw "overflow!";
            }

            carry <<= 1;
        }

        return sum;
    }
}

void TestAdd()
{
    auto sum = Add(0, 0);
    //sum = Add(1, 0);
    //sum = Add(7, 1);
    //sum = Add(10, 1);
    sum = Add(static_cast<unsigned>(-1), 1);
}
