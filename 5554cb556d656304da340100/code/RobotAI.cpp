#include "RobotAI.h"
double dis(double, double, double, double);
RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}
double Angle(double x1, double y1, double x2, double y2){
	double angle = acos((x1*x2 + y1*y2) / sqrt(x1*x1 + y1*y1) / sqrt(x2*x2 + y2*y2));
	return angle;
}
bool HitTestCirclePoint_own(const Circle &c, const double &x, const double &y, double i)
{
	double dx = x - c.x, dy = y - c.y;

	/*
	if(abs(dx)>i*c.r || abs(dy)>i*c.r) //预先判断，简化计算
	{
	return false;
	}
	*/
	double dis1 = sqrt(dx*dx + dy*dy);
	if (dis1 > c.r&&dis1 <= c.r*i)
		return true;
	else
		return false;
}
bool HitTestCircles_fake(const Circle &c1, const Circle &c2, double p,double q)
{
	double xx = c1.x - c2.x, yy = c1.y - c2.y;
	double rr = c1.r *p+ q*c2.r;

	

	double dis2 = xx*xx + yy*yy;

	return (rr*rr >= dis2);
}
//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------
void weapon_rotation(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID) {
	auto& me = info.robotInformation[myID];
	auto& target = info.robotInformation[1 - myID];
	double dx = target.circle.x - me.circle.x;
	double dy = target.circle.y - me.circle.y;
	double dt = atan2(dy, dx)*180.0 / PI - me.weaponRotation;
	AngleAdjust(dt);
	const double eps = 1e-3;
	if (dt> eps){
		order.wturn = 1;//顺时针
	}
	else if (dt < -eps){
		order.wturn = -1;//逆时针
	}
	else {
		order.wturn = 0;
	}
	
}

void engine_drive(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID) {
	auto& me = info.robotInformation[myID];
	auto& target = info.robotInformation[1 - myID];
	double dx = target.circle.x - me.circle.x;
	double dy = target.circle.y - me.circle.y;
	double dt = atan2(dy, dx)*180.0 / PI - me.engineRotation;
	AngleAdjust(dt);
	const double eps = 1e-3;
	if (dt> eps){
		order.eturn = 1;//顺时针
	}
	else if (dt < -eps){
		order.eturn = -1;//逆时针
	}
	else {
		order.eturn = 0;
	}
	order.run = 1;
}
void obstacle_hit(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	auto& me = info.robotInformation[myID];
	for (int i = 0; i <= 1; i++)
	{

		if (bool obstacle_judge = HitTestCircles_fake(info.robotInformation[myID].circle, info.obstacle[i],1,1.1))
		{
			if (Angle(me.circle.x, me.circle.y, info.obstacle[i].x - me.circle.x, info.obstacle[i].y - me.circle.y) <= 90.0)
			{
				order.eturn = 1;
			
			}
		}
	}
	
}
void escape_bullet(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	auto& me = info.robotInformation[myID];
	
	//找最近的子弹
	double min_dis = 1000;//初始化一个很大的值
	int num_min = 0;
	for (int i = 0; i <= info.num_bullet; i++)
	{
		auto& target = info.bulletInformation[i];
		double temp = dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y);
		if (min_dis <= temp)
		{
			min_dis = temp;
			num_min = i;
		}
	}
	
	
	auto& target = info.bulletInformation[num_min];
			//躲避各种子弹
	
	double r_expansion=0;//半径倍增系数
	
	switch (target.type)
	{
	case BT_Cannonball:
		r_expansion = 4.5;
		break;
	case BT_ShotgunBall:
		r_expansion = 3.5;
		break;
	case BT_MachinegunBall:
		r_expansion = 3;
		break;
	case WT_PlasmaTorch:
		r_expansion = 2.7;
		break;
	}
	
    if (r_expansion)
		{
			//如果更大半径内碰得到，处于两圆中间地带
			if (HitTestCirclePoint_own(me.circle, target.circle.x, target.circle.y,r_expansion))
			{

				double dx = target.circle.x - me.circle.x;
				double dy = target.circle.y - me.circle.y;

				double dt = atan2(dy, dx)*180.0 / PI - target.rotation;
				AngleAdjust(dt);
				double dt2 = atan2(dy, dx)*180.0 / PI - me.engineRotation;
				AngleAdjust(dt2);


				if (dt >= 160.0&&dt <= 180)
				{
					order.eturn = 1;//顺时针
					
				}
				else if (dt <= -160 && dt >= -180)
				{
					order.eturn = -1;//逆时针
				}
			}
		}
}

void boundry_hit(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	auto& me = info.robotInformation[myID];

	if (me.circle.y -60- me.circle.r == 0)
	{
		if (me.engineRotation >270)
			order.eturn = 1;
		else if (me.engineRotation >180 && me.engineRotation<270)
			order.eturn = -1;
	}
		
	if (me.circle.y + me.circle.r+60 == info.boundary.height)
	{
		if (me.engineRotation >0 && me.engineRotation<90)
			order.eturn = -1;
		else if (me.engineRotation >90 && me.engineRotation <180)
			order.eturn = 1;
	}

	if (me.circle.x + me.circle.r+60 == info.boundary.width)
	{
		if (me.engineRotation >0 && me.engineRotation < 90)
			order.eturn = 1;
		else if (me.engineRotation >270)
			order.eturn = -1;
	}

	if (me.circle.x - me.circle.r-60 == 0)
	{
		if (me.engineRotation >90 && me.engineRotation < 180)
			order.eturn = -1;
		else if (me.engineRotation >180 && me.engineRotation < 270)
			order.eturn = 1;
	}

}
void fire_or_not(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	auto& me = info.robotInformation[myID];
	auto& target = info.robotInformation[1 - myID];
	if (bool obstacle_judge = HitTestCircles_fake(info.robotInformation[myID].circle, info.robotInformation[1-myID].circle,2,1))
	{
		order.fire = 1;
	}
}
void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	auto& me = info.robotInformation[myID];
	auto& target = info.robotInformation[1 - myID];
	weapon_rotation(order, info, myID);
	
	if (target.remainingAmmo - me.hp / 25 > 0 && dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y) <= 600)
	{
		order.run = 1;
		order.eturn = -1;
	}
	else
	{
		engine_drive(order, info, myID);
	}
	
	obstacle_hit(order, info, myID);
	
	escape_bullet(order, info, myID);
	
	boundry_hit(order, info, myID);
	
	fire_or_not(order, info, myID);

	//order.run=Random(1, 4);
	

	
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
	engine = ET_GhostTank;	//四轴飞行器
	SetSeed();
}

double dis(double x1, double y1, double x2, double y2) {
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}









//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "Trail";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "ui";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return -50;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return -50;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -50;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return -50;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return -50;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return -50;
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