#include "stdafx.h"
#include <math.h>
#include <map>
#include <vector>

using std::vector;
using std::map;
using std::pair;

void findPairs()
{
    const int maxN = 100;

    map<__int64, vector<pair<int, int>>> outputs;

    for (auto a = 1; a <= maxN; ++a)
    {
        __int64 a3 = a * a * static_cast<__int64>(a);
        for (auto b = a; b <= maxN; ++b)
        {
            __int64 b3 = b * b * static_cast<__int64>(b);
            __int64 a3b3 = a3 + b3;

            outputs[a3b3].push_back(pair<int, int>(a, b));
        }
    }

    for (auto mapIt = outputs.begin(); mapIt != outputs.end(); ++mapIt)
    {
        auto value = mapIt->first;
        auto pairList = mapIt->second;

        //if (pairList.size() < 3)
        //{
        //    continue;
        //}

        printf("%lld = ", value);

        for (auto it = pairList.begin(); it != pairList.end(); ++it)
        {
            printf("(%d, %d) ", it->first, it->second);
        }

        printf("\n");
    }
}
