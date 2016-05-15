#pragma once
#include<vector>
using namespace std;
//ÅÆ
class Card
{
public:
    int num;
    int color;
    Card();
    Card(int n,int c);
    ~Card(void);
};
class Cards
{
public:
	bool operator==(Cards &c1);
    vector<Card> element;
    void sort();
    Cards();
    ~Cards(void);
};
