#include "Player.h"
#include "Interface.h"
#include "Desk.h"
#include "level.h"
#include <stdlib.h>
Player player;
Player::Player(void)
{
	NowStack=0;
}


Player::~Player(void)
{
}

void Player::ClearPlayerData()
{
	SParam temp;
	temp.c=0;
	temp.num=0;
	int i,j,k;
	for (i=0;i<3;i++)
		for (j=0;j<28;j++)
			for (k=0;k<15;k++)
			{
				Raise[i][j][k]=LimpIn[i][j][k]=temp;
			}
}
double Player::CalR()
{
	double HandStack=RemainStack;
	int max=desk.NowStack-player.NowStack;
	if(max!=0)
	return HandStack/max;
    return 100;
}
Cards Player::CominePlayerDeskCard()
{
	Cards c=HandCard;
	Cards c2=desk.DeskCard;
	unsigned int i;
	for (i=0;i<c2.element.size();i++)
		c.element.push_back(c2.element[i]);
	return c;
}

void Player::ErasePair(Cards &c)
{
	c.sort();
	unsigned int i;
	for (i=0;i<c.element.size()-1;i++)
		if (c.element[i].num==c.element[i+1].num)
		{
			c.element.erase(c.element.begin()+i,c.element.begin()+i+2);
			i=i-2;
		}
}
bool Player::CheckFullHouse()
{
	runlog<<"CheckTrip true"<<endl;
	Cards c=CominePlayerDeskCard();
	c.sort();
	unsigned int i;
	int s1=0;
	int s2=0;
	int count=0;
	for (i=0;i<c.element.size()-2;i++)
	{
		if (c.element[i].num==c.element[i+1].num && c.element[i+1].num==c.element[i+2].num)
		{
			c.element.erase(c.element.begin()+i,c.element.begin()+i+3);
			for(int j=0;j<c.element.size()-1;j++)
			{
				if(c.element[j].num==c.element[j+1].num)
					return true;
			}
	    }
		//return true;
	}
	return false;
}
bool Player::CheckOnePair()
{
	runlog<<"CheckOnePair true"<<endl;
	if (CheckTrip()) return true;
	Cards temp=desk.DeskCard;
	ErasePair(desk.DeskCard);
	Cards c=CominePlayerDeskCard();
	desk.DeskCard=temp;
	c.sort();
	unsigned int i;
	int count=0;
	for (i=0;i<c.element.size()-1;i++)
	{
		if (c.element[i].num==c.element[i+1].num) count++;
	}
	if (count>=1) return true;
	runlog<<" false"<<endl;
	return false;
}
bool Player::CheckTopPair()
{
	runlog<<"CheckTopPair true"<<endl;
	if (CheckTrip()) return true;
	Cards temp=desk.DeskCard;
	ErasePair(desk.DeskCard);
	Cards c=CominePlayerDeskCard();

	Cards d=desk.DeskCard;
	d.sort();
	if (d.element.size()==0) return false;
	int top=d.element[d.element.size()-1].num;

	desk.DeskCard=temp;

	c.sort();
	unsigned int i;
	for (i=0;i<c.element.size()-1;i++)
		if (c.element[i].num==c.element[i+1].num && c.element[i].num>=top) return true;
	runlog<<" false"<<endl;
	return false;

}

bool Player::CheckMidPair()
{
	runlog<<"CheckMidPair true"<<endl;
	if (CheckTrip()) return true;
	Cards temp=desk.DeskCard;
	ErasePair(desk.DeskCard);
	Cards c=CominePlayerDeskCard();

	Cards d=desk.DeskCard;
	d.sort();
	if (d.element.size()<2) return false;
	int mid=d.element[d.element.size()-2].num;

	desk.DeskCard=temp;

	c.sort();
	unsigned int i;
	for (i=0;i<c.element.size()-1;i++)
		if (c.element[i].num==c.element[i+1].num && c.element[i].num>=mid) return true;
	runlog<<" false"<<endl;
	return false;
}


bool Player::CheckTwoPair()
{
	runlog<<"CheckTwoPair true"<<endl;
	if (CheckTrip()) return true;
	Cards temp=desk.DeskCard;
	ErasePair(desk.DeskCard);
	Cards c=CominePlayerDeskCard();
	desk.DeskCard=temp;

	c.sort();
	unsigned int i;
	int count=0;
	for (i=0;i<c.element.size()-1;i++)
	{
		if (c.element[i].num==c.element[i+1].num) count++;
	}
	if (count>=2) return true;
	runlog<<" false"<<endl;
	return false;
}
bool Player::CheckTrip()
{
	runlog<<"CheckTrip true"<<endl;
	Cards c=CominePlayerDeskCard();
	c.sort();
	unsigned int i;
	int count=0;
	for (i=0;i<c.element.size()-2;i++)
	{
		if (c.element[i].num==c.element[i+1].num && c.element[i+1].num==c.element[i+2].num) return true;
	}
	runlog<<" false"<<endl;
	return false;
}
bool Player::CheckStraightDraw()
{
	runlog<<"CheckStraightDraw true"<<endl;
	Cards c=CominePlayerDeskCard();
	c.sort();
	if (c.element[c.element.size()-1].num==14) c.element.pop_back();

	unsigned int i;
	int count=0;
	for (i=0;i<c.element.size()-1;i++)
	{
		if (c.element[i].num+1==c.element[i+1].num) count++;
		if (c.element[i].num+1<c.element[i+1].num) count=0;
		if (count>=3) return true;
	}
	runlog<<" false"<<endl;
	return false;
}
bool Player::CheckSingleStraightDraw()
{
	runlog<<"CheckSingleStraightDraw true"<<endl;
	Cards c=CominePlayerDeskCard();
	c.sort();
	if (c.element[c.element.size()-1].num==14) c.element.insert(c.element.begin(),Card(1,1));
	unsigned int i;
	for (i=3;i<c.element.size();i++)
		if (c.element[i].num-c.element[i-3].num<=4) return true;
	runlog<<" false"<<endl;
	return false;
}
bool Player::CheckFlushDraw()
{
	Cards c=CominePlayerDeskCard();
	unsigned int i;
	int count[5]={0};
	bool result=false;
	for (i=0;i<c.element.size();i++)
		count[c.element[i].color]++;
	for (i=0;i<=4;i++)
		if (count[i]>=4) result=true;
	runlog<<" CheckFlushDraw "<< result<<endl;
	return result;
}
bool Player::CheckFour()
{
	runlog<<"CheckTrip true"<<endl;
	Cards c=CominePlayerDeskCard();
	c.sort();
	unsigned int i;
	int count=0;
	for (i=0;i<c.element.size()-3;i++)
	{
		if (c.element[i].num==c.element[i+1].num && c.element[i+1].num==c.element[i+2].num&&c.element[i+2].num==c.element[i+3].num) return true;
	}
	runlog<<" false"<<endl;
	return false;
}
bool Player::CheckFlush()
{
	Cards c=CominePlayerDeskCard();
	unsigned int i;
	int count[5]={0};
	bool result=false;
	for (i=0;i<c.element.size();i++)
		count[c.element[i].color]++;
	for (i=0;i<=4;i++)
		if (count[i]>=5) result=true;
	runlog<<" CheckFlushDraw "<< result<<endl;
	return result;
}
bool Player::CheckStraight()
{
	runlog<<"CheckSingleStraightDraw true"<<endl;
	Cards c=CominePlayerDeskCard();
	c.sort();
	unsigned int i;
	//c.element.insert(c.element.begin(),Card(1,1));
    for(i=1;i<c.element.size()-1;i++)
		if(c.element[i].num-c.element[i-1].num!=1)
	    return false;
	i=c.element.size()-1;
	if(c.element[i].num==14&&(c.element[i-1].num==5))
		return true;
	if(c.element[i].num-c.element[i-1].num==1)
		return true;
    return false;
}
int Player::DefineCardLevel(int threadhold)
{
	Cards c=CominePlayerDeskCard();
	int num;
	if (c.element.size()==6) num=Level::Select5From6(c);
	if (c.element.size()==7) num=Level::Select5From7(c);
	int result=num;
	if (result==1 && CheckTopPair())
	{
		result=2;
	}
	if (result==1)
	{
		if (CheckMidPair())
			result=1;
		else
			result=0;
	}
	if (result<threadhold) result=0;
	return result;
}
bool Player::CheckPotCommited()
{
	if (CheckTopPair() || CheckFlushDraw() || CheckStraightDraw() || (CheckSingleStraightDraw() && CheckOnePair()))
	{
		runlog<<"Check Pot Commited TRUE"<<endl;
	}
	else
		runlog<<"Check Pot Commited FALSE"<<endl;
	return CheckTopPair() || CheckFlushDraw() || CheckStraightDraw() || (CheckSingleStraightDraw() && CheckOnePair());
}

bool Player::CheckPairPotCommited()
{
	if (CheckOnePair() || CheckFlushDraw() || CheckStraightDraw())
	{
		runlog<<"Check Pot Commited TRUE"<<endl;
	}
	else
		runlog<<"Check Pot Commited FALSE"<<endl;
	return CheckTopPair() || CheckFlushDraw() || CheckStraightDraw() || (CheckSingleStraightDraw() && CheckOnePair());
}

vector<string> Player::split(string s)
{
	unsigned int i;
	vector<string> result;
	string temp;
	int re=0;
	for (i=0;i<s.length();i++)
	{
		if (s[i]==' ' || s[i]==',' || s[i]==';' || s[i]==13 || s[i]==10)
		{
			temp=s.substr(re,i-re);
			re=i+1;
			result.push_back(temp);
			while (s[i]==' ' || s[i]==',' || s[i]==';' || s[i]==13 || s[i]==10)
				i++;
		}
	}
	temp=s.substr(re,s.length());
	result.push_back(temp);
	return result;
}

void Player::ReadOpenLib(string FileName,SParam result[][15])
{

	int i,j;
	ifstream isOpenLib(FileName.c_str());

	if (!isOpenLib) return ;
	string temp;
	isOpenLib>>temp;
	vector<string> s;
	for (i=27;i>=2;i--)
	{
		isOpenLib>>temp;
		temp=temp.substr(2,temp.length());
		s=split(temp);
		if (s[0].length()==0)
		{
			i++;
			continue;
		}
		int count=0;
		for (j=14;j>=2;j--)
		{
			if (s[count][0]>=65 && s[count][0]<=97+25)
			{
				result[i][j].c=s[count][0];
				if (s[count].length()>1)
				{
					s[count]=s[count].substr(1,s[count].length());
					result[i][j].num=atof(s[count].c_str());
				}
				else
					result[i][j].num=0;
			}
			else
			{
				result[i][j].num=0;
				result[i][j].c=0;
			}
			count++;
		}
	}
}
void Player::ReadExcel()
{
	ClearPlayerData();
	int k,p;
	ifstream is_r,is_l,is;
	string s;
	string temp;
	k=pos;
	for (p=0;p<=2;p++)
	{
		s="../source/csv/P";s+=(char)(k+49);s+="_";s+=(char)(p+48);s+="_Raise.csv";
	 	ReadOpenLib(s,Raise[p]);
		s="../source/csv/P";s+=(char)(k+49);s+="_";s+=(char)(p+48);s+="_LimpIn.csv";
		ReadOpenLib(s,LimpIn[p]);
		s="../source/csv/P";s+=(char)(9+48);s+="_";s+=(char)(p+48);s+="_Raise.csv";
		ReadOpenLib(s,Special_Raise[p]);
		s="../source/csv/P";s+=(char)(9+48);s+="_";s+=(char)(p+48);s+="_LimpIn.csv";
		ReadOpenLib(s,Special_LimpIn[p]);
	}
}


