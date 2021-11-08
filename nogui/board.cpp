#include "board.hpp"
#include <chrono>
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
	for(int i = 0;i < 64;i++)
		board[i] = -1;
}

int Board::set(int pos, int rotation)
{
	if(pos < 0 || pos > 63 || rotation < 0 || rotation > 7)
		return -2;
	if(prevPos >= 0)
	{
		switch (prevRot)
		{
		case 0:
		case 4:
			if(!(pos%8 == prevPos%8)) return -1;
			break;
		case 1:
		case 5:
			if(!((pos%8+pos/8) == (prevPos%8+prevPos/8))) return -1;
			break;
		case 2:
		case 6:
			if(!(pos/8 == prevPos/8)) return -1;
			break;
		case 3:
		case 7:
			if(!((pos%8-pos/8) == (prevPos%8-prevPos/8))) return -1;
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

	for(int i = 0;i < 8;i++)
		if((board[(prevPos/8)*8+i] == -1 && (prevRot==2||prevRot==6)) || (board[(prevPos%8)+i*8] == -1 && (prevRot==0||prevRot==4)))
			return 0;

	int tests[4] = {-9,-7,7,9};
	int rot[4] = {1,1,3,3};
	for(int s = 1;s < 8;s++)
		for(int i = 0;i < 4;i++)
			if(prevPos+tests[i]*s >= 0 && prevPos+tests[i]*s < 64)
				if(board[prevPos+tests[i]*s] == -1 && (prevRot==rot[i]||prevRot==8-rot[i]))
					return 0;
	return 2;
}

int Board::fill(int pos, int side)
{
	if(board[pos] == -1 || board[pos]>>3 != side) return 0;
	int ret = 1;
	board[pos] = -1;

	int tests[4] = {-8,8,-1,1};
	for(int i = 0;i < 4;i++)
		if(pos+tests[i] >= 0 && pos+tests[i] < 64)
			ret += fill(pos+tests[i],side);

	return ret;
}

int Board::check()
{
	if(checkStats() == 0)
		return 0;
	
	for(int i = 0;i < 64;i++)
		if(board[i] == -1)
			board[i] = (!turn)<<3;

	int max = 0;
	int side = 0;
	for(int i = 0;i < 64;i++)
	{
		int fs = board[i]>>3;
		int f = fill(i,board[i]>>3);
		if(max < f)
		{
			max = f;
			side = fs;
		}
	}
	return side?1:-1;
}
