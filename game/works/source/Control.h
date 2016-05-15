#pragma once
#include "Card.h"
#include "Player.h"

//æﬂÃÂAcitonøÿ÷∆
class Control
{
public:
	Control();
	~Control();
	static void Anaysis(int& status,int &num);
	static void Initial();
	static int CalAction();	
	static int CheckRaiseCallFold_FirstCircle();
	static int PreFlop();
	static int Flop();
	static int Turn();
	static int River();
	static SParam GetSParam(Cards c ,SParam Raise[28][15]);
	static bool CheckInArray(Cards c,SParam Raise[28][15]);
	static bool isT(int pair);
	static bool CheckLikeCheck();
	static int CalF(int TurnLevel,int CardLevel);
	static bool IsWaitingT();
};
