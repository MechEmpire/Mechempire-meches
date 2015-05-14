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
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	static int frame = 0;	
    if (frame != 0)
	{
			onFire1(order, info, myID);
	}
	onTag1(order, info, myID);
	onMove1(order, info, myID);
	frame++;
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

	weapon = WT_ElectricSaw;	//啊，我爱加农炮
//	weapon = WT_Prism;
	engine = ET_AFV;	//啊，我爱小蜘蛛
	
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "叫我黏糊糊粗粮";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "爆炸何";
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

//判断和敌方之间是否有障碍物,ArID为障碍物ID
bool RobotAI::HaveBarrier(const RobotAI_BattlefieldInformation& info,const int myID,const int ArID)
{
	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[1 - myID];
	Circle obs_circle = info.obstacle[ArID];
	double pdis = pointdis(me.circle, tag.circle, obs_circle);
	if (pdis > info.obstacle[ArID].r)		
		return false;
	else
	{
		if (((me.circle.y - obs_circle.y)*(tag.circle.y - obs_circle.y)) &&
			((me.circle.x - obs_circle.x)*(tag.circle.x - obs_circle.x)))
			return false;
		else
			return true;	
	}

}

double pi = 3.14159;

void RobotAI::onTag1(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info, const int myID)//瞄准函数（不带预判）
{
	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[1 - myID];
	double dx = tag.circle.x - me.circle.x;
	double dy = tag.circle.y - me.circle.y;
	double Rota = atan2(dy, dx) * 180 / pi;
	double wpRota = info.robotInformation[myID].weaponRotation;
	double offset = wpRota - Rota;
	if (abvalue(offset) >= 180)
	{
		if (offset > 0)
			order.wturn = 1;
		else
			order.wturn = -1;
	}
	else{
		if (offset > 0)
			order.wturn = -1;
		else
			order.wturn = 1;
	}
	
}

double RobotAI::onTagRota(const RobotAI_BattlefieldInformation& info, const int myID)
{
	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[1 - myID];
	double dx = tag.circle.x - me.circle.x;
	double dy = tag.circle.y - me.circle.y;
	double Rota = atan2(dy, dx) * 180 / pi;
	double wpRota = info.robotInformation[myID].weaponRotation;
	double offset = wpRota - Rota;
	return offset;
}

void RobotAI::onMove1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID)
{
	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[1 - myID];
	double dx = tag.circle.x - me.circle.x;
	double dy = tag.circle.y - me.circle.y;
	double Rota = atan2(dy, dx) * 180 / pi;
	double Rota_eng = me.engineRotation;
	double offset = Rota_eng - Rota;
	switch (me.engineTypeName)
	{
	case ET_AFV:
		if (abvalue(offset) >= 180)
		{
			if (offset > 0)
				order.eturn = 1;
			else
				order.eturn = -1;
		}
		else{
			if (offset > 0)
				order.eturn = -1;
			else
				order.eturn = 1;
		}
		order.run = 1;
		break;
	default:
		break;
	}
}

void RobotAI::onFire1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID)
{
	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[1 - myID];
	switch (me.weaponTypeName)
	{
	case WT_ElectricSaw:
		if (
			!HaveBarrier(info, myID, 0) &&
			!HaveBarrier(info, myID, 1) &&
			(onTagRota(info, myID)<3) &&
			(onTagRota(info, myID)>-3) &&
			dis(me.circle, tag.circle)<95 + tag.circle.r
			)
			order.fire = 1;
		break;
	case WT_Prism:
	case WT_Tesla:
		if (
			!HaveBarrier(info, myID, 0) &&
			!HaveBarrier(info, myID, 1) &&
			(onTagRota(info, myID)<3)&&
			(onTagRota(info, myID)>-3)
			)
			order.fire = 1;
		break;
	default:
		break;
	}
}

double RobotAI::pointdis(double x1, double y1, double x2, double y2, double x3, double y3)
{
	double A = (y2 - y1) / (x2 - x1);
	double B = -1.0;
	double C = y1 - (y2 - y1)*x1 / (x2 - x1);
	double son = A*x3 + B*y3 + C;
	double Abvalue = son / sqrt(A*A + B*B);
	if (Abvalue > 0)
		return Abvalue;
	else
		return -Abvalue;
}

double RobotAI::pointdis(const Circle a1, const Circle a2, const Circle a3)
{
	return pointdis(a1.x, a1.y, a2.x, a2.y, a3.x, a3.y);
}


double RobotAI::dis(double x1, double y1, double x2, double y2) {
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

double RobotAI::dis(const Circle& a, const Circle& b){
	return dis(a.x, a.y, b.x, b.y);
}

double RobotAI::abvalue(double x)
{
	if (x >= 0)
		return x;
	else
		return -x;
}


