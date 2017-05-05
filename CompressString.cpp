#include "stdafx.h"

#include <assert.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

template<typename T>
int charCounter(T it, T end)
{
    char toCount = *it;

    assert(tolower(toCount) >= 'a' && toCount <= 'z');

    int count = 1;
    for (++it; it != end && *it == toCount; ++count, ++it)
    {
        ;
    }

    return count;
}

string ConvertPositiveInteger(int toConvert)
{
    vector<char> tmp;
    assert(toConvert > 0);
    for (; toConvert > 0; toConvert /= 10)
    {
        tmp.push_back(toConvert % 10 + '0');
    }

    string output;
    for (auto it = tmp.rbegin(); it != tmp.rend(); ++it)
    {
        output.push_back(*it);
    }

    return output;
}

string CompressString(string const& input)
{
    string output;
    int spaceRemaining = input.length();

    auto it = input.begin();
    for (; it != input.end() && spaceRemaining > 1;)
    {
        output.push_back(*it);
        int run = charCounter(it, input.end());
        string countString = ConvertPositiveInteger(run);
        it += run;
        spaceRemaining -= countString.length() + 1;
        output += countString;
    }

    if (it != input.end())
    {
        return input;
    }

    return output;
}

void TestCompressString()
{
    char* inputs[] = { "","aa", "abc", "abbccc", "aaaaaaaaaabcdefgg" };
    char* outputs[] = { "", "a2", "abc", "a1b2c3", "a10b1c1d1e1f1g2" };

    for (int i = 0; i < _countof(inputs); ++i)
    {
        auto test = CompressString(inputs[i]);
        printf("%s -> %s expected %s\n", inputs[i], test.c_str(), outputs[i]);
        assert(test == outputs[i]);
    }
}

