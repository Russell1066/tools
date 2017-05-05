#include "stdafx.h"

namespace
{

    struct list
    {
    private:
        struct node
        {
            node(int value, node* pNext = nullptr) : value(value), pNext(pNext) {}

            int value;
            node* pNext;
        };

    public:
        list() : pHead(nullptr), pTail(nullptr) { }

        void AddNode(int value)
        {
            auto pNode = new node(value, pHead);
            pHead = pNode;

            if (pTail == nullptr)
            {
                pTail = pHead;
            }
        }

        static void Splice(node* pLhsEnd, list rhs)
        {
            assert(pLhsEnd != nullptr && pLhsEnd->pNext == nullptr);
            pLhsEnd->pNext = rhs.pHead;
        }

        template<typename T>
        void WalkList(T visit)
        {
            WalkListPointers([&](node * pNode) { visit(pNode->value); });
        }

        bool IsCorrupt()
        {
            if (pHead == nullptr)
            {
                return false;
            }

            node* pFast = pHead;
            for (node* pWalk = pHead; pWalk != nullptr && pFast != nullptr; pWalk = pWalk->pNext)
            {
                pFast = pFast->pNext;
                if (pWalk == pFast)
                {
                    return true;
                }


                if (pFast != nullptr)
                {
                    pFast = pFast->pNext;
                    if (pWalk == pFast)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        node* FindCorruption()
        {
            if (pHead == nullptr)
            {
                return nullptr;
            }

            node* pFast = pHead;
            node* pWalk = pHead;
            for (; pWalk != nullptr && pFast != nullptr; pWalk = pWalk->pNext)
            {
                pFast = pFast->pNext;
                if (pWalk == pFast)
                {
                    break;
                }


                if (pFast != nullptr)
                {
                    pFast = pFast->pNext;
                    if (pWalk == pFast)
                    {
                        break;
                    }
                }
            }

            if (pFast == nullptr)
            {
                return nullptr;
            }

            node* pByCount = pHead;
            pWalk = pWalk->pNext;
            while (pWalk != pFast)
            {
                pWalk = pWalk->pNext;
                pByCount = pByCount->pNext;
            }

            pByCount = pByCount->pNext;
            for (pWalk = pHead; pWalk != pByCount;)
            {
                pByCount = pByCount->pNext;
                pWalk = pWalk->pNext;
            }

            return pWalk;
        }

        void MakeCorrupt()
        {
            pTail->pNext = pHead;
        }

    private:
        template<typename T>
        void WalkListPointers(T visit)
        {
            for (node* pWalk = pHead; pWalk != nullptr; )
            {
                node* pNext = pWalk->pNext;
                visit(pWalk);
                pWalk = pNext;
            }
        }

        node* AddNode(node* pNode)
        {
            assert(pNode != nullptr);
            pNode->pNext = pHead;
            pHead = pNode;

            return pHead;
        }

        node* pHead;
        node* pTail;
    };

    list InitCorruptList()
    {
        int preCorrupt[] = { 5, 4, 3, 8 };
        int postCorrupt[] = { 2, 1, 0 };
        list l;

        for each (auto it in preCorrupt)
        {
            l.AddNode(it);
        }

        l.MakeCorrupt();

        for each (auto it in postCorrupt)
        {
            l.AddNode(it);
        }

        return l;
    }
}

void TestCorruption()
{
    auto l = InitCorruptList();
    auto corrupt = l.FindCorruption();
}

