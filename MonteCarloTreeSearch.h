//
// Created by 陈齐斌 on 15/05/2017.
//

#ifndef STRATEGY_MONTECARLOTREESEARCH_H
#define STRATEGY_MONTECARLOTREESEARCH_H

class MonteCarloSearchState
{
public:
	MonteCarloSearchState()
	{}

	virtual ~MonteCarloSearchState()
	{}

	virtual int DefaultPolicy() = 0;

	virtual MonteCarloSearchState *NextChildState() = 0;

	virtual void Print() = 0;
};

class MonteCarloSearchTreeNode
{
protected:
	MonteCarloSearchTreeNode *firstChild, *nextSibling, *parent;
	MonteCarloSearchState *state;
	int value, times;

public:
    MonteCarloSearchTreeNode(MonteCarloSearchState *state, MonteCarloSearchTreeNode *parent) : state(state), parent(parent), value(0), times(0)
    {
	    firstChild = nextSibling = nullptr;
    }
    virtual ~MonteCarloSearchTreeNode()
    {
	    for (MonteCarloSearchTreeNode *child = firstChild, *next; child != nullptr; child = next)
	    {
		    next = child->nextSibling;
		    delete child;
	    }
	    delete state;
    }

	int GetValue() const { return value; }

	int GetTimes() const { return times; }

	void AddValue(const int delta) { value += delta; times++; }

	virtual MonteCarloSearchTreeNode *Expand() = 0;

	MonteCarloSearchTreeNode *GetFirstChild() const
	{
		return firstChild;
	}

	MonteCarloSearchTreeNode *GetNextSibling() const
	{
		return nextSibling;
	}

	MonteCarloSearchTreeNode *GetParent() const
	{
		return parent;
	}

	MonteCarloSearchState *GetState() const
	{
		return state;
	}

	void SetFirstChild(MonteCarloSearchTreeNode *firstChild)
	{
		MonteCarloSearchTreeNode::firstChild = firstChild;
	}

	void SetNextSibling(MonteCarloSearchTreeNode *nextSibling)
	{
		MonteCarloSearchTreeNode::nextSibling = nextSibling;
	}

};

class MonteCarloSearchTree
{
protected:
	MonteCarloSearchTreeNode *root;

public:
	MonteCarloSearchTree(MonteCarloSearchTreeNode *root) : root(root)
	{}

	virtual ~MonteCarloSearchTree()
	{
		delete root;
	}

	virtual MonteCarloSearchTreeNode *TreePolicy() = 0;

//	virtual void BackTrace() = 0;
};




#endif //STRATEGY_MONTECARLOTREESEARCH_H
