#include "stdafx.h"

#include "Tests.h"

#include <random>
#include <vector>

using std::vector;

namespace
{
    class Card
    {
    public:
        enum Suit
        {
            Spade = 0,
            Heart = 1,
            Club = 2,
            Diamond = 3,
            Special
        };

        enum Color
        {
            Black = 0,
            Red = 1,
            None
        };

        enum Name
        {
            Ace,
            Two,
            Three,
            Four,
            Five,
            Six,
            Seven,
            Eight,
            Nine,
            Ten,
            Jack,
            Queen,
            King,
            LittleJoker, // Unused in this implementation
            BigJoker,    // Unused in this implementation
        };

        static const int numCards = 52;

        Card(int number = 0) : m_cardNumber(number)
        {
            assert(m_cardNumber >= 0 && m_cardNumber < numCards);
        }

        Color GetColor() const
        {
            return static_cast<Color>(GetSuit() % 2);
        }

        Suit GetSuit() const
        {
            return static_cast<Suit>(m_cardNumber / 13);
        }

        Name GetCardName() const
        {
            return static_cast<Name>(m_cardNumber % 13);
        }

    private:
        int m_cardNumber;
    };

    vector<Card> GetNewDeck()
    {
        vector<Card> deck;
        for (int i = 0; i < Card::numCards; ++i)
        {
            deck.push_back(Card(i));
        }

        return deck;
    }

    vector<Card> ShuffleDeck()
    {
        vector<Card> deck = GetNewDeck();

        int i = 0;
        for (auto it = deck.begin(); it < deck.end() - 1; ++it, ++i)
        {
            auto cardIndex = std::rand() % (Card::numCards - i);
            std::swap(*it, *(it + cardIndex));
        }

        return deck;
    }
}

void TestShuffle()
{
    auto cards = ShuffleDeck();
}

