#include "stdafx.h"

#include <stdlib.h>
#include <deque>
#include <vector>

using std::deque;
using std::vector;

#include <stdlib.h>
#include <deque>
#include <vector>

using std::deque;
using std::vector;

namespace
{
    struct treeNode
    {
        int value;
        treeNode* pLhs;
        treeNode* pRhs;

        treeNode(int value = 0) : value(value), pLhs(nullptr), pRhs(nullptr) {}
    };

    struct levelNode
    {
        treeNode* pNode;
        int       level;

        levelNode(treeNode* pNode, int level) : pNode(pNode), level(level) {}
    };

    treeNode* BinaryVisit(int* pArray, int size)
    {
        int center = size / 2;
        treeNode* pNode = new treeNode(pArray[center]);
        if (center > 0)
        {
            pNode->pLhs = BinaryVisit(pArray, center);
        }

        if (size - center > 1)
        {
            pNode->pRhs = BinaryVisit(pArray + center + 1, size - center - 1);
        }

        return pNode;
    }

    void TestBinarySearchTree()
    {
        int testArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        treeNode* pRoot = BinaryVisit(testArray, _countof(testArray));
    }
}

void TestLevels()
{
    int testArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    treeNode* pRoot = BinaryVisit(testArray, _countof(testArray));
    vector<vector<treeNode*>> levelLists;

    deque<levelNode> visitList;
    visitList.push_back(levelNode(pRoot, 1));
    while (visitList.size() > 0)
    {
        levelNode item = visitList.front();
        visitList.pop_front();
        if (item.pNode->pLhs != nullptr)
        {
            visitList.push_back(levelNode(item.pNode->pLhs, item.level + 1));
        }

        if (item.pNode->pRhs != nullptr)
        {
            visitList.push_back(levelNode(item.pNode->pRhs, item.level + 1));
        }

        if (levelLists.size() < static_cast<unsigned>(item.level))
        {
            levelLists.push_back(vector<treeNode*>());
        }

        levelLists[item.level - 1].push_back(item.pNode);
    }

}

