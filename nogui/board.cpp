#include "board.hpp"
#include <chrono>
#include <curses.h>
#define currentTime std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()

int abs(int i){return i>=0?i:-i;}
int sign(int i){return i>=0?1:-1;}

Board::Board(int _time)
{
	time[0] = _time*1000;
	time[1] = _time*1000;
	startTime = currentTime;
	turn = 0;
	prevPos = -1;
	result = 0;
	for(int i = 0;i < SIZE*SIZE;i++)
		board[i] = -1;
}

int Board::set(int pos, int rotation)
{
	if(pos < 0 || pos > SIZE*SIZE-1 || rotation < 0 || rotation > 7)
		return -2;
	if(prevPos >= 0)
	{
		switch (prevRot)
		{
		case 0:
		case 4:
			if(!(pos%SIZE == prevPos%SIZE)) return -1;
			break;
		case 1:
		case 5:
			if(!((pos%SIZE+pos/SIZE) == (prevPos%SIZE+prevPos/SIZE))) return -1;
			break;
		case 2:
		case 6:
			if(!(pos/SIZE == prevPos/SIZE)) return -1;
			break;
		case 3:
		case 7:
			if(!((pos%SIZE-pos/SIZE) == (prevPos%SIZE-prevPos/SIZE))) return -1;
			break;
		default:
			return -2;
		}
	}
	if(board[pos] != -1)
		return -1;
	board[pos] = rotation|turn<<3;
	time[turn] -= currentTime-startTime;
	startTime = currentTime;
	prevPos = pos;
	prevRot = rotation;

	turn = turn?0:1;
	return 0;
}

int Board::checkStats()
{
	if(time[0] < 0 || time[1] < 0)
		return 1;

	for(int i = 0;i < SIZE;i++)
		if((board[(prevPos/SIZE)*SIZE+i] == -1 && (prevRot==2||prevRot==6)) || (board[(prevPos%SIZE)+i*SIZE] == -1 && (prevRot==0||prevRot==4)))
			return 0;

	int tests[4] = {-SIZE-1,-SIZE+1,SIZE-1,SIZE+1};
	int rot[4] = {1,1,3,3};
	for(int s = 1;s < SIZE;s++)
		for(int i = 0;i < 4;i++)
			if(prevPos+tests[i]*s >= 0 && prevPos+tests[i]*s < SIZE*SIZE)
				if(board[prevPos+tests[i]*s] == -1 && (prevRot==rot[i]||prevRot==8-rot[i]))
					return 0;
	return 2;
}

int Board::fill(int pos, int side)
{
	if(board[pos] == -1 || board[pos]>>3 != side) return 0;
	int ret = 1;
	board[pos] = -1;

	int tests[4] = {-SIZE,SIZE,-1,1};
	for(int i = 0;i < 4;i++)
		if(pos+tests[i] >= 0 && pos+tests[i] < SIZE*SIZE)
			ret += fill(pos+tests[i],side);

	return ret;
}

int Board::check()
{
	if(result != 0)
		return result;
	if(checkStats() == 0)
		return 0;
	
	for(int i = 0;i < SIZE*SIZE;i++)
		if(board[i] == -1)
			board[i] = (!turn)<<3;

	int max = 0;
	int side = 0;
	for(int i = 0;i < SIZE*SIZE;i++)
	{
		int fs = board[i]>>3;
		int f = fill(i,board[i]>>3);
		if(max < f)
		{
			max = f;
			side = fs;
		}
	}
	result = (side?1:-1)*max;
	return result;
}
