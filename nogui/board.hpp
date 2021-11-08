#pragma once

class Board
{
	int board[64];
	int time[2];
	int turn;
	int prevPos, prevRot;
	long long int startTime;

	int checkStats();
	int fill(int pos, int side);
public:
	Board(int _time);
	Board() : Board(300) {};

	int set(int pos,int rotation);
	int check();
	int getBoard(int i) {if(i>=0&&i<64) return board[i]; else return -2;}
	int displayTime(int player) {return time[player?1:0];}
};
