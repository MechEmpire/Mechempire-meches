#include "RobotAI.h"
#include<iostream>
#define Rleft (order.run =1)
#define Rright (order.run =2)
#define Rup (order.run =3)
#define Rdown (order.run =4)

double tAngle(double angle1, double angle2)
{
	double a;
	a = abs(angle1 - angle2);
	if (a > 180)
		a = 360 - a;
	
	return a;
}

double getAngle(Circle a, Circle b)
{
	Circle A;
	A.x = b.x - a.x;
	A.y = b.y - a.y;
	A.r = atan(A.y / A.x) * 180 / PI;
	double a1;
	a1 = A.r + 180;
	if (A.x < 0)
		A.r = AngleAdjust(a1);
	return A.r;
}

int turnfire(double radfire, double radother, double radins, bool&state)
{

	double rad1, rad2;
	double a;
	double a2;
	int a1[2] = { 0 };
	a = radother + radins;
	rad1 = AngleAdjust(a);
	a = radother - radins;
	rad2 = AngleAdjust(a);
	//cout << rad1 << '\t' << rad2 << '\n';
	a = abs(rad1 - rad2);
	if (a <= 180)
	{
		a1[0] = 1;
	}

	if (rad1 > rad2)
		a2 = rad2;
	else
		a2 = rad1;
	if (radfire < a2 + a && radfire > a2)
		a1[1] = 1;
	if (a1[1] == a1[0])
	{
		state = 1;
		return 0;
	}
	if (state == 0)
	{	
		a = radother + 1;
		if (tAngle(radother, radfire) > tAngle(AngleAdjust(a), radfire))
			return -1;
		else
			return  1;
	}
}

void controlfire(Circle mypoint, Circle otherpoint, double radfire, int&fire, int&wturn)
{
	fire = 0;
	bool state = 0;
	bool state1 = 1;//未有障碍物
	Circle A;
	A.x = otherpoint.x - mypoint.x;
	A.y = otherpoint.y - mypoint.y;
	A.r = sqrt(A.x*A.x + A.y*A.y);
	double  ins;
	ins = (otherpoint.r - A.r*0.04366) / (otherpoint.y*0.04366 + A.r);
	double radins = atan(ins) * 180 / PI;
	radins -= radins*A.r / 1400;
	double radother = getAngle(mypoint, otherpoint);
	wturn = turnfire(radfire, radother, radins, state);
	if (state == 1)
	{
		bool aa = 0;
		Circle b[2] = { 300, 250, 82 + A.r/1400*15, 1066, 430, 82+A.r/1400*15 };
		double rad3, rad4;
		rad3 = getAngle(mypoint, b[0]);
		rad4 = getAngle(mypoint, b[1]);
		Beam line = { 0 };
		line.x = mypoint.x;
		line.y = mypoint.y;
		line.rotation = radfire;
		if (HitTestBeamCircle(line, b[0]) == true)
		{
			state1 = 0;
			wturn = -turnfire(radfire, rad3, 0, aa);
		}
		if (HitTestBeamCircle(line, b[1]) == true)
		{
			state1 = 0;
			wturn = -turnfire(radfire, rad4, 0, aa);
		}
	}
	if (state == 1 && state1 == 1)
		fire = 1;

	}






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

	double radfire = info.robotInformation[myID].weaponRotation;
	Circle mypoint = info.robotInformation[myID].circle;
	Circle otherpoint = info.robotInformation[1 - myID].circle;
	controlfire(mypoint, otherpoint, radfire, order.fire, order.wturn);
	//cout << radfire << '\n';
	//system("pause");
	int stape = 0;
	static int st = 0;
	++st;
	stape = st / 145;
	if (myID == 1)
	{
		switch (stape % 2)
		{
		case 0:
			Rup;
			break;
		case 1:
			Rdown;
			break;
		}
	}
	if (myID == 0)
	{
		switch (stape % 2)
		{
		case 1:
			Rup;
			break;
		case 0:
			Rdown;
			break;
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

	weapon = WT_Prism;	//啊，我爱
	engine = ET_Spider;	//啊，我爱蜘蛛
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "登革热";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "菜刀队";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return -179;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 10;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return 209;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 32;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 231;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 220;
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