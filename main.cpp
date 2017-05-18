//
// Created by 陈齐斌 on 16/05/2017.
//

#include <iostream>
#include "Board.h"

int BoardState::BoardHeight = 9, BoardState::BoardWidth = 9;
int BoardState::noX = 3, BoardState::noY = 1;
int **BoardState::board;

int main()
{
	srand((unsigned int) time(0));

	BoardTree *boardTree= new BoardTree(0);

//	Round 1
	boardTree->MoveRoot(std::make_pair(0, 3));

	std::pair<int, int> put = boardTree->MonteCarloTreeSearch();

	boardTree->MoveRoot(put);

//	Round 2
	boardTree->MoveRoot(std::make_pair(1, 5));

	put = boardTree->MonteCarloTreeSearch();

	boardTree->MoveRoot(put);

//	Round 3
	boardTree->MoveRoot(std::make_pair(0, 6));

	put = boardTree->MonteCarloTreeSearch();

	boardTree->MoveRoot(put);

//	Round 4
	boardTree->MoveRoot(std::make_pair(1, 6));

	put = boardTree->MonteCarloTreeSearch();

	boardTree->MoveRoot(put);

//	Round 5
	boardTree->MoveRoot(std::make_pair(2, 5));

	put = boardTree->MonteCarloTreeSearch();

	boardTree->MoveRoot(put);

//	Round 6
	boardTree->MoveRoot(std::make_pair(0, 0));

	put = boardTree->MonteCarloTreeSearch();

	boardTree->MoveRoot(put);

}
