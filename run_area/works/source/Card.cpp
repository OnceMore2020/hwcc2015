#include "Card.h"

Card::Card()
{}

Card::Card(int n,int c)
{
    num=n;
    color=c;
}

Card::~Card(void)
{}

Cards::Cards(void)
{}

Cards::~Cards(void)
{}

void Cards::sort()
{
    int l=element.size();
    int i,j;
    Card temp;
    for (i=0;i<l;i++)
        for (j=i+1;j<l;j++)
        {
			if (element[i].num>element[j].num || (element[i].num==element[j].num && element[i].color>element[j].color))
            {
                temp=element[i];element[i]=element[j];element[j]=temp;
            }
        }

}
 bool Cards::operator==(Cards &c1)
 {
	 c1.sort();
	 this->sort();
	 if (element.size()!=c1.element.size()) return false;
	 unsigned int i;
	 for (i=0;i<element.size();i++)
		 if (element[i].num!=c1.element[i].num || element[i].color!=c1.element[i].color) return false;
	 return true;  
 }