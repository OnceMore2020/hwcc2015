#include "Control.h"
#include "Desk.h"
#include "Player.h"
using namespace std;
ofstream runlog;
Control::Control()
{

}
Control::~Control()
{

}
void Control::Initial()
{
	runlog.open("log.txt",ios::out);
}
bool Control::CheckLikeCheck()
{
	if (desk.NowStack-player.NowStack<=BIGBLIND) return true;
	return false;
}

int Control::CheckRaiseCallFold_FirstCircle()
{
	int ini=Desk::BigBlind;
	int max=0;
	for(int i=2;i<=player.pos;i++)
	{
		if(desk.tempscene[i]>max)
		   max=desk.tempscene[i];
	}
	if (max<ini) return 0;
	if (max==ini) return 1;
	if (max>ini) return 2;
	return 0;
}
SParam Control::GetSParam(Cards c ,SParam Raise[28][15])
{
	int p1,p2;
	if (c.element[0].color==c.element[1].color) p1=13+c.element[0].num;
	else p1=c.element[0].num;
	p2=c.element[1].num;
	return Raise[p1][p2];
}

bool Control::CheckInArray(Cards c,SParam Raise[28][15])
{
	SParam s=GetSParam(c,Raise);
	if (s.num<D2I && s.c==0) return false;
	else return true;
}

bool Control::isT(int pair)
{
	//T>=top pair  或者 T是 两头顺draw 或者 同花draw
	if (player.CheckTwoPair() || player.CheckTopPair()||player.CheckFlush()||player.CheckFour()
		||player.CheckStraight()) return true;
	else if (pair==1 && player.CheckOnePair()) return true;
	else return false;
}
bool Control::IsWaitingT()
{
	//同花或者是两头顺
	if(player.CheckFlushDraw() || player.CheckStraightDraw())
		return true;
	return false;
}
int Control::PreFlop()
{
	runlog<<"*****************************************PreFlop**************************************"<<endl;
	int i,j;	
	int max=desk.NowStack;
	int self=player.NowStack;
	int CallNum=max-self;
	int flag=CheckRaiseCallFold_FirstCircle();
	if (player.fold_raise_status==-1)
		player.fold_raise_status=flag;	
	double R=player.CalR();
	runlog<<"flag "<<flag<<"Player pos "<<player.pos<<endl;
	runlog<<"Raise Lib"<<endl;
	runlog<<"R "<<R<<endl;
	for (i=27;i>=2;i--)
	{
		for (j=14;j>=2;j--)
			runlog<<player.Raise[flag][i][j].c<<player.Raise[flag][i][j].num<<' ';
		runlog<<endl;
	}
	runlog<<"LimpIn Lib"<<endl;
	for (i=27;i>=2;i--)
	{
		for (j=14;j>=2;j--)
			runlog<<player.LimpIn[flag][i][j].c<<player.LimpIn[flag][i][j].num<<' ';
		runlog<<endl;
	}
	//特殊情况处理
	if (
		(player.HandCard.element[0].num==12 && player.HandCard.element[1].num==12)
       || (player.HandCard.element[0].num==13 && player.HandCard.element[1].num==13)
	   || (player.HandCard.element[0].num==14 && player.HandCard.element[1].num==14)
	 //|| (player.HandCard.element[0].num==14 && player.HandCard.element[1].num==13)
	   )
	{
		//if(desk.NowStack-player.NowStack>0)
		//	return desk.NowStack-player.NowStack;
		return desk.pot;
	}
	if(R>5)
	{
	   if (CheckInArray(player.HandCard,player.Raise[flag]))
	   {
		runlog<<"Raise"<<endl;
		SParam sp=GetSParam(player.HandCard,player.Raise[flag]);
		return CallNum;
	   }
	   else if (CheckInArray(player.HandCard,player.LimpIn[flag]))
	   {
		runlog<<"LimpIn"<<endl;
		SParam sp=GetSParam(player.HandCard,player.LimpIn[flag]);
		//if (R>=sp.num) return max-self;
		return CallNum;
		//else return 0;			
	   }
	}
    return 0;
}
int Control::Flop()
{
	runlog<<"*****************************************Flop**************************************"<<endl;
	bool T=isT(2),T_OnePair=isT(1),WaitingT=IsWaitingT();
	int BetSize=desk.pot;
	runlog<<"T "<<T<<endl;
	if(T) return BetSize;
	if(WaitingT||(desk.pot>4*(desk.NowStack-player.NowStack))) return  desk.NowStack-player.NowStack;
	return 0;
}
int Control::Turn()
{
	runlog<<"*****************************************Turn**************************************"<<endl;
	int BetSize=desk.pot;
	int pot=desk.pot;
	//bool T=isT(2),T_OnePair=isT(1),
	bool WaitingT=IsWaitingT();
	if(player.CheckFlush()||player.CheckFour()
		||player.CheckStraight()||player.CheckFullHouse()) 
		{
	         return pot;
		}
	if(player.CheckTopPair()||player.CheckTwoPair())
	    return desk.NowStack-player.NowStack;
	if(WaitingT) if((desk.NowStack-player.NowStack)*6<pot) return desk.NowStack-player.NowStack;
	return 0;
}
int Control::River()
{
	runlog<<"**********************************River************************************"<<endl;
	int pot=desk.pot;
	runlog<<"pot:"<<pot<<endl;
	int BetSize=desk.pot;
	if(player.CheckFlush()||player.CheckFour()
		||player.CheckStraight()||player.CheckFullHouse()) 
		{
            return pot;
		}
	if(player.CheckTopPair()||player.CheckTwoPair())
		return desk.NowStack-player.NowStack;
	return 0;
}
int Control::CalAction()
{
	int num=0;
	if (desk.DeskCard.element.size()==0) num=PreFlop();
	if (desk.DeskCard.element.size()==3) num=Flop();
	if (desk.DeskCard.element.size()==4) num=Turn();
	if (desk.DeskCard.element.size()==5) num=River();
	runlog<<"CalAction Result "<<num<<endl;
	if (num>desk.NowStack-player.NowStack+D2I)
		if (num<desk.minRaise+desk.NowStack-player.NowStack)
			num=desk.minRaise+desk.NowStack-player.NowStack;
	return num;
}
//status 0 放弃 1 check 2 call 3 raise 4 all in
void Control::Anaysis(int &status,int &num)
{
	runlog<<"Interface "<<player.OriStack<<' '<<player.RemainStack<<endl;
	runlog<<"Desk Now Stack"<<desk.NowStack<<"Player Now Stack"<<player.NowStack<<endl;	
	num=CalAction();
	if (num<D2I) 
	{
		if(desk.NowStack-player.NowStack>0)
		{
			status=0;
		}
		else
		status=1;
		return;
	}
	if (num>player.RemainStack-D2I)
	{
		status=4;return;
	}
	if (num<=desk.NowStack-player.NowStack+D2I) status=2;
	else status=3;
}





