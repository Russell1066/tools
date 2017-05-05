#include "stdafx.h"

#include <map>
#include <string>
#include <set>

using std::string;
using std::set;


bool IsPalindromePermutation(string const& value)
{
    set<string::value_type> items;
    for (auto it = value.cbegin(); it != value.cend(); ++it)
    {
        if (*it == ' ')
        {
            continue;
        }

        auto c = tolower(*it);
        auto exists = items.find(c);
        if (exists == items.end())
        {
            items.insert(c);
        }
        else
        {
            items.erase(exists);
        }
    }

    return items.size() < 2;
}

void TestIsPalindromePermutation()
{

    string test = "Able I was ere I saw Elba";
    assert(IsPalindromePermutation(test) == true);
}

