//
//  Board.cpp
//  Strategy
//
//  Created by 陈齐斌 on 14/05/2017.
//  Copyright © 2017 Yongfeng Zhang. All rights reserved.
//

#include "Board.h"

BoardState::BoardState(bool player)
{
	this->player = player;
	chessState = new u_short[BoardWidth];
	top = new u_short[BoardWidth];
	memset(chessState, 0, BoardWidth * sizeof(u_short));
	memset(top, 0, BoardWidth * sizeof(u_short));
	nextChildState = 0;
	BoardState::board = new int*[BoardState::BoardHeight];
	for (int i = 0; i < BoardState::BoardHeight; i++)
		BoardState::board[i] = new int[BoardState::BoardWidth];
//	srand((unsigned int) time(0));
}

BoardState::BoardState(const BoardState &predstate, int x, int y)
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
	nextChildState = 0;
//	判断终态
	ToBoard();
//  若当前0落子，说明上一步1落了子，1有可能赢
	if ((!player && machineWin(BoardHeight - putLocation.first - 1, putLocation.second, BoardHeight, BoardWidth, board))
	    || (player && userWin(BoardHeight - putLocation.first - 1, putLocation.second, BoardHeight, BoardWidth, board)))
		nextChildState = BoardWidth;
	else
	{
//		确定子状态下一步可以下哪里
		for (; nextChildState < BoardWidth; ++nextChildState)
			if (top[nextChildState] < BoardHeight && !(top[nextChildState] == noX && nextChildState == noY && noX == BoardHeight - 1))
				break;
	}
}

BoardState::~BoardState()
{
	delete[] chessState;
	delete[] top;
}

std::pair<int, int> BoardState::RandomPut()
{/*
	int cnt = 0;
	for (int i = 0; i < BoardWidth; ++i)
		if (top[i] < BoardHeight && !(top[i] == noX && i == noY && noX == BoardHeight - 1))
			cnt++;
	if (cnt == 0)
		return std::make_pair(-1, -1);
	while (true)
	{
		int &&index = rand() % BoardWidth;
		if (top[index] < BoardHeight && !(top[index] == noX && index == noY && noX == BoardHeight - 1))
			return (!(top[index] == noX && index == noY)) ? std::make_pair((int) top[index], index) : std::make_pair(
					(int) top[index] + 1, index);
	}
 */

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

std::pair<int, int> BoardState::Put()
{
	std::pair<int, int> &&location = (!(top[nextChildState] == noX && nextChildState == noY)) ? std::make_pair((int) top[nextChildState], nextChildState) : std::make_pair(
			(int) top[nextChildState] + 1, nextChildState);

	for (nextChildState++; nextChildState < BoardWidth; ++nextChildState)
		if (top[nextChildState] < BoardHeight && !(top[nextChildState] == noX && nextChildState == noY && noX == BoardHeight - 1))
			break;
	return location;
//	std::cout << "PUT ERROR!" << std::endl;
//	return std::make_pair(-1, -1);
}

void BoardState::ToBoard()
{
	for (int i = 0; i < BoardHeight; i++)
		memset(board[i], 0, BoardWidth * sizeof(int));
	for (int j = 0; j < BoardWidth; j++)
		for (int i = 0; i < top[j]; i++)
			if (!(i == noX && j == noY))
				board[BoardHeight - i - 1][j] = (((1 << i) & chessState[j]) > 0) + 1;
}

std::pair<int, int> BoardState::GetPutLocation()
{
	return putLocation;
};

MonteCarloSearchState *BoardState::NextChildState()
{
	if (nextChildState == BoardWidth)
		return nullptr;

	std::pair<int, int> location = Put();

	return new BoardState(*this, location.first, location.second);
}

int BoardState::DefaultPolicy()
{
	if (nextChildState == BoardWidth)
	{
		return -1;
	}

	bool player = this->player;
	std::pair<int, int> location;
	ToBoard();

	u_short *top = new u_short[BoardWidth];
	memcpy(top, this->top, BoardWidth * sizeof(u_short));

	while ((location = RandomPut()).first != -1)
	{
		this->top[location.second] = location.first + 1;

		board[BoardHeight - location.first - 1][location.second] = player + 1;
		if ((player && machineWin(BoardHeight - location.first - 1, location.second, BoardHeight, BoardWidth, board))
		    || (!player && userWin(BoardHeight - location.first - 1, location.second, BoardHeight, BoardWidth, board)))
		{
			break;
		}
		player = 1 - player;
	}

	memcpy(this->top, top, BoardWidth * sizeof(u_short));

	delete[] top;

//  Tie / Win / Lose
	return location.first == -1 ? 0 : (player == this->player ? 1 : -1);
}

void BoardState::Print()
{
	ToBoard();

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



MonteCarloSearchTreeNode *BoardTreeNode::Expand()
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
		newNode->SetNextSibling(firstChild);
		firstChild = newNode;
	}
	return newNode;
}



float BoardTree::UCB1(MonteCarloSearchTreeNode *parent, MonteCarloSearchTreeNode *child)
{
	int &&N = child->GetTimes(), &&Q = child->GetValue();
	return -(float)Q / N + C * sqrtf(2 * logf(parent->GetTimes()) / N);
}

MonteCarloSearchTreeNode *BoardTree::BestChild(MonteCarloSearchTreeNode *parent)
{
	float maxValue = -INF;
	MonteCarloSearchTreeNode *bestChild = nullptr;
	for (MonteCarloSearchTreeNode *child = parent->GetFirstChild(); child != nullptr; child = child->GetNextSibling())
	{

		float &&ucb1 = UCB1(parent, child);

//			Choose the most unfavorable decision of the enemy
		if (ucb1 > maxValue)
		{
			maxValue = ucb1;
			bestChild = child;
		}
	}

	return bestChild;
}

void BoardTree::BackTrace(MonteCarloSearchTreeNode *node, int value)
{
	while (node != nullptr)
	{
		node->AddValue(value);
		value = -value;
		node = node->GetParent();
	}
}

MonteCarloSearchTreeNode *BoardTree::TreePolicy()
{
	MonteCarloSearchTreeNode *current = root, *newNode;

	while ((newNode = current->Expand()) == nullptr && current->GetFirstChild() != nullptr)
	{
		current = BestChild(current);
	}

	return current->GetFirstChild() == nullptr ? current : newNode;
}

void BoardTree::MoveRoot(std::pair<int, int> put)
{

	if (root->GetFirstChild() == nullptr)
	{
		MonteCarloSearchTreeNode *newNode;
//		while ((newNode = root->Expand()) != nullptr)
//			;
		while (true)
		{
			newNode = root->Expand();
			if (newNode == nullptr)
				break;
		}
	}

	MonteCarloSearchTreeNode *newRoot;
	for (newRoot = root->GetFirstChild(); newRoot != nullptr; newRoot = newRoot->GetNextSibling())
	{
		if (((BoardState *)newRoot->GetState())->GetPutLocation() == put)
			break;
	}
	for (MonteCarloSearchTreeNode *child = root->GetFirstChild(), *next; child != nullptr; child = next)
	{
		next = child->GetNextSibling();
		if (child != newRoot)
			delete child;
	}
	root->SetFirstChild(newRoot);
	newRoot->SetNextSibling(nullptr);
	root = newRoot;
}

std::pair<int, int> BoardTree::MonteCarloTreeSearch()
{
	int per_clock = 0;
	clock_t startTime = clock();

	while ((++per_clock % 100 != 0) || (clock() - startTime) < CLOCKS_PER_SEC * TIME_LIMIT)
	{
		MonteCarloSearchTreeNode *newNode = TreePolicy();

		BackTrace(newNode, newNode->GetState()->DefaultPolicy());
	}

	int maxTime = -INF;
	MonteCarloSearchTreeNode *bestChild = nullptr;
	for (MonteCarloSearchTreeNode *child = root->GetFirstChild(); child != nullptr; child = child->GetNextSibling())
	{
//		Choose the most unfavorable decision of the enemy
		if (child->GetTimes() > maxTime)
		{
			maxTime = child->GetTimes();
			bestChild = child;
		}
	}
	if (bestChild == nullptr)
		std::cout << "No answer!" << std::endl;

	std::pair<int,int> put = ((BoardState*)bestChild->GetState())->GetPutLocation();

//	bestChild->GetState()->Print();
	return put;
};
