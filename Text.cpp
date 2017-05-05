#include "stdafx.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm> 

using namespace std;

namespace
{

const char gettysburgText[] =
"Four score and seven years ago our fathers brought forth on this continent "
"a new nation, conceived in liberty, and dedicated to the proposition that "
"all men are created equal. "
"Now we are engaged in a great civil war, testing whether that nation, or "
"any nation so conceived and so dedicated, can long endure.We are met on a "
"great battlefield of that war.We have come to dedicate a portion of that "
"field, as a final resting place for those who here gave their lives that "
"that nation might live.It is altogether fitting and proper that we should "
"do this. "
"But, in a larger sense, we can not dedicate, we can not consecrate, we can "
"not hallow this ground.The brave men, living and dead, who struggled here, "
"have consecrated it, far above our poor power to add or detract.The world "
"will little note, nor long remember what we say here, but it can never "
"forget what they did here.It is for us the living, rather, to be dedicated "
"here to the unfinished work which they who fought here have thus far so "
"nobly advanced.It is rather for us to be here dedicated to the great task "
"remaining before us—that from these honored dead we take increased devotion "
"to that cause for which they gave the last full measure of devotion—that we "
"here highly resolve that these dead shall not have died in vain—that this "
"nation, under God, shall have a new birth of freedom—and that government of "
"the people, by the people, for the people, shall not perish from the earth.";



typedef map<string, vector<int>> wordList;
typedef wordList::mapped_type wordLocations;
typedef map<string, int> wordPairDistance;
class Words
{
public:
    void ReadText(string const& text)
    {
        string currentWord;
        int wordNumber = 0;
        for (auto it = text.cbegin(); it != text.cend(); ++it)
        {
            unsigned char c = static_cast<unsigned char>(*it);
            auto isAlpha = isalpha(c);
            // Found a word start?
            if (isAlpha)
            {
                currentWord.push_back(tolower(c));
                continue;
            }

            // Found a word end?
            if (!isAlpha && currentWord.length() > 0)
            {
                m_words[currentWord].push_back(wordNumber);

                // next word
                ++wordNumber;

                // reset the start
                currentWord.clear();
            }
        }
    }

    int FindDistance(string const& lhsWord, string const& rhsWord)
    {
        auto lhsIt = m_words.find(ToLower(lhsWord));
        auto rhsIt = m_words.find(ToLower(rhsWord));

        if (lhsIt == m_words.end() || rhsIt == m_words.end())
        {
            // Pick a strategy, may depend on the house
            //return -1;
            throw "word not found";
        }

        if (lhsIt == rhsIt)
        {
            return 0;
        }

        if (lhsIt->second.size() == 1 && rhsIt->second.size() == 1)
        {
            return abs(lhsIt->second[0] - rhsIt->second[0]);
        }

        if (lhsIt->second.size() == 1)
        {
            // Do a binary search
            return FindDistance(lhsIt->second[0], rhsIt->second);
        }

        if (rhsIt->second.size() == 1)
        {
            // Do a binary search
            return FindDistance(rhsIt->second[0], lhsIt->second);
        }

        return FindDistance(lhsIt->second, rhsIt->second);
    }

private:
    int FindDistance(int lhs, wordLocations rhs)
    {
        if (rhs.size() < 3)
        {
            vector<int> dummy;
            dummy.push_back(lhs);
            return FindDistance(dummy, rhs);
        }

        unsigned index = rhs.size() / 2;
        int step = index / 2;
        int retv = abs(lhs - rhs[index]);

        while (index > 0 && index < rhs.size() && step > 0)
        {
            retv = min(retv, abs(lhs - rhs[index]));
            if (lhs > rhs[index])
            {
                index += step;
            }
            else
            {
                index -= step;
            }

            step /= 2;
        }

        // I don't think this gets the edge cases - we should check
        auto bounds = min(abs(lhs - rhs[index - 1]), abs(lhs - rhs[index + 1]));
        auto center = min(retv, abs(lhs - rhs[index]));
        return min(bounds, center);
    }

    int FindDistance(wordLocations const& lhs, wordLocations const& rhs)
    {
        auto itL = lhs.cbegin();
        auto itR = rhs.cbegin();
        int distance = INT_MAX;
        while (itL != lhs.cend() && itR != rhs.cend())
        {
            distance = min(distance, abs(*itL - *itR));
            if (*itL < *itR)
            {
                ++itL;
            }
            else
            {
                ++itR;
            }
        }

        return distance;
    }

    static string ToLower(string const& in)
    {
        auto out = string(in);
        for (auto it = out.begin(); it != out.end(); ++it)
        {
            *it = tolower(*it);
        }

        return out;
    }

private:
    wordList m_words;
};

}

void TestWordDistance()
{
    Words w;
    w.ReadText(gettysburgText);
    w.FindDistance("A", "a");
    w.FindDistance("above", "a");
    //w.FindDistance("above", "a");
}