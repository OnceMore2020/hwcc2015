#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "Interface.h"
#include "Control.h"
using namespace std;

bool msgProcess(const string&); //监听到消息后的处理函数

int main(int argc, char *argv[])
{
    if (argc != 6) {
        printf("Usage: ./%s server_ip server_port my_ip my_port my_id\n", argv[0]);
        return -1;
    }
    // 初始化Log文件
    Control::Initial();

    // 套接字寻址
    in_addr_t serverIp = inet_addr(argv[1]);
    in_port_t serverPort = htons(atoi(argv[2]));
    in_addr_t clientIp = inet_addr(argv[3]);
    in_port_t clientPort = htons(atoi(argv[4]));
    int clientId = atoi(argv[5]);

    // 创建客户端套接字
    int clientSocket = socket(AF_INET,SOCK_STREAM,0);
    if( clientSocket < 0) {
        printf("Create Client Socket failed...\n");
        return -1;
    }

    // 设置套接字选项，地址重复使用，防止程序非正常退出，下次启动时bind失败
    int is_reuse_addr = 1;
    setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&is_reuse_addr, sizeof(is_reuse_addr));

    // 绑定客户端socket和地址
    struct sockaddr_in clientAddr;
    bzero(&clientAddr, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = clientIp;
    clientAddr.sin_port = clientPort;
    if( bind(clientSocket,(struct sockaddr*)&clientAddr,sizeof(clientAddr))) {
        printf("Client Bind Port Failed!\n");
        return -1;
    }

    // 连接服务器
    struct sockaddr_in serverAddr;
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = serverIp;
    serverAddr.sin_port = serverPort;
    while(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        usleep(100*1000);
        printf("重试connect\n");
    }
    printf("连接成功\n");

    // 注册
    char regMsg[50] = {'\0'};
    snprintf(regMsg, sizeof(regMsg) - 1, "reg: %d %s need_notify \n", clientId, "hehe");
    send(clientSocket, regMsg, strlen(regMsg) + 1, 0);

    // 开始游戏
    string parameter;
    parameter = argv[5];
    Interface gameInfo(parameter);
    while(1) {
        string msgStr;
        char msgBuf[1024] = {'\0'};
        int length = recv(clientSocket, msgBuf, sizeof(msgBuf) - 1, 0);
        if(length > 0) {
            msgStr = string(msgBuf);
        }
        // processRes取值含义：
        // 0 : fold
        // 1 : check
        // 2 : call
        // 3 : raise
        // 4 : all_in
        // 5 : 无操作
        // -1 : game_over
        int processRes = gameInfo.msgProcess(msgStr); //处理消息，返回结果
        if (processRes==1) {//check
            char checkMsg[50] = {'\0'};
            snprintf(checkMsg, sizeof(checkMsg) - 1, "check \n");
            send(clientSocket, checkMsg, strlen(checkMsg) + 1, 0);
        }
        if (processRes==2) {//call
            char callMsg[50] = {'\0'};
            snprintf(callMsg, sizeof(callMsg) - 1, "call \n");
            send(clientSocket, callMsg, strlen(callMsg) + 1, 0);
        }
        if (processRes==3) {//raise
            char raiseMsg[50] = {'\0'};
            snprintf(raiseMsg, sizeof(raiseMsg) - 1, "raise %d \n", gameInfo.raiseNum);
            send(clientSocket, raiseMsg, strlen(raiseMsg) + 1, 0);
        }
        if (processRes==4) {//all_in
            char all_inMsg[50] = {'\0'};
            snprintf(all_inMsg, sizeof(all_inMsg) - 1, "all_in \n");
            send(clientSocket, all_inMsg, strlen(all_inMsg) + 1, 0);
        }
        if (processRes==0) {//fold
            char foldMsg[50] = {'\0'};
            snprintf(foldMsg, sizeof(foldMsg) - 1, "fold \n");
            send(clientSocket, foldMsg, strlen(foldMsg) + 1, 0);
        }
        if (processRes==-1) {
            cout << "gameover" << endl;
            break;
        }
    }
    //关闭socket
    close(clientSocket);
    return 0;
}
