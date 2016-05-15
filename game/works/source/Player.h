#pragma once
#include "Card.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
//自己的信息
struct SParam
{
	char c;
	double num;
};
class Player
{
private:

	void ClearPlayerData();
	void ErasePair(Cards &c);
public:
	int pos;//计数为从0开始的，切记,小盲位置为0。*****************************************************************
	int OpenRaise,RaiseNum;
	int OriStack,RemainStack;
	int NowStack;
	int fold_raise_status;
	Cards HandCard;
	SParam Special_Raise[3][28][15];
	SParam Special_LimpIn[3][28][15];
	SParam Raise_Limpin[9][3][28][15];
	SParam Raise[3][28][15]; // 0 fold  1 limp in  2 raise
	SParam LimpIn[3][28][15];
	double CalR();
	Cards CominePlayerDeskCard();
	bool CheckOnePair();
	bool CheckTopPair();
	bool CheckMidPair();
	bool CheckTwoPair();
	bool CheckTrip();
	bool CheckFlush();
	bool CheckStraight();
	bool CheckFour();
	bool CheckStraightDraw();
	bool CheckSingleStraightDraw();
	bool CheckFlushDraw();
	bool CheckPotCommited();
	bool CheckPairPotCommited();
    bool CheckFullHouse();
	int DefineCardLevel(int threadhold=0);
	vector<string> split(string s);
	void ReadExcel();
	void ReadOpenLib(string FileName,SParam result[28][15]);
	Player(void);
	~Player(void);
};
extern Player player;
