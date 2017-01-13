#pragma once
#include <deque>
#include <functional>
#include <map>
#include <stack>
#include <vector>

template<typename keyType = int, keyType invalidNodeValue = -1>
class BreadthFirstSearch
{
private:

    struct Node
    {
        static const keyType invalidNode = invalidNodeValue;

        Node(keyType id = invalidNode) : parent(invalidNode), id(id) {}

        keyType parent;
        keyType id;
    };

public:
    typedef std::function<std::vector<keyType>(keyType id)> GetChildrenFn;

private:
    typedef std::deque<Node> nodeQueue;
    typedef std::map<keyType, Node> nodeMap;
    typedef std::deque<keyType> nodePath;

public:
    static nodePath FindPath(keyType start, keyType end, GetChildrenFn getChildren)
    {
        BreadthFirstSearch bfs;

        return bfs.FindPathImpl(start, end, getChildren);
    }

private:
    // Does a breadth first search
    // Returns the path from start to end on a stack where start is at the top of the stack
    nodePath FindPathImpl(keyType start, keyType end, GetChildrenFn getChildren)
    {
        nodePath retv;

        // Check for the trivial case that your path consists merely of starting
        if (start == end)
        {
            retv.push_front(start);
            return retv;
        }

        nodeQueue queue;
        Node& startNode = GetNode(start);
        startNode.parent = start;
        queue.push_back(startNode);

        while (queue.size() > 0)
        {
            auto node = queue.front();
            queue.pop_front();
            auto children = getChildren(node.id);

            for each (auto child in children)
            {
                Node& childNode = GetNode(child);

                // If the node has already been visited then the parent node will have been set
                // if some degenerate has decided to tell you that a node is a child of itself
                // then don't add that node either. No shortest path will ever pass through the
                // same node twice.
                if (childNode.parent != Node::invalidNode || childNode.id == node.id)
                {
                    continue;
                }

                childNode.parent = node.id;

                if (child == end)
                {
                    return GetPath(childNode, start);
                }

                queue.push_back(childNode);
            }
        }

        return retv;
    }

    const nodePath GetPath(Node childNode, keyType start)
    {
        nodePath retv;

        // The start node is the parent of itself
        while (childNode.parent != childNode.id)
        {
            retv.push_front(childNode.id);
            childNode = GetNode(childNode.parent);
        }

        assert(childNode.id == start);
        retv.push_front(childNode.id);

        return retv;
    }

    void CleanupNodes()
    {
        nodes.clear();
    }

    Node& GetNode(keyType start)
    {
        if (nodes.find(start) == nodes.end())
        {
            nodes[start] = Node(start);
        }

        return nodes[start];
    }

private:
    nodeMap nodes;
};
