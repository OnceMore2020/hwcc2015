#pragma once
#include <vector>
#include "Card.h"
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class Cards;

//struct card
//{
//	int num;
//	int color;
//};
class Interface
{
public:
	Interface(string);
	~Interface(void);
    int msgProcess(const string&); //监听到消息后的处理函数
    vector<string> split(string,string); //字符串分割函数
    int proSeat(const vector<string>&, int);//处理座次信息
    int proBlind(const vector<string>&, int);//处理盲注消息
    int proHold(const vector<string>&, int);//处理手牌消息
    int proInquire(const vector<string>&, int);//处理询问消息
    int proFlop(const vector<string>&, int);//处理公牌消息
    int proTurn(const vector<string>&, int);//处理转牌消息
    int proRiver(const vector<string>&, int);//处理河牌消息
    int proShowdown(const vector<string>&, int);//处理摊牌消息
    int proPotWinMsg(const vector<string>&, int);//处理彩池分配消息
    int proNotify(const vector<string>&, int);//处理通知消息
	bool is_my_operation_now();  //是否轮到我操作
	bool is_new_turn_start();
	void parameter_setting();
	void new_game_start();  //开始新的一局
	void operation_fold();
	void operation_check();
	void operation_call(int);
	void operation_raise(int);
	void operation_all_in(int);
	void main_process();
	void initial();
	void get_information();//从服务器端获取信息
	void save_deskInformation_log(); //保存相应的信息
    int raiseNum; //若msgProcess返回raise,则raise这么多
private:
	int gameNum;//牌局数
	bool New_Street_Start;
	vector<Card> hand_card_info;	//手牌 2-14对应与2-A，0-4对应四种颜色
	vector<Card> public_card_info; //公共牌
	Cards hand_public_cards;
	bool is_parameter_updated;
	int hand_card_num;		//手牌的张数
	int public_card_num; //公共牌的数量
	int player_num;			//玩家的个数
	int small_blind_pot_index;	//小盲注位置
	int big_blind_pot_index;	//大盲注位置
    vector<string> idTable; //按ID存座次
	int TempPot;//目前锅的大小
	vector<int> TempScene,TempStatus,TempMoney;//TempScence每个玩家的下注量，TempStatus每个玩家的状态，是否已经弃牌
	//TempMoney每个玩家的代入资金量
	//小盲为第一个位置，依次
	int taken_money;  //玩家的资金代入量
	int self_pos;  //玩家的位置，小盲的位置是0，依次为大盲1，
    string self_id;
	int real_player_num;   //目前在玩的玩家数量
    ofstream infor;
};


