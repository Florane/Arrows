#include <iostream>
#include "board.hpp"

int main()
{
	Board board;
	board.set(5,7);
	board.set(0,4);
	board.set(4,1);
	board.set(1,2);
	board.set(2,2);
	board.set(3,4);
	board.set(11,0);
	board.set(7,4);
	board.set(15,6);
	board.set(13,2);
	board.set(14,6);
	board.set(12,0);
	board.set(8,2);
	board.set(9,2);
	board.set(10,0);
	board.set(6,0);
	for(int y = 0;y < 4;y++)
	{
		for(int x = 0;x < 4;x++)
			std::cout << board.getBoard(y*4+x) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl << board.displayTime(0) << std::endl;
	std::cout << board.check() << std::endl;
	return 0;
}
