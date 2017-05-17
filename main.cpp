//
// Created by 陈齐斌 on 16/05/2017.
//

#include <iostream>
#include "Board.h"

int BoardState::BoardHeight = 8, BoardState::BoardWidth = 8;
int BoardState::noX = 7;
int BoardState::noY = 4;

int main()
{
	srand(time(0));
	MonteCarloSearchTree *boardTree= new BoardTree(1);

	std::pair<int, int> put = ((BoardTree*)boardTree)->MCTS();
	std::cout << put.first << " " << put.second << std::endl;

}
