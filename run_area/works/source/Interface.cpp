#include "Interface.h"
#include "Desk.h"
#include "Control.h"
#include "stdlib.h"
#include <fstream>
Interface::Interface(string my_id):gameNum(0)
{
	infor.open("information.txt",ios::app);
	hand_card_num = 0;				//手牌总数
	player_num = 0;					//玩家总数
	//启动
	public_card_num = 0;
	hand_card_num = 0;
	player_num = 0;
	TempPot = 0;
	//small_blind_pot_index = -1;
	//big_blind_pot_index = -1;
	self_pos = -1;
	real_player_num = 8;
    self_id = my_id;
}
Interface::~Interface()
{
	infor.close();
}
// 字符串分割函数
vector<string> Interface::split(string str,string pattern) {
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();
    for(int i=0;i<size;i++) {
        pos=str.find(pattern,i);
            if(pos<size) {
                std::string s=str.substr(i,pos-i);
                result.push_back(s);
                i=pos;
            }
       }
    return result;
}
int Interface::proSeat(const vector<string>& msgLines, int i) {//处理座次信息
    runlog<<"****************************************************************************GameNum:"<<++gameNum<<"****************************************************************************"<<endl;
	Desk::BigBlind=100;
	TempScene.clear();
    TempMoney.clear();
    idTable.clear();
    hand_card_info.clear();
    public_card_info.clear();
    hand_public_cards.element.clear();
    string buttonPid;int buttonJetton;
    for (; msgLines[++i].substr(0,3) != "/se";) {
        std::vector<std::string> lineInfo = split(msgLines[i], string(" ")); //按空格分割
        if (msgLines[i].substr(0,2)=="bu") {
            buttonPid = lineInfo[1];
            buttonJetton = atoi(lineInfo[2].c_str());
            if (lineInfo[1].substr(0,4) == self_id){
                self_pos = 7;
            }
        }
        else if (msgLines[i].substr(0,2)=="sm") {
            TempScene.push_back(50);
            TempMoney.push_back(atoi(lineInfo[3].c_str()));
            idTable.push_back(lineInfo[2]);
            if (lineInfo[2].substr(0,4) == self_id){
                self_pos = 0;
            }
        }
        else if (msgLines[i].substr(0,2)=="bi") {
            TempScene.push_back(100);
            TempMoney.push_back(atoi(lineInfo[3].c_str()));
            idTable.push_back(lineInfo[2]);
            if (lineInfo[2].substr(0,4) == self_id){
                self_pos = 1;
            }
        }
        else {
            TempScene.push_back(0);
            TempMoney.push_back(atoi(lineInfo[1].c_str()));
            idTable.push_back(lineInfo[0]);
            if (lineInfo[0].substr(0,4) == self_id){
                self_pos = TempScene.size()-1;
            }
        }
    }
    player_num = TempScene.size()+1;
    taken_money=TempMoney[self_pos];
    if (TempScene.size()==7) {
        TempScene.push_back(0);
        TempMoney.push_back(buttonJetton);
        idTable.push_back(buttonPid);
    }
    else {
        for(int j=TempScene.size();j<7;j++) {//被淘汰的填-1,-1
            TempScene.push_back(-1);
            TempMoney.push_back(-1);
            idTable.push_back("-1");
        }
        TempScene.push_back(0);
        TempMoney.push_back(buttonJetton);
        idTable.push_back(buttonPid);
    }
    // 打印测试
    runlog << "Interface::--Seat Info--" << endl;
    runlog << "Interface::self pos: " << self_pos << endl;
	desk.Initial(self_pos,player_num,taken_money,TempScene);
    for(int j=0;j<TempScene.size();j++)
	{
       runlog << TempScene[j] << " " << TempMoney[j] << " " << idTable[j] << endl;
       cout << TempScene[j] << " " << TempMoney[j] << " " << idTable[j] << endl;
	}
    return i;
}
int Interface::proBlind(const vector<string>& msgLines, int i) {//处理盲注信息
    for(;msgLines[++i].substr(0,3)!="/bl";)
    return i;
}
int Interface::proHold(const vector<string>& msgLines, int i) {//处理手牌消息
    Card temp; //临时存放手牌
    string colorTable[] = {"SPADES","HEARTS","CLUBS","DIAMONDS"};
    string pointTable[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    for(;msgLines[++i].substr(0,3)!="/ho";) {
        std::vector<std::string> lineInfo = split(msgLines[i], string(" ")); //按空格分割
        for(int k=0;k<4;k++){
            if (lineInfo[0]==colorTable[k])
                temp.color = k;
        }
        for(int k=0;k<13;k++){
            if (lineInfo[1]==pointTable[k])
                temp.num = k+2;
        }
        hand_card_info.push_back(temp);
        hand_public_cards.element.push_back(temp);
    }
    // 打印测试
    runlog << "Interface::--Hand Card--" << endl;
    for(int j=0;j<hand_card_info.size();j++)
        cout << hand_card_info[j].num <<" " << hand_card_info[j].color << endl;
    return i;
}
int Interface::proInquire(const vector<string>& msgLines, int i) {//处理询问消息
    for(;msgLines[++i].substr(0,3)!="tot";) {
        std::vector<std::string> lineInfo = split(msgLines[i], string(" ")); //按空格分割
        cout<<"msgLines"<<msgLines[i]<<endl;
        cout<<"lineInfo"<<lineInfo[0]<<endl;
		for (int k=0;k<idTable.size();k++) {
            if (lineInfo[0]==idTable[k]){
                cout << lineInfo[0] << " " << idTable[k] << " " <<atoi(lineInfo[3].c_str())<<endl;
                TempScene[k] = atoi(lineInfo[3].c_str());
                cout << TempScene[k] << "----" << endl;
                if (lineInfo[4]=="fold") {
                    TempScene[k] = -1;
                    cout << TempScene[k] <<" "<< idTable[k]<<endl;
                }
            }
        }
    }
    std::vector<std::string> lineInfo = split(msgLines[i++], string(" ")); //按空格分割
    TempPot=atoi(lineInfo[2].c_str());
    //打印测试
    cout << "Interface::--Inquire Info--" << endl;
    for(int j=0;j<TempScene.size();j++)
        cout <<"Interface::TempScene:"<<TempScene[j] << " TempMoney" << TempMoney[j] <<"  idTable"<<idTable[j]<< endl;
    cout<<TempScene.size();
	cout << "---" << TempPot << "----" << endl;
    return i;
}
int Interface::proFlop(const vector<string>& msgLines, int i) {//处理公牌信息
    Card temp; //临时存放公牌
    string colorTable[] = {"SPADES","HEARTS","CLUBS","DIAMONDS"};
    string pointTable[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    for(;msgLines[++i].substr(0,3)!="/fl";) {
        std::vector<std::string> lineInfo = split(msgLines[i], string(" ")); //按空格分割
        for(int k=0;k<4;k++){
            if (lineInfo[0]==colorTable[k])
                temp.color = k;
        }
        for(int k=0;k<13;k++){
            if (lineInfo[1]==pointTable[k])
                temp.num = k+2;
        }
        public_card_info.push_back(temp);
        hand_public_cards.element.push_back(temp);
    }
    // 打印测试
    runlog << "Interface::--Public Card--" << endl;
    for (int j=0;j<public_card_info.size();j++) {
        runlog << public_card_info[j].num << " " << public_card_info[j].color << endl;
    }
    runlog << "Interface::--hand+public--" << endl;
    for (int j=0;j<hand_public_cards.element.size();j++) {
        runlog << hand_public_cards.element[j].num << " " << hand_public_cards.element[j].color << endl;
    }
    return i;
}
int Interface::proTurn(const vector<string>& msgLines, int i) {//处理转牌信息
    Card temp; //临时存放转牌
    string colorTable[] = {"SPADES","HEARTS","CLUBS","DIAMONDS"};
    string pointTable[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    std::vector<std::string> lineInfo = split(msgLines[++i], string(" ")); //按空格分割
    for(int k=0;k<4;k++){
        if (lineInfo[0]==colorTable[k])
            temp.color = k;
    }
    for(int k=0;k<13;k++){
        if (lineInfo[1]==pointTable[k])
            temp.num = k+2;
    }
    public_card_info.push_back(temp);
    hand_public_cards.element.push_back(temp);
    // 打印测试
    runlog << "Interface::--Public Card--" << endl;
    for (int j=0;j<public_card_info.size();j++) {
        runlog << public_card_info[j].num << " " << public_card_info[j].color << endl;
    }
    runlog << "Interface::--hand+public--" << endl;
    for (int j=0;j<hand_public_cards.element.size();j++) {
        runlog << hand_public_cards.element[j].num << " " << hand_public_cards.element[j].color << endl;
    }
    ++i;
    return i;
}
int Interface::proRiver(const vector<string>& msgLines, int i) {//处理河牌消息
    Card temp; //临时存放河牌
    string colorTable[] = {"SPADES","HEARTS","CLUBS","DIAMONDS"};
    string pointTable[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    std::vector<std::string> lineInfo = split(msgLines[++i], string(" ")); //按空格分割
    for(int k=0;k<4;k++){
        if (lineInfo[0]==colorTable[k])
            temp.color = k;
    }
    for(int k=0;k<13;k++){
        if (lineInfo[1]==pointTable[k])
            temp.num = k+2;
    }
    public_card_info.push_back(temp);
    hand_public_cards.element.push_back(temp);
    // 打印测试
    runlog << "Interface::--Public Card--" << endl;
    for (int j=0;j<public_card_info.size();j++) {
        runlog << public_card_info[j].num << " " << public_card_info[j].color << endl;
    }
    runlog << "Interface::--hand+public--" << endl;
    for (int j=0;j<hand_public_cards.element.size();j++) {
        runlog << hand_public_cards.element[j].num << " " << hand_public_cards.element[j].color << endl;
    }
    ++i;
    return i;
}
int Interface::proShowdown(const vector<string>& msgLines, int i) {//处理摊牌消息
    std::vector<std::string> showdownInfo(13);
    int j=0;++i;
    for(;msgLines[++i].substr(0,3)!="/co";) {//存5张公共牌
        std::vector<std::string> lineInfo = split(msgLines[i], string(" ")); //按空格分割
        showdownInfo[j++]=lineInfo[0] + " " + lineInfo[1];
    }
    for(;msgLines[++i].substr(0,3)!="/sh";) {//存排名信息
        std::vector<std::string> lineInfo = split(msgLines[i], string(" ")); //按空格分割
        showdownInfo[j++]=lineInfo[0].substr(0,1) + " " + lineInfo[1] + " "+lineInfo[2] + " " + lineInfo[3] + " "+
            lineInfo[4] + " "+lineInfo[5]+" "+lineInfo[6];
    }
    runlog << "Interface::--Showdown--" << endl;
    for(int j=0;j<showdownInfo.size();j++)
        runlog << showdownInfo[j] << endl;
    return i;
}
int Interface::proPotWinMsg(const vector<string>& msgLines, int i) {//处理彩池分配信息
    std::vector<std::string> potwinInfo(13);
    int j=0;
    for(;msgLines[++i].substr(0,3)!="/po";) {//彩池分配信息
        std::vector<std::string> lineInfo = split(msgLines[i], string(" ")); //按空格分割
        potwinInfo[j++]=lineInfo[0].substr(0,4) + " " + lineInfo[1];
    }
    runlog << "Interface::--Pot-Win-Msg--" << endl;
    for(int j=0;j<potwinInfo.size();j++)
        runlog <<"Interface::"<< potwinInfo[j] << endl;
    return i;
}
int Interface::proNotify(const vector<string>& msgLines, int i) {//处理通知信息
    for(;msgLines[++i].substr(0,3)!="tot";) {
        std::vector<std::string> lineInfo = split(msgLines[i], string(" ")); //按空格分割
        for (int k=0;k<idTable.size();k++) {
           // if (idTable[k]==lineInfo[0])
            //    TempScene[k] = atoi(lineInfo[3].c_str());
           // if (lineInfo[4]=="fold")
              //  TempScene[k] = -1;
        }
    }
    std::vector<std::string> lineInfo = split(msgLines[i++], string(" ")); //按空格分割
    TempPot=atoi(lineInfo[2].c_str());
    //打印测试
    runlog<<"Interface::" << "--Notify Info--" << endl;
    for(int j=0;j<TempScene.size();j++)
        runlog << "Interface::"<<TempScene[j] << " " << TempMoney[j] << endl;
    runlog <<"Interface::"<< "---" << TempPot << "----" << endl;
    return i;
}
// 监听到消息后的处理函数
int Interface::msgProcess(const string& msgStr) {
    runlog <<"Interface::"<< msgStr << "-----" << endl << endl;
    std::vector<std::string> msgLines = split(msgStr, string("\n")); //按行分割
    int curLine=0;
    for(;curLine<msgLines.size();) {
        if (msgLines[curLine].substr(0,2)=="se")
            curLine = proSeat(msgLines,curLine);
        else if (msgLines[curLine].substr(0,2)=="bl")
            curLine = proBlind(msgLines,curLine);
        else if (msgLines[curLine].substr(0,2)=="ho")
            curLine = proHold(msgLines,curLine);
        else if (msgLines[curLine].substr(0,2)=="in") {
			cout<<"Interface::"<<msgStr<<"----"<<endl<<endl;
            int action=0;
            curLine = proInquire(msgLines,curLine);
		    desk.Update(TempScene,TempMoney,TempPot,hand_public_cards,taken_money);
            Control::Anaysis(action,raiseNum);
            return action;
        }
        else if (msgLines[curLine].substr(0,2)=="fl")
            curLine = proFlop(msgLines,curLine);
        else if (msgLines[curLine].substr(0,2)=="tu")
            curLine = proTurn(msgLines,curLine);
        else if (msgLines[curLine].substr(0,2)=="ri")
            curLine = proRiver(msgLines,curLine);
        else if(msgLines[curLine].substr(0,2)=="sh")
            curLine = proShowdown(msgLines,curLine);
        else if (msgLines[curLine].substr(0,2)=="po")
            curLine = proPotWinMsg(msgLines,curLine);
        else if (msgLines[curLine].substr(0,2)=="no")
            curLine = proNotify(msgLines,curLine);
        else if (msgLines[curLine].substr(0,2)=="ga")
            return -1;
        curLine++;
    }
    return 5;
}
//利用这四个函数进行与服务器信息的操作
void Interface::operation_all_in(int num)
{

}
void Interface::operation_raise(int num)
{

}
void Interface::operation_call(int num)
{

}
void Interface::operation_check()
{

}
void Interface::operation_fold()
{

}
//从服务器获取信息,并保存到相应的数据中
/*void Interface::get_information()
{

}*/
bool Interface::is_my_operation_now()
{

	return false;
}
//保存与服务器通信后的信息
void Interface::save_deskInformation_log()
{


}
bool Interface::is_new_turn_start()
{
	//
	return false;
}
/*void Interface::parameter_setting()
{

	//一.牌参数设置
	hand_public_cards.element.clear();
	TempScene.clear();
	TempMoney.clear();
	//将数据更新
	is_parameter_updated=true;
}*/
/*
void Interface::main_process()
{
	desk.Update(TempScene,TempMoney,TempPot,hand_public_cards,taken_money);
    Control::Anaysis(action,raisenum);
}*/
