#include <iostream>
#include "board.hpp"

int main()
{
	Board board;
	for(int i = 0;i < 2;i++)
		board.set(i,2);
	for(int i = 0;i < 64;i++)
		std::cout << board.getBoard(i);
	std::cout << std::endl << board.displayTime(0) << std::endl;
	std::cout << board.check() << std::endl;
	return 0;
}
