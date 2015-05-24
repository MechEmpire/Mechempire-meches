#include "RobotAI.h"

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
	int hhh;
	hhh=(rand()+19990203)%6;
	if(hhh==0||hhh==4)
		order.run=1;
	if(hhh==1)
		order.run=2;
	if(hhh==2)
		order.run=3;
	if(hhh==3||hhh==5)
		order.run=4;
	if(fabs(info.robotInformation[myID].circle.y-info.robotInformation[1-myID].circle.y)<5)
		order.fire=1;
}
	
	


void RobotAI::ChooseArmor(weapontypename& weapon,enginetypename& engine,bool a)
{
	weapon = WT_Prism;				
	engine = ET_Spider;						
}
//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------
string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "随机的力量";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "Yinhang_4";
}
int RobotAI::GetWeaponRed()
{
	return 5;
}
int RobotAI::GetWeaponGreen()
{
	return -62;
}
int RobotAI::GetWeaponBlue()
{
	return -241;
}
int RobotAI::GetEngineRed()
{
	return -255;
}
int RobotAI::GetEngineGreen()
{
	return -91;
}
int RobotAI::GetEngineBlue()
{
	return -136;
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

