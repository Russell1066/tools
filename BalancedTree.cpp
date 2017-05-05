#include "stdafx.h"

#include <math.h>
#include <tuple>
#include <queue>

using std::pair;
using std::queue;

namespace
{
    struct node
    {
        int value;
        node* pLhs;
        node* pRhs;

        int Count()
        {
            int lhsCount = 0;
            int rhsCount = 0;

            if (pLhs)
            {
                lhsCount = pLhs->Count();
                if (lhsCount == -1)
                {
                    return -1;
                }
            }

            if (pRhs)
            {
                rhsCount = pRhs->Count();
                if (rhsCount == -1)
                {
                    return -1;
                }
            }

            if (abs(rhsCount - lhsCount) > 1)
            {
                return -1;
            }

            return rhsCount + lhsCount + 1;
        }

        bool IsBalancedSimple()
        {
            int lhs = pLhs->Count();
            if (lhs == -1)
            {
                return false;

            }

            int rhs = pRhs->Count();

            if (rhs == -1)
            {
                return false;
            }

            int diff = abs(lhs - rhs);

            return diff <= 1;
        }
    };
}

