#pragma once
#define SIZE 4

class Board
{
	int board[SIZE*SIZE];
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
	int getBoard(int i) {if(i>=0&&i<SIZE*SIZE) return board[i]; else return -2;}
	int displayTime(int player) {return time[player?1:0];}
};
