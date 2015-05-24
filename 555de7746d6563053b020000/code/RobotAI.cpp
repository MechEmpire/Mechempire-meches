#include "RobotAI.h"
#include <stdlib.h>
#include "MyFunction.h"
using namespace std;
RobotAI::RobotAI()
{
}


RobotAI::~RobotAI()
{
	delete [] arsenal;
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
	tool.Log("weapon");
	
	tool.updateTrack(info.robotInformation[1-myID].circle);

	if(info.robotInformation[myID].weaponTypeName==WT_RPG){
		order.wturn=tool.TargetAt(info.robotInformation[myID].circle,tool.GetProPointRPG(info.robotInformation[myID].circle),
			info.robotInformation[myID].weaponRotation,1.0);
	
		order.run=1;
		if(info.robotInformation[myID].remainingAmmo!=0){	
			if(order.wturn==0){
				if(tool.canShoot(info.robotInformation[myID].circle,tool.GetProPointRPG(info.robotInformation[myID].circle),info.obstacle)){
					order.fire=1;
				}else{
					order.fire=0;
				}
			}
			Circle targetAddr=tool.GetTargetAddr(info.robotInformation[myID].circle,info.robotInformation[1-myID].circle,
				info.obstacle,150);
			order.eturn=tool.TargetAt(info.robotInformation[myID].circle,targetAddr,
				info.robotInformation[myID].engineRotation,1.0);
		}else{
			int arsenalId=tool.GetSafeArsenal(info.robotInformation[myID].circle,info.robotInformation[1-myID].circle,
				info.arsenal,200.0);
			if(arsenalId==-1){
				Circle targetAddr=tool.GetTargetAddr(info.robotInformation[myID].circle,info.robotInformation[1-myID].circle,
					info.obstacle,90);
				order.eturn=tool.TargetAt(info.robotInformation[myID].circle,targetAddr,
					info.robotInformation[myID].engineRotation,1.0);
			}else{
				order.eturn=tool.TargetAt(info.robotInformation[myID].circle,arsenal[arsenalId],
					info.robotInformation[myID].engineRotation,1.0);
			}
		}
	}else if(info.robotInformation[myID].weaponTypeName==WT_GrenadeThrower){
		order.wturn=tool.TargetAt(info.robotInformation[myID].circle,tool.GetProPointCannon(info.robotInformation[myID].circle),
			info.robotInformation[myID].weaponRotation,1.0);

		order.run=1;
		if(info.robotInformation[myID].remainingAmmo!=0){	
			if(order.wturn==0){
				if(tool.canShoot(info.robotInformation[myID].circle,tool.GetProPointRPG(info.robotInformation[myID].circle),info.obstacle)){
					order.fire=1;
				}else{
					order.fire=0;
				}
			}
			Circle targetAddr=tool.GetTargetAddr(info.robotInformation[myID].circle,info.robotInformation[1-myID].circle,
				info.obstacle,90);
			order.eturn=tool.TargetAt(info.robotInformation[myID].circle,targetAddr,
				info.robotInformation[myID].engineRotation,1.0);
		}else{
			int arsenalId=tool.GetSafeArsenal(info.robotInformation[myID].circle,info.robotInformation[1-myID].circle,
				info.arsenal,200.0);
			if(arsenalId==-1){
				Circle targetAddr=tool.GetTargetAddr(info.robotInformation[myID].circle,info.robotInformation[1-myID].circle,
					info.obstacle,90);
				order.eturn=tool.TargetAt(info.robotInformation[myID].circle,targetAddr,
					info.robotInformation[myID].engineRotation,1.0);
			}else{
				order.eturn=tool.TargetAt(info.robotInformation[myID].circle,arsenal[arsenalId],
					info.robotInformation[myID].engineRotation,1.0);
			}
		}
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
	srand(time(0));
	int i=rand()%2;
	switch(i){
	case 0:
		weapon = WT_RPG;
		engine = ET_AFV;
		break;
	case 1:
		weapon = WT_RPG;
		engine = ET_AFV;
	}
	//engine = ET_AFV;
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "木之本樱";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "Frostsword";
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
	
	arsenal=new Circle[2];
	arsenal[0]=info.arsenal[0].circle;
	arsenal[1]=info.arsenal[1].circle;
	tool.init(info.robotInformation[1-myID].circle);
	
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