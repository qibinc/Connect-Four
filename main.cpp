//
// Created by 陈齐斌 on 16/05/2017.
//

#include <iostream>
#include "Board.h"

int BoardState::BoardHeight = 9, BoardState::BoardWidth = 10;
int BoardState::noX = 8, BoardState::noY = 7;
int **BoardState::board;
clock_t BoardTree::StartTime;
int main()
{
//	srand((unsigned int) time(0));

	BoardTree *boardTree= new BoardTree(1);
	std::cout << clock() - BoardTree::StartTime << std::endl;

//	Round 1
	BoardTree::StartTime = clock();

	std::pair<int, int> put = boardTree->MonteCarloTreeSearch();

	std::cout << clock() - BoardTree::StartTime << std::endl;

//	Round 2
	BoardTree::StartTime = clock();
	boardTree->MoveRoot(put);

	boardTree->MoveRoot(std::make_pair(0, 4));

	put = boardTree->MonteCarloTreeSearch();

	std::cout << clock() - BoardTree::StartTime << std::endl;

//	Round 3
	BoardTree::StartTime = clock();
	boardTree->MoveRoot(put);

	boardTree->MoveRoot(std::make_pair(1, 4));

	put = boardTree->MonteCarloTreeSearch();

	std::cout << clock() - BoardTree::StartTime << std::endl;

//	Round 4
	BoardTree::StartTime = clock();
	boardTree->MoveRoot(put);

	boardTree->MoveRoot(std::make_pair(2, 4));

	put = boardTree->MonteCarloTreeSearch();

	std::cout << clock() - BoardTree::StartTime << std::endl;

//	Round 5
	BoardTree::StartTime = clock();
	boardTree->MoveRoot(put);

	boardTree->MoveRoot(std::make_pair(2, 6));

	put = boardTree->MonteCarloTreeSearch();

	std::cout << clock() - BoardTree::StartTime << std::endl;

//	Round 6
	BoardTree::StartTime = clock();
	boardTree->MoveRoot(put);

	boardTree->MoveRoot(std::make_pair(0, 8));

	put = boardTree->MonteCarloTreeSearch();

	boardTree->MoveRoot(put);
	std::cout << clock() - BoardTree::StartTime << std::endl;

}
