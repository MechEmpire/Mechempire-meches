﻿#include "RobotAI.h"
#include<math.h>
#include<iostream>
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

struct node
{
	double x,y,dis;
};

double max_num(double a,double b)
{
	return a>b?a:b;
}
void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	int enery;
	if(myID==1)
		enery=0;
	else
		enery=1;

	int x=info.robotInformation[myID].circle.x;
	int y=info.robotInformation[myID].circle.y;
	int x1=info.robotInformation[enery].circle.x;
	int y1=info.robotInformation[enery].circle.y;
	int x2=300;
	int y2=250;
	int x3=1066;
	int y3=430;
	int d=120;
	int dis;
	int ddx;
	int ddy;

	if(myID==1)
	{
		dis=(int)sqrt((x3-x1)*(x3-x1)+(y3-y1)*(y3-y1));
		ddx=(120*(x3-x1))/dis+x3;
		ddy=(120*(y3-y1))/dis+y3;
	}
	else
	{
		dis=(int)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
		ddx=x2-dis*(x1-x2)/120;
		ddy=y2-dis*(y1-y2)/120;
	}


	int dx=(x1-x);
	int dy=(y1-y);
	double k=Random0_1();
	double kk,kk1;
	if(myID==1)
	{
		kk=(y1-430)/max_num(0.01,(x1-1066));
		kk1=(430-y)/(max_num(0.01,(1066-x)));
	}
	else
	{
		kk=(y1-250)/max_num(0.01,(x1-300));
		kk1=(250-y)/(max_num(0.01,(300-x)));
	}
	//1234 左右上下
	if(k<0.5)
	{
		if(ddx>x)
			order.run=2;
		else 
			order.run=1;
	}
	else
	{
		if(ddy>y)
			order.run=4;
		else
			order.run=3;
	}
	if(myID==1)
	{
		if(info.robotInformation[myID].circle.x>1066+d)
			order.run=1;
		if(info.robotInformation[myID].circle.x<1066-d)
			order.run=2;
		if(info.robotInformation[myID].circle.y>430+d)
			order.run=3;
		if(info.robotInformation[myID].circle.y<430-d)
			order.run=4;
	}
	if(myID==0)
	{
		if(info.robotInformation[myID].circle.x>300+d)
			order.run=1;
		if(info.robotInformation[myID].circle.x<300-d)
			order.run=2;
		if(info.robotInformation[myID].circle.y>250+d)
			order.run=3;
		if(info.robotInformation[myID].circle.y<250-d)
			order.run=4;
	}

	double kiss=Random0_1();
	if(info.robotInformation[myID].remainingAmmo==0)
	{
		for(int i=0;i<2;i++)
		{
			if(info.arsenal[i].respawning_time==0)
			{
				if(kiss<0.5)
				{
					if(info.arsenal[i].circle.x+30>info.robotInformation[myID].circle.x)
					{
						order.run=2;
						break;
					}
					if(info.arsenal[i].circle.x+30<info.robotInformation[myID].circle.x)
					{
						order.run=1;
						break;
					}
				}
				else
				{
					if(info.arsenal[i].circle.y+30>info.robotInformation[myID].circle.y)
					{
						order.run=4;
						break;
					}
					if(info.arsenal[i].circle.y+30<info.robotInformation[myID].circle.y)
					{
						order.run=3;
						break;
					}	
				}
			}
		}
	}

	double ck;
	Beam shoot;
	double ck1=info.robotInformation[myID].weaponRotation;
	shoot.x=x;
	shoot.y=y;
	shoot.rotation=ck1;
	Circle kis;
	if(myID==1)
	{
		kis.x=1066;
		kis.y=430;
		kis.r=100;
		if(HitTestBeamCircle(shoot,kis))
			order.wturn=1;
	}
	else
	{
		kis.x=300;
		kis.y=250;
		kis.r=80;
		if(HitTestBeamCircle(shoot,kis))
			order.wturn=1;
	}


	if(myID==0)
		order.fire=2;
	else 
		order.fire=1;
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

	weapon = WT_MissileLauncher;
	engine = ET_Spider;
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "bird_raincoatV0.19";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "qscqesze";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return 255;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return -69;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return 31;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return -255;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return -106;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return -4;
}









//-----------------------------------------------------
//3.用不用随你的触发函数
//-----------------------------------------------------


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标

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