#include "level.h"
int Level::DefineLevel(Cards c)
{
    int i;
    int sameNumber1=0,sameCount1=2;
    int sameNumber2=0,sameCount2=2;
    c.sort();
	bool Straight=true,Flush=true;
   for (i=1;i<5;i++)
   if (c.element[i].color!=c.element[i-1].color)
   {
          Flush=false;
          break;
   }
   for (i=1;i<5;i++)
   if (!((c.element[i].num-1==c.element[i-1].num) || ((i==4) && c.element[i].num-9==c.element[i-1].num)))
   {
      Straight=false;
      break;
  }
  if (Straight && Flush)
  {
     if (c.element[0].num==1 && c.element[1].num==10) return 9;
     else return 8;
  }
  if (Straight) return 4;
  if (Flush) return 5;
  return 0;
}
int Level::Select5From7(Cards c)
{
	int i,j;
	int max=0;
	Cards temp;
	int level;
	if (c.element.size()==6) return Select5From6(c);
	for (i=0;i<=5;i++)
		for (j=i+1;j<=6;j++)
		{
			temp=c;
			temp.element.erase(temp.element.begin()+j);
			temp.element.erase(temp.element.begin()+i);
			level = DefineLevel(temp);
			if (level>max) max=level;		
		}
	return max;
}

int Level::Select5From6(Cards c)
{
	int i;
	int max=0;
	Cards temp;
	int level;
	for (i=0;i<=5;i++)
	{
		temp=c;
		temp.element.erase(temp.element.begin()+i);
		level = DefineLevel(temp);
		if (level>max) max=level;		
	}
	return max;
}
Level::Level(void)
{

}
Level::~Level(void)
{

}




