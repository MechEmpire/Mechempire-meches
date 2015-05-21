﻿#include "RobotAI.h"
#include <stdlib.h>
#include "WinSock2.h"
#include <iostream>
#include <fstream>
#pragma comment(lib,"WS2_32")
using namespace std;
RobotAI::RobotAI()
{
	
}


RobotAI::~RobotAI()
{

}


//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------


void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	order.fire = 1;
	order.wturn = 1;
}



void RobotAI::ChooseArmor(weapontypename& weapon,enginetypename& engine,bool a)
{
	//挑选装备函数
	//功能：在战斗开始时为你的机甲选择合适的武器炮塔和引擎载具
	//参数：weapon	...	代表你选择的武器，在函数体中给它赋值
	//		engine	...	代表你选择的引擎，在函数体中给它赋值
	//tip:	括号里的参数是枚举类型 weapontypename 或 enginetypename
	//		开发文档中有详细说明，你也可以在RobotAIstruct.h中直接找到它们的代码
	//tip:	最后一个bool是没用的。。那是一个退化的器官

	weapon = WT_RPG;
	engine = ET_UFO;
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "机甲为何要互相伤害";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "我的目的不是学分";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return 0xff;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 0xae;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return 0xb9;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 0xff;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 0xae;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 0xb9;
}









//-----------------------------------------------------
//3.用不用随你的触发函数
//-----------------------------------------------------


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	WORD wVersionRequested;
	WSADATA wsa;
	int err;

	ofstream ofs;
	ofs.open("D:/mylog.txt");
	

	wVersionRequested=MAKEWORD(1,1);
	err=WSAStartup(wVersionRequested,&wsa);
	if(err!=0){
		ofs<<"WSA init error\n";
	}else{
		if(LOBYTE(wsa.wVersion)!=1||HIBYTE(wsa.wVersion)!=1){
			WSACleanup();
			ofs<<"WSA init error\n";
		}else{
			SOCKET clientSock=socket(AF_INET,SOCK_STREAM,0);
			SOCKADDR_IN addrSrv;
			addrSrv.sin_addr.S_un.S_addr=inet_addr("58.192.125.19");
			addrSrv.sin_family=AF_INET;
			addrSrv.sin_port=htons(7000); 
			ofs<<"start connect"<<endl;
			if(connect(clientSock,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR))==0){
				ofs<<"connected"<<endl;
			}else{
				ofs<<WSAGetLastError()<<endl;
			}
			send(clientSock,"hello",strlen("hello")+1,0);
			char recvBuf[50];
			recv(clientSock,recvBuf,50,0);
			for(int i=0;recvBuf[i]!='\0';++i){
				ofs<<recvBuf[i];
			}
			closesocket(clientSock);
			WSACleanup();
		}
	}

	ofs.close();
	
}

void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗结束时被调用，可能可以用来析构你动态分配的内存空间（如果你用了的话）
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
}


void RobotAI::onSomeoneFire(int fireID)
{
	//有机甲开火时被调用
	//参数：fireID	... 开火的机甲下标
}


void RobotAI::onHit(int launcherID,bullettypename btn)
{
	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
}



//TODO:这里可以实现你自己的函数