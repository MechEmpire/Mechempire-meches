#include "RobotAI.h"
#include "math.h"
#include <cmath>
#include "time.h"
#include <iostream>


int RobotAI::time1=0;

RobotAI::RobotAI()
{

}
RobotAI::~RobotAI()
{

}


//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------
int RobotAI::gettime()
{
	return time1;
}
/*
void RobotAI::zds(RobotAI_RobotInformation &bullet)
{
	zidan=bullet.remainingAmmo;
}
int RobotAI::getzidan(RobotAI_RobotInformation &bullet){
	return bullet.remainingAmmo;
}*/
void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	auto wo = info.robotInformation[myID];
	auto ta = info.robotInformation[1 - myID];

	double wo_x=wo.circle.x; 
	double wo_y=wo.circle.y;
	double ta_x=ta.circle.x;
	double ta_y=ta.circle.y;
	double dis_x=ta.circle.x-wo.circle.x;
	double dis_y=ta.circle.y-wo.circle.y;
	
	double dis=fabs(dis_x*dis_x)+fabs(dis_y*dis_y);	
	double tan = atan2(dis_y,dis_x)*60;
		
	
/*	time_t timee=time(NULL);
	int time2=timee;
	int s=time2-gettime();
	if (s>2)
	{order.fire=1;
	} 
	else
	{order.fire=0;
	}
*/
	time1++;
	if ((time1>140)&&(dis<800000))
	{
		if ((wo_x>ta_x)&&(wo_y>ta_y)&&(ta_x<300)&&(ta_y<250))
		{
			order.fire=0;
		} 
		else if ((wo_x<ta_x)&&(wo_y<ta_y)&&(ta_x>1400)&&(ta_y>640))
		{
			order.fire=0;
		}
		else
		{
			order.fire=1;
		}

	} 
	else
	{
		order.fire=0;
	}
	if (time1>60)
	{order.run=1;
	}
		
	if((time1<250)||(time1>640)){
		
		if (dis>150000)
		{
		
		double w_Rotation = tan - wo.weaponRotation;
		double e_Rotation=tan-wo.engineRotation;
		while(w_Rotation>180)w_Rotation-=360;	
		while(w_Rotation<=-180)w_Rotation+=360;
		if (w_Rotation<-3.2) 
			order.wturn = -1;
		else if (w_Rotation>2)
			order.wturn = 1;	
		if (e_Rotation<-3.3) 
			order.eturn = -1;
		else if (e_Rotation>2)
			order.eturn = 1;	
		}
	    else {

		double dis_x2=50-wo.circle.x;
		double dis_y2=630-wo.circle.y;
			
		double tan2 = atan2(dis_y2,dis_x2)*60;


		double w_Rotation = tan - wo.weaponRotation;
		double e_Rotation2=tan2-wo.engineRotation;

		while(w_Rotation>180)w_Rotation-=360;	
		while(w_Rotation<=-180)w_Rotation+=360;
		if (w_Rotation<-2) 
			order.wturn = -1;
		else if (w_Rotation>2)
			order.wturn = 1;	

		if (e_Rotation2<-2) 
			order.eturn = 1;
	   else if (e_Rotation2>2)
			order.eturn = -1;	
        }	
	}
	else{

		double dis_x2=50-wo.circle.x;
		double dis_y2=630-wo.circle.y;

		double tan2 = atan2(dis_y2,dis_x2)*60;


		double w_Rotation = tan - wo.weaponRotation;
		double e_Rotation2=tan2-wo.engineRotation;

		while(w_Rotation>180)w_Rotation-=360;	
		while(w_Rotation<=-180)w_Rotation+=360;
		if (w_Rotation<-2) 
			order.wturn = -1;
		else if (w_Rotation>2)
			order.wturn = 1;	

		if (e_Rotation2<-2) 
			order.eturn = -1;
		else if (e_Rotation2>2)
			order.eturn = 1;	
	}
	
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

	weapon = WT_Machinegun;	//啊，我爱加农炮
	engine = ET_Robotman  ;	//啊，我爱小蜘蛛
}

//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "卡布达";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "铁甲小宝";
}


//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return 0;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 0;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return 0;
}

//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 0;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 0;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 0;
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