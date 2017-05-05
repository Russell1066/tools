#include "stdafx.h"

#include <ctype.h>

namespace
{
    class CalculatorEngine
    {
    private:
        struct node
        {
            enum type
            {
                Value,
                Add = '+',
                Subtract = '-',
                Multiply = '*',
                Divide = '/',
            };

            type  nodeType;
            double value;
            node* pLhs;
            node* pRhs;

            node(type nodeType = Value, double value = 0) :
                nodeType(nodeType),
                value(value),
                pLhs(nullptr),
                pRhs(nullptr)
            {
            }

            double Evaluate() const
            {
                switch (nodeType)
                {
                case Value:
                    assert(pLhs == nullptr && pRhs == nullptr);
                    return value;

                case Add:
                    EnsureSubTrees();
                    return pLhs->Evaluate() + pRhs->Evaluate();

                case Subtract:
                    EnsureSubTrees();
                    return pLhs->Evaluate() - pRhs->Evaluate();

                case Multiply:
                    EnsureSubTrees();
                    return pLhs->Evaluate() * pRhs->Evaluate();

                case Divide:
                    EnsureSubTrees();
                    // Can throw if pRhs->Evaluate() == 0;
                    return pLhs->Evaluate() / pRhs->Evaluate();

                default:
                    throw "unknown nodeType";
                }
            }

            void EnsureSubTrees() const
            {
                if (pLhs == nullptr)
                {
                    throw "missing left hand side";
                }

                if (pRhs == nullptr)
                {
                    throw "missing right hand side";
                }
            }
        };

        static void DestroyTree(node* pNode)
        {
            if (pNode == nullptr)
            {
                return;
            }

            DestroyTree(pNode->pLhs);
            DestroyTree(pNode->pRhs);

            delete pNode;
        }

        void AddDigit(int d)
        {
            assert(d >= 0 && d <= 9);
            if (pCurrent->nodeType != node::Value)
            {
                throw "malformed expression";
            }

            pCurrent->value = pCurrent->value * 10 + d;
        }

        static bool IsOperator(char c)
        {
            switch (c)
            {
            case '+':
            case '-':
            case '*':
            case '/':
                return true;
            }

            return false;
        }

        static void DisplayOperator(node* pNode)
        {
            printf("(");

            DisplayExpression(pNode->pLhs);

            printf(" %c ", static_cast<char>(pNode->nodeType));

            DisplayExpression(pNode->pRhs);

            printf(")");
        }

        static void DisplayExpression(node* pNode)
        {
            if (pNode == nullptr)
            {
                return;
            }

            if (IsOperator(static_cast<char>(pNode->nodeType)))
            {
                DisplayOperator(pNode);
            }
            else
            {
                assert(pNode->pLhs == nullptr && pNode->pRhs == nullptr);
                printf("%d", static_cast<int>(pNode->value));
            }
        }

        void DisplayExpression()
        {
            DisplayExpression(pRoot);
            printf("\n");
        }

        void AddOperator(char c)
        {
            node* pNewNode = new node(static_cast<node::type>(c));

            switch (c)
            {
            case '+':
            case '-':
                pNewNode->pLhs = pRoot;
                pNewNode->pRhs = new node();
                pRoot = pNewNode;
                pCurrent = pRoot->pRhs;
                break;

            case '*':
            case '/':
                pNewNode->pLhs = pRoot->pRhs;
                pRoot->pRhs = pNewNode;
                pNewNode->pRhs = new node();
                pCurrent = pNewNode->pRhs;
                break;

            default:
                throw "unknown operator";
            }
        }

    public:
        CalculatorEngine() : pRoot(new node()), pCurrent(pRoot) {}
        ~CalculatorEngine()
        {
            DestroyTree(pRoot);
        }

        void AddCharacter(char c)
        {
            if (isdigit(c))
            {
                AddDigit(c - '0');
                DisplayExpression();
            }
            else if (IsOperator(c))
            {
                AddOperator(c);
                DisplayExpression();
            }
            else if (!iswspace(c))
            {
                throw "unexpected character";
            }
        }

        double Evaluate()
        {
            return pRoot->Evaluate();
        }

    private:
        node* pRoot;
        node* pCurrent;
    };

    double Calculate(char const* pExpression)
    {
        if (pExpression == nullptr)
        {
            throw "null pointer";
        }

        CalculatorEngine ce;

        for (; *pExpression; ++pExpression)
        {
            ce.AddCharacter(*pExpression);
        }

        return ce.Evaluate();
    }

}

void TestCalculator()
{
    printf("= %f\n", Calculate("47 - 3 * 5 + 1"));
    printf("= %f\n", Calculate("47 + 3 + 5 / 2 * 6 - 1"));
}

