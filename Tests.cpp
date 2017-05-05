// Tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <map>
#include <vector>
#include <limits>

#include "Tests.h"

using std::map;
using std::vector;

typedef map<unsigned, unsigned char> histogram;
const unsigned char MAXFREQUENCY = std::numeric_limits<unsigned char>::max();
const unsigned MAXVALUE = 5000000;
const unsigned NUMVALUES = 1000000;
const unsigned MAXRAND = (1 << 30) - 1;
typedef void(*rawFunction)(unsigned, unsigned char);

// Problem
//      RAND_MAX is 32767, too small for a random number [1, MAXVALUE]
// Solution
//      since we have 15 bits of random, make it into 30

// Solve problem of the generic random numbers not having enough bits
unsigned GetRandom(unsigned maxValue)
{
    long long lowBits = rand();
    long long highBits = rand() << 15;

    return static_cast<unsigned>((highBits | lowBits) * maxValue / MAXRAND + 1);
}

// A simple placeholder to allow code reuse
static void DoNothing(unsigned, unsigned char) {}

// code to generate random number histograms
template<typename T = rawFunction>
histogram CreateRandomNumberHistogram(unsigned numberCount, unsigned maxValue, T func = DoNothing)
{
    histogram values;

    for (unsigned i = 0; i < numberCount; ++i)
    {
        unsigned value = GetRandom(maxValue);
        unsigned char& ref = values[value];

        // We've assumed that the data fits into a char
        if (ref == MAXFREQUENCY)
        {
            throw "Frequency is too high to be counted - change to a larger data type";
        }

        ref += 1;

        func(value, ref);
    }

    return values;
}

// Use the generic CreateRandomNumberHistogram to find the most common values
vector<unsigned> GetMostFrequentRandomNumbers(int numberCount, int maxValue)
{
    vector<unsigned> topValues;
    unsigned maxFrequency = 2;  // If we use 1 or zero, we will fill up the array, even though it is 
                                // unlikely that all of the elements found are equally common
                                // there is a correction for this at the end if needed

    // Use a lambda to keep track of the most common values
    auto values = CreateRandomNumberHistogram(numberCount, maxValue, [&](unsigned value, unsigned char count) {
        if (count > maxFrequency)
        {
            topValues.clear();
            maxFrequency = count;
        }

        if (count == maxFrequency)
        {
            topValues.push_back(value);
        }
    });

    if (topValues.size() == 0)
    {
        for each (auto value in values)
        {
            topValues.push_back(value.first);
        }
    }

    return topValues;
}

// Write a function that that uses a standard random number generator to 
// compute 1,000,000 random numbers between 1 and 5,000,000 and returns the 
// total number of unique random numbers found
int GetRandomNumberCount()
{
    return CreateRandomNumberHistogram(NUMVALUES, MAXVALUE).size();
}

// Write a function similar to the first but returns the top repeated value 
// (or values if there’s a tie).
vector<unsigned> GetMostFrequentRandomNumbers()
{
    return GetMostFrequentRandomNumbers(NUMVALUES, MAXVALUE);
}

int main()
{
    int count = GetRandomNumberCount();
    auto list = GetMostFrequentRandomNumbers();

    return 0;
}

