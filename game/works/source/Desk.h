#pragma once
#include "Card.h"
#include <fstream>
using namespace std;
const double D2I=0.00001;
const int BIGBLIND=40;
struct record
{
	int pos;
	int chip;
};
//������Ϣ
class Desk
{
private:
	void UpdateCards(Cards c);
	int CalPot();
public:
    static int BigBlind;
	int PeopleNumAll;
	//const int BigBlind=50;
	int Round;
	int pot;
	int last_pot;
	int preflop_pot;
	int NowStack;//Ŀǰ�����ע
	int minRaise;
	Cards DeskCard;
	int RoundBeginCircle[4];//ÿһ�ֶ�Ӧscene��ʼ��Ȧ��
	vector<int> PlayerAction;//��ע��Ϊ 0 fold 1 call 2 raise 3 All in
	vector<int> PlayerChips;
	vector<int> tempscene;
	void Initial(int playerpos,int PeopleNum,int OriStack,vector<int> TempScene);
	void Update(vector<int>TempScene,vector<int> TempChips,int POT,Cards c,int money);
	int GetNowStack(vector<int> TempNowScene);
	int DefineTurnLevel();
	int DefineRiverLevel();
	Desk(void);
	~Desk(void);
};
extern Desk desk;
extern ofstream runlog;
