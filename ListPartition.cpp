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
        list() : pHead(nullptr) { }

        void AddNode(int value)
        {
            auto pNode = new node(value, pHead);
            pHead = pNode;
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

        list PartitionList(int partition)
        {
            list lt;
            list ge;
            list::node* pltTail = nullptr;

            WalkListPointers([&](list::node* pNode) {
                if (pNode->value < partition)
                {
                    auto pNew = lt.AddNode(pNode);
                    if (pltTail == nullptr)
                    {
                        pltTail = pNew;
                    }
                }
                else
                {
                    ge.AddNode(pNode);
                }
            });

            if (pltTail == nullptr)
            {
                return ge;
            }

            list::Splice(pltTail, ge);

            return lt;
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
    };

    void PrintList(list l)
    {
        l.WalkList([](int value) {
            printf("%d ", value);
        });

        printf("\n");
    }

    list InitTestPartition()
    {
        list l;
        int values[] = { 6, 3, 1, 7, 5 };

        for each (auto it in values)
        {
            l.AddNode(it);
        }

        return l;
    }

    list PartitionList(list l, int partition)
    {
        return l.PartitionList(partition);
    }
}

void TestPartition()
{
    auto l = InitTestPartition();
    PrintList(l);
    auto output = PartitionList(l, 5);
    PrintList(output);

    PrintList(PartitionList(InitTestPartition(), 0));
    PrintList(PartitionList(InitTestPartition(), 50));
}
