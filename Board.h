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
#include <cmath>
#include "Judge.h"
#include "MonteCarloTreeSearch.h"

#define TIME_LIMIT 2
#define INF 1000000000
#define C 0.8

#define USER 0
#define MACHINE 1

class BoardState : public MonteCarloSearchState
{
    u_short *chessState, *top;
    int nextChildState;
	std::pair<int, int> putLocation;
//    player 表示当前轮到谁落子
    bool player;

    std::pair<int, int> RandomPut();

    std::pair<int, int> Put();

	static int **board;
	void ToBoard();

public:
	static int BoardWidth, BoardHeight, noX, noY;

	BoardState(bool player);

    BoardState(const BoardState &predstate, int x, int y);

	virtual ~BoardState();

	std::pair<int, int> GetPutLocation();

    MonteCarloSearchState *NextChildState() override;

    int DefaultPolicy() override;

	void Print() override;
};

class BoardTreeNode : public MonteCarloSearchTreeNode
{
public:
    BoardTreeNode(MonteCarloSearchState *state, MonteCarloSearchTreeNode *parent) : MonteCarloSearchTreeNode(state, parent)
    {}

    MonteCarloSearchTreeNode *Expand() override;
};

class BoardTree : public MonteCarloSearchTree
{
	float UCB1(MonteCarloSearchTreeNode *parent, MonteCarloSearchTreeNode *child);

	MonteCarloSearchTreeNode *BestChild(MonteCarloSearchTreeNode *parent);

	void BackTrace(MonteCarloSearchTreeNode *node, int value);

public:
    BoardTree(bool player) : MonteCarloSearchTree(new BoardTreeNode(new BoardState(player), nullptr))
    {}

    MonteCarloSearchTreeNode *TreePolicy() override;

	void MoveRoot(std::pair<int, int> put);

	std::pair<int, int> MonteCarloTreeSearch();
};

#endif /* Board_hpp */
