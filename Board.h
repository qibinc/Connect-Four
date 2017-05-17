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

#define TIME_LIMIT 0.1
#define INF 1000000000
#define C 0.8

#define USER 0
#define MACHINE 1

class BoardState : public MonteCarloSearchState
{
    u_short *chessState, *top;
    int numberOfChildStates, nextChildState;
	std::pair<int, int> putLocation;
//    player 表示当前轮到谁落子
    bool player;

    void SetNumberOfChildStates();

    std::pair<int, int> RandomPut();

    std::pair<int, int> Put();

    void ToBoard();
	static int **board;

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
	void BackTrace(MonteCarloSearchTreeNode *node, int value);

	float UCB1(MonteCarloSearchTreeNode *parent, MonteCarloSearchTreeNode *child);

	MonteCarloSearchTreeNode *BestChild(MonteCarloSearchTreeNode *parent);

public:
    BoardTree(bool player) : MonteCarloSearchTree(new BoardTreeNode(new BoardState(player), nullptr))
    {}

    MonteCarloSearchTreeNode *TreePolicy() override;

	std::pair<int, int> MCTS();

	void MoveRoot(std::pair<int, int> put);
};

#endif /* Board_hpp */
