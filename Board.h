//
//  Board.hpp
//  Strategy
//
//  Created by 陈齐斌 on 14/05/2017.
//  Copyright © 2017 Yongfeng Zhang. All rights reserved.
//

#ifndef STRATEGY_BOARD_H
#define STRATEGY_BOARD_H

#include <cstring>
#include <iostream>
#include "Judge.h"
#include "MonteCarloTreeSearch.h"

#define TIME_LIMIT 1000000
#define INF 1000000000
#define BOARD_SIZE 12
#define NODE_AMOUNT 2000003
#define USER 0
#define MACHINE 1

class BoardState : public MonteCarloSearchState
{
    u_short *chessState, *top;
    int numberOfChildStates, nextChildState;
	std::pair<int, int> putLocation;
//    player 表示当前轮到谁落子
    bool player, isTerminate;
//    , hash;
    /*
    void SetHash()
    {
        int h1 = 0;
        for (int j = 0; j < BOARD_SIZE; ++j)
            h1 ^= chessState[j];
        int h2 = 0;
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            int t = 0;
            for (int i = 0; i < BOARD_SIZE; ++i)
                t ^= ((chessState[j] & (1 << i)) > 0);
            h2 += t << j;
        }
        hash = (h1 << BOARD_SIZE) + h2;
    }
     */

    void SetNumberOfChildStates()
    {
        numberOfChildStates = nextChildState = 0;
	    int **board = this->ToBoard();
//        若当前0落子，说明上一步1落了子，1有可能赢
	    if ((!player && machineWin(BoardHeight - putLocation.first - 1, putLocation.second, BoardHeight, BoardWidth, board))
	        || (player && userWin(BoardHeight - putLocation.first - 1, putLocation.second, BoardHeight, BoardWidth, board)))
		    ;
	    else
	    {
		    for (int i = 0; i < BoardWidth; ++i)
			    if (top[i] < BoardHeight && !(top[i] == noX && i == noY && noX == BoardHeight - 1))
				    numberOfChildStates++;
	    }
	    
//	    delete board;
    }

    std::pair<int, int> RandomPut()
    {
        int cnt = 0;
        for (int i = 0; i < BoardWidth; ++i)
            if (top[i] < BoardHeight && !(top[i] == noX && i == noY && noX == BoardHeight - 1))
                cnt++;

        if (cnt == 0)
            return std::make_pair(-1, -1);

        cnt = rand() % cnt;

        for (int i = 0; i < BoardWidth; ++i)
            if (top[i] < BoardHeight && !(top[i] == noX && i == noY && noX == BoardHeight - 1))
            {
                if (cnt-- == 0)
                {
	                return (!(top[i] == noX && i == noY)) ? std::make_pair((int) top[i], i) : std::make_pair(
			                (int) top[i] + 1, i);

                }
            }
        std::cout << "RANDOM PUT ERROR!" << std::endl;
        return std::make_pair(-1, -1);
    }

    std::pair<int, int> Put()
    {
        int cnt = nextChildState;
        for (int i = 0; i < BoardWidth; ++i)
            if (top[i] < BoardHeight && !(top[i] == noX && i == noY && noX == BoardHeight - 1))
            {
                if (cnt-- == 0)
                {
	                return (!(top[i] == noX && i == noY)) ? std::make_pair((int) top[i], i) : std::make_pair(
			                (int) top[i] + 1, i);
                }
            }
        std::cout << "PUT ERROR!" << std::endl;
        return std::make_pair(-1, -1);
    }

    int **ToBoard()
    {
        int **board = new int*[BoardHeight];
        for (int i = 0; i < BoardHeight; i++)
        {
	        board[i] = new int[BoardWidth];
	        memset(board[i], 0, BoardWidth * sizeof(int));
        }
        for (int j = 0; j < BoardWidth; j++)
            for (int i = 0; i < top[j]; i++)
	            if (!(i == noX && j == noY))
                board[BoardHeight - i - 1][j] = (((1 << i) & chessState[j]) > 0) + 1;
        return board;
    }

public:
	static int BoardWidth, BoardHeight, noX, noY;

	BoardState(bool player)
    {
        this->player = player;
        chessState = new u_short[BoardWidth];
        top = new u_short[BoardWidth];
        memset(chessState, 0, BoardWidth * sizeof(u_short));
        memset(top, 0, BoardWidth * sizeof(u_short));
	    numberOfChildStates = BoardWidth;
	    nextChildState = 0;
//        hash = state.hash;
    }

    BoardState(const BoardState &predstate, int x, int y)
    {

	    this->putLocation.first = x;
	    this->putLocation.second = y;
        player = 1 - predstate.player;
        chessState = new u_short[BoardWidth];
        top = new u_short[BoardWidth];
        memcpy(chessState, predstate.chessState, BoardWidth * sizeof(u_short));
        memcpy(top, predstate.top, BoardWidth * sizeof(u_short));
        chessState[y] |= player ? 0 : (1 << x);
        top[y] = (u_short) (x + 1);

        SetNumberOfChildStates();

//        SetHash();
    }

	std::pair<int, int> GetPutLocation()
	{
		return putLocation;
	};
    /*
    int GetHash() const
    {
        return hash;
    }
     */

    bool operator ==(const BoardState &right) const
    {
        return /*(hash == right.hash) && */(memcmp(chessState, right.top, BoardWidth * sizeof(u_short)) == 0) && (memcmp(top, right.top, BoardWidth * sizeof(u_short)) == 0);
    }

    MonteCarloSearchState *NextChildState() override
    {
        if (nextChildState == numberOfChildStates)
            return nullptr;
 
        std::pair<int, int> location = Put();

        nextChildState++;

        return new BoardState(*this, location.first, location.second);
    }

    int DefaultPolicy() override
    {

        bool player = this->player;
        std::pair<int, int> location;
        int **board = ToBoard();
        u_short *chessState = new u_short[BoardWidth], *top = new u_short[BoardWidth];
        memcpy(chessState, this->chessState, BoardWidth * sizeof(u_short));
        memcpy(top, this->top, BoardWidth * sizeof(u_short));

        while ((location = RandomPut()).first != -1)
        {
//	        std::cout << location.first << " " << location.second << std::endl;

            this->chessState[location.second] |= player ? (1 << location.first) : 0;
            this->top[location.second] = location.first + 1;

            board[BoardHeight - location.first - 1][location.second] = player + 1;
            if ((player && machineWin(BoardHeight - location.first - 1, location.second, BoardHeight, BoardWidth, board))
                || (!player && userWin(BoardHeight - location.first - 1, location.second, BoardHeight, BoardWidth, board)))
            {
                break;
            }
            player = 1 - player;
        }

//Print();
        for (int i = 0; i < BoardHeight; i++)
            delete[] board[i];
        delete[] board;
        memcpy(this->chessState, chessState, BoardWidth * sizeof(u_short));
        memcpy(this->top, top, BoardWidth * sizeof(u_short));

        delete[] chessState;
        delete[] top;

//      Tie / Win / Lose
        return location.first == -1 ? 0 : (player == this->player ? 1 : -1);
    }

	void Print() override
	{
		int **board = ToBoard();
		for (int i = 0; i < BoardHeight; i++)
		{
			for (int j = 0; j < BoardWidth; j++)
				switch (board[i][j]) {
					case 0:
						if (!(i == BoardHeight - noX - 1 && j == noY))
							std::cout << ". ";
						else
							std::cout << "X ";
						break;
					case 1:
						std::cout << "A ";
						break;
					case 2:
						std::cout << "B ";
						break;
					default:
						break;
				}
			std::cout << std::endl;
		}
		std::cout << std::endl;

	}
};

class BoardTreeNode : public MonteCarloSearchTreeNode
{
public:
    BoardTreeNode(MonteCarloSearchState *state, MonteCarloSearchTreeNode *parent) : MonteCarloSearchTreeNode(state, parent)
    {
    }

    MonteCarloSearchTreeNode *Expand() override
    {
        MonteCarloSearchState *newState = state->NextChildState();

	    if (newState == nullptr)
            return nullptr;

	    MonteCarloSearchTreeNode *newNode = new BoardTreeNode(newState, this);
        if (firstChild == nullptr)
        {
            firstChild = newNode;
        }
        else
        {
            MonteCarloSearchTreeNode *child;
            for (child = firstChild; child->GetNextSibling() != nullptr; child = child->GetNextSibling())
                ;
            child->SetNextSibling(newNode);
        }
        return newNode;
    }
};

class BoardTree : public MonteCarloSearchTree
{
	MonteCarloSearchTreeNode *pendingList[1000000];
	void BackTrace(MonteCarloSearchTreeNode *node, int value)
	{
		while (node != nullptr)
		{
			node->AddValue(value);
			value = -value;
			node = node->GetParent();
		}
	}

public:
    BoardTree(bool player) : MonteCarloSearchTree(new BoardTreeNode(new BoardState(player), nullptr))
    {
    }

    MonteCarloSearchTreeNode *TreePolicy() override
    {
	    int pendingListCurrent = 0, pendingListSize = 0;
	    pendingList[pendingListSize++] = root;
        while (pendingListCurrent < pendingListSize)
        {
            MonteCarloSearchTreeNode *current = pendingList[pendingListCurrent];
            MonteCarloSearchTreeNode *newNode = current->Expand();
            if (newNode != nullptr)
            {
	            return newNode;
            }
	        else
            {
	            for (MonteCarloSearchTreeNode *child = current->GetFirstChild(); child != nullptr; child = child->GetNextSibling())
	            {
		            pendingList[pendingListSize++] = child;
	            }
	            pendingListCurrent++;
            }
        }
	    std::cout << "Tree Policy nullptr!!" << std::endl;
	    
        return nullptr;
    }

	std::pair<int, int> MCTS()
	{
		clock_t startTime = clock();
		while (clock() - startTime < TIME_LIMIT)
		{
//			std::cout << pendingListSize << " " << pendingListCurrent << std::endl;
			MonteCarloSearchTreeNode *newNode = TreePolicy();
//			((BoardState*)newNode->GetState())->Print();
			BackTrace(newNode, newNode->GetState()->DefaultPolicy());
		}

		float maxValue = INF;
		std::pair<int, int> put = std::make_pair(-1, -1);
		for (MonteCarloSearchTreeNode *child = root->GetFirstChild(); child != nullptr; child = child->GetNextSibling())
		{
//			std::cout << (float)child->GetValue() << " " << child->GetTimes() << std::endl;

			if ((float)child->GetValue() / child->GetTimes() < maxValue)
			{
				maxValue = (float)child->GetValue() / child->GetTimes();
				put = ((BoardState *) child->GetState())->GetPutLocation();
			}
		}
//std::cout << std::endl;
		(new BoardState(*((BoardState*)root->GetState()), put.first, put.second))->Print();
		return put;
	};

	void MoveRoot(std::pair<int, int> put)
	{
		if (root->GetFirstChild() == nullptr)
		{
			MonteCarloSearchTreeNode *newNode;
			while ((newNode = root->Expand()) != nullptr)
				;
		}

		MonteCarloSearchTreeNode *newRoot;
		for (newRoot = root->GetFirstChild(); newRoot != nullptr; newRoot = newRoot->GetNextSibling())
		{
			if (((BoardState *)newRoot->GetState())->GetPutLocation() == put)
			{
//				std::cout << put.first << " " << put.second << std::endl;
//				std::cout << ((BoardState *)newRoot->GetState())->GetPutLocation().first << " " << ((BoardState *)newRoot->GetState())->GetPutLocation().second << std::endl;
				break;
			}
		}
		for (MonteCarloSearchTreeNode *child = root->GetFirstChild(); child != nullptr; child = child->GetNextSibling())
			if (child != newRoot)
				delete child;
		root = newRoot;
//		((BoardState *)child->GetState())->Print();

	}
};

/*
class BoardStateRecorder
{
BoardState states[NODE_AMOUNT];
int head[NODE_AMOUNT], next[NODE_AMOUNT], top;
public:
void Init()
{
	memset(head, -1, sizeof(head));
	memset(next, -1, sizeof(next));
	top = 0;
}
void AddState(const BoardState& state)
{
	int &&hash = state.GetHash();
	next[top] = head[hash];
	head[hash] = top;
	states[top] = state;
	top++;
}

int ExistState(const BoardState& state)
{
	int &&hash = state.GetHash(), index;
	for (index = head[hash]; index != -1; index = next[index])
	{
		if (states[index] == state)
			break;
	}
	return index;
}
const BoardState &GetState(const int index) const
{
	return states[index];
}
};
*/

/*
class BoardTree
{
    BoardTreeNode treeNodes[NODE_AMOUNT];
    int top, root;
//    BoardStateRecorder *boardStateRecorder;
public:
    BoardTree()
    {
//        boardStateRecorder = new BoardStateRecorder();
    }
    
    void Init()
    {
        memset(head, -1, sizeof(head));
        memset(next, -1, sizeof(next));
        top = 0;
        root = NewNode(BoardState(), -1);
//        boardStateRecorder->Init();
    }

    void MoveRoot(int x, int y, int player)
    {
        if (treeNodes[root].child[y] != -1)
        {
            root = treeNodes[root].child[y];
        }
        else
        {
            root = NewNode(BoardState(treeNodes[root].board, x, y, player), root);
        }
    }

    int NewNode(const BoardState &board, const int parent)
    {
        memset(treeNodes[top].child, -1, sizeof(treeNodes[top].child));
        treeNodes[top].parent = parent;
        treeNodes[top].board = board;
        return top++;
    }
    
};
*/

#endif /* Board_hpp */
