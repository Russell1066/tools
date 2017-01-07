#pragma once
#include <deque>
#include <functional>
#include <map>
#include <stack>
#include <vector>

template<typename keyType = int, keyType invalidNodeValue = -1>
class BreadthFirstSearchBiDirectional
{
private:

	struct Node
	{
		static const keyType invalidNode = invalidNodeValue;

		Node(keyType id = invalidNode) : start(invalidNode), parent(invalidNode), id(id) {}

		keyType start;
		keyType parent;
		keyType id;
	};

public:
	typedef std::function<std::vector<keyType>(keyType id)> GetChildrenFn;

private:
	typedef std::deque<Node> nodeQueue;
	typedef std::map<keyType, Node> nodeMap;


public:
	static std::deque<keyType> FindPath(keyType start, keyType end, GetChildrenFn getChildren)
	{
		BreadthFirstSearchBiDirectional bfs;

		return bfs.FindPathImpl(start, end, getChildren);
	}

private:
	// Does a breadth first search
	// Returns the path from start to end on a stack where start is at the top of the stack
	std::deque<keyType> FindPathImpl(keyType start, keyType end, GetChildrenFn getChildren)
	{
		std::deque<keyType> retv;

		// Check for the trivial case that your path consists merely of starting
		if (start == end)
		{
			retv.push_back(start);
			return retv;
		}

		nodeQueue startQueue = InitStartNode(start);
		nodeQueue endQueue = InitStartNode(end);

		while (startQueue.size() > 0 && endQueue.size() > 0)
		{
			retv = VisitNode(getChildren, start, end, startQueue, true);

			if (retv.size() > 0)
			{
				return retv;
			}

			retv = VisitNode(getChildren, end, start, endQueue, false);
			if (retv.size() > 0)
			{
				return retv;
			}
		}

		return retv;
	}

	std::deque<keyType> VisitNode(GetChildrenFn &getChildren, keyType const &start, keyType const &end, std::deque<Node>& queue, bool isStart)
	{
		std::deque<keyType> retv;

		auto node = queue.front();
		queue.pop_front();

		for each (auto child in getChildren(node.id))
		{
			Node& childNode = GetNode(child);

			// If the nodes have met, we are done
			if (childNode.start == end)
			{
				GetPath(node, start, retv, isStart);
				GetPath(childNode, end, retv, !isStart);
				return retv;
			}

			// If the node has already been visited then the parent node will have been set
			// if some degenerate has decided to tell you that a node is a child of itself
			// then don't add that node either. No shortest path will ever pass through the
			// same node twice.
			if (childNode.parent != Node::invalidNode || childNode.id == node.id)
			{
				continue;
			}

			childNode.parent = node.id;
			childNode.start = node.start;

			queue.push_back(childNode);
		}

		return retv;
	}

	std::deque<Node> InitStartNode(keyType &start)
	{
		Node& startNode = GetNode(start);
		startNode.parent = start;
		startNode.start = start;

		std::deque<Node> queue;
		queue.push_back(startNode);
		return queue;
	}

	const void GetPath(Node childNode, keyType start, std::deque<keyType>& retv, bool pushFront)
	{
		// The start node is the parent of itself
		while (childNode.parent != childNode.id)
		{
			assert(childNode.parent != Node::invalidNode);

			PushNode(retv, childNode, pushFront);
			childNode = GetNode(childNode.parent);
		}

		assert(childNode.id == start);
		PushNode(retv, childNode, pushFront);
	}

	void PushNode(std::deque<keyType> & retv, Node &childNode, bool pushFront)
	{
		if (pushFront)
		{
			retv.push_front(childNode.id);
		}
		else
		{
			retv.push_back(childNode.id);
		}
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
