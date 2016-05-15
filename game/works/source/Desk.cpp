#include "Desk.h"
#include "Player.h"
#include "level.h"
#include <fstream>
Desk desk;
int Desk::BigBlind;
int Desk::GetNowStack(vector<int> TempNowScene)
{
	unsigned int i;
	int result=0;
	for (i=0;i<TempNowScene.size();i++)
		if (TempNowScene[i]>result) result=TempNowScene[i];
	return result;

}
void Desk::Initial(int playerpos,int PeopleNum,int OriStack,vector<int> TempScene)
{
	int i;
	player.pos=playerpos;
	player.ReadExcel();
	player.HandCard.element.clear();
	DeskCard.element.clear();
    runlog<<"****************************************************************************Initial********************************************************************************"<<endl;
	runlog<<"playerpos "<<playerpos<<endl;
	player.OriStack=player.RemainStack=OriStack;
	Round=0;
	last_pot = int(Desk::BigBlind * 1.5);
	pot = last_pot;
	preflop_pot=pot;
    //minRaise=1;
	player.fold_raise_status = -1;
	PlayerChips.clear();
	for(i=0;i<PeopleNum;i++)
	{
		PlayerChips.push_back(0);
	}
	for (i=0;i<PeopleNumAll;i++)
	{
		PlayerAction.push_back(0);
	}
}
void Desk::UpdateCards(Cards c)
{
	unsigned int i;//,j,k;

	player.HandCard.element.clear();
	DeskCard.element.clear();
	for (i=0;i<=1;i++)
		player.HandCard.element.push_back(c.element[i]);
	Card x;
	if (player.HandCard.element[0].num<player.HandCard.element[1].num)
	{
		x=player.HandCard.element[0];
		player.HandCard.element[0]=player.HandCard.element[1];
		player.HandCard.element[1]=x;
	}
	for (i=2;i<c.element.size();i++)
		 DeskCard.element.push_back(c.element[i]);

	for (i=0;i<player.HandCard.element.size();i++)
		runlog<<"PlayerCard "<<player.HandCard.element[i].num<<' '<<player.HandCard.element[i].color<<' ';
	runlog<<endl;
	for (i=0;i< DeskCard.element.size();i++)
		runlog<<"DeskCard "<< DeskCard.element[i].num<<' '<< DeskCard.element[i].color<<' ';
	runlog<<endl;
}

void Desk::Update(vector<int>TempScene,vector<int> TempChips,int POT,Cards c,int money)
{
	unsigned int i;
	if (player.pos==0) money+=BIGBLIND/2;
	if (player.pos==1) money+=BIGBLIND;
	tempscene=TempScene;
	pot=POT;
	if (money>player.OriStack)  player.OriStack=money;
	player.NowStack=TempScene[player.pos];
	runlog<<"MyselfNowStack:"<<player.NowStack<<endl;
	NowStack=GetNowStack(TempScene);
	runlog<<"DeskNowStack:"<<NowStack;
	runlog<<"TempScene"<<endl;
	for (i=0;i<TempScene.size();i++)
		runlog<<TempScene[i]<<' ';
	runlog<<endl;
	int total_deskcards_1 = DeskCard.element.size();
	UpdateCards(c);
	int total_deskcards_2 = DeskCard.element.size();
	if(total_deskcards_1 == 0 && total_deskcards_2 == 3)
	    preflop_pot = pot;
	runlog<<endl;
}

int Desk::DefineTurnLevel()
{
	return DefineRiverLevel();
}
int Desk::DefineRiverLevel()
//pair,two pair,top pair,trips 3 straight 4 flush 5 full house 6 four 7 flushstraight 8
{
	Cards c=DeskCard;
	c.sort();
	unsigned int i;
	int col[5]={0};
	int equalnum=0;
	for (i=0;i<c.element.size();i++)
		col[c.element[i].color]++;
	for (i=0;i<=4;i++)
		if (col[i]>=3) return 5;
	if (c.element[c.element.size()-1].num==14) c.element.insert(c.element.begin(),Card(1,1));
	for (i=2;i<c.element.size();i++)
		if (c.element[i].num-c.element[i-2].num<=4) return 4;
	return 3;
}

Desk::Desk(void)
{

}


Desk::~Desk(void)
{

}
