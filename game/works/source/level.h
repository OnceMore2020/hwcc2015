#pragma once
#include "Card.h"
#include <vector>
#include <iostream>
#include<fstream>
//º∆À„≈∆¡¶÷µ
using namespace std;
class Level
{	
public:
	static int Select5From7(Cards c);
	static int Select5From6(Cards c);
	static int DefineLevel(Cards c);
	Level(void);
	~Level(void);
};


