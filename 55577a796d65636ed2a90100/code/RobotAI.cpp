﻿#include "RobotAI.h"

RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}


//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------
int i_jishu = 1;
void shoot(RobotAI_Order& order, double x1, double y1, const RobotAI_BattlefieldInformation& info, int myID);
void walkrotation(RobotAI_Order& order, double rio, const RobotAI_BattlefieldInformation& info, int myID);
void walkpoint(RobotAI_Order& order, double x1, double y1, const RobotAI_BattlefieldInformation& info, int myID);
void duozidan(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID);
void raojiaoluo(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID);
void duozhangai(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID);
void daweisuo(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID);
void dadianju(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID);
void dajiqiang(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID);
void dajianong(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID);
void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	
	const double eps = 1e-3;
	double R = info.robotInformation[myID].circle.r;
	double x0 = info.robotInformation[myID].circle.x;
	double y0 = info.robotInformation[myID].circle.y;
	double x1 = info.robotInformation[1 - myID].circle.x;
	double y1 = info.robotInformation[1 - myID].circle.y;
	double distance = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
	double k1 = atan2(y1 - y0, x1 - x0) * 180 / 3.14;
	double k2_jh1 = atan2(y0 - 630, x0 - 50) * 180 / 3.14;
	double k2_jh2 = atan2(y0 - 50, x0 - 1316) * 180 / 3.14;
	double deviation = ((y1 - y0) / (x1 - x0));
	double hittime = distance / 15;
	bool isdj = false, isws = false, firstrate_shoot = false, isjq = false, isyf = false,isshu = false;
	//判断是否是ws
 if (info.robotInformation[1 - myID].weaponTypeName == WT_Prism || info.robotInformation[1 - myID].weaponTypeName == WT_MissileLauncher || info.robotInformation[1 - myID].weaponTypeName == WT_Tesla)
		isws = true;
	else
		isws = false;
	//判断是否是电锯
	if (info.robotInformation[1 - myID].weaponTypeName == WT_ElectricSaw)
		isdj = true;
	else
		isdj = false;
	//判断是否是机枪
	 if (info.robotInformation[1 - myID].weaponTypeName == WT_Machinegun)
		isjq = true;
	else
		isjq = false;
	//判断是否是UFO
	if (info.robotInformation[1 - myID].engineTypeName == ET_UFO)
		isyf = true;
	else isyf = false;
	//判断是否是shuttle
	if (info.robotInformation[1 - myID].engineTypeName == ET_Shuttle)
		isshu = true;
	else isshu = false;
	//判断第一帧是否射击；
	double dis_first0 = sqrt((x0 - 50)*(x0 - 50) + (y0 - 50)*(y0 - 50));
	double dis_first1 = sqrt((x0 - 1316)*(x0 - 1316) + (y0 - 630)*(y0 - 630));
	if (dis_first0 < 5 || dis_first1 < 5)
		firstrate_shoot = false;
	else firstrate_shoot = true;
	//判断是否碰到障碍物
	Beam  be1 = { x0, y0, info.robotInformation[myID].weaponRotation, x1, y1 };
	Circle c1 = { 300, 250, 75 }, c2 = { 1066, 430, 75 };
	bool p, p1, p2;
	p = false;
	p1 = HitTestBeamCircle(be1, c1);
	p2 = HitTestBeamCircle(be1, c2);
	if (p1 == true || p2 == true)
		p = true;
	else p = false;
	//射击方式改进
	double x1n = info.robotInformation[1 - myID].vx * hittime + x1;
	double y1n = info.robotInformation[1 - myID].vy * hittime + y1;
	double x1ng, y1ng;
	if (isyf == true)
	{
		x1ng = x1;
		y1ng = y1;
	}
	else if (isyf == false)
	{
		if (x1n > 1366)
			x1ng = x1;
		else x1ng = x1n;
		if (x1n < 0)
			x1ng = x1;
		else x1ng = x1n;
		if (y1n > 680)
			y1ng = y1;
		else y1ng = y1n;
		if (y1n < 0)
			y1ng = y1;
		else y1ng = y1;
	}
	// 判断敌方机甲在那个弹药库附近
	double dis_enemyammo1, dis_enemyammo2, dis_meammo1, dis_meammo2;
	int b1 = 0;
	dis_enemyammo1 = sqrt((x1 - 50)*(x1 - 50) + (y1 - 630)*(y1 - 630));
	dis_enemyammo2 = sqrt((x1 - 1316)*(x1 - 1316) + (y1 - 50)*(y1 - 50));
	dis_meammo1 = sqrt((x0 - 50)*(x0 - 50) + (y0 - 630)*(y0 - 630));
	dis_meammo2 = sqrt((x0 - 1316)*(x0 - 1316) + (y0 - 50)*(y0 - 50));
	if (dis_enemyammo1 < 100)
	{
		b1 = 1;
	}
	else if (dis_enemyammo2 < 100)
	{
		b1 = 2;
	}

	//射击
	if (distance > 300 && isws == false && distance < 900)
	{
		shoot(order, x1ng, y1ng, info, myID);
		if (p != 1 
			&& firstrate_shoot == true)
			order.fire = 1;
	}
	else if (distance < 300 && isws == false)
	{
		shoot(order, x1, y1, info, myID);
		if (p != 1 && firstrate_shoot == true)
			order.fire = 1;
	}
	else if (isws == true && distance < 500)
	{
		shoot(order, x1ng, y1ng, info, myID);
		if (p != 1 && firstrate_shoot == true)
			order.fire = 1;
	}
	//行走方式改进	
	order.run = 1;
	if (info.robotInformation[myID].remainingAmmo > 2)
	{
		if (isdj == true && isshu == false)//距离要边远 勾引
		{
			dadianju(order, info, myID);
		}
		else if (isws == true && isshu == false)//测试效果很好
		{
			daweisuo(order, info, myID);
		}
		else if (isjq == true && isshu == false)//效果不好 改
		{
			
			dajiqiang(order, info, myID);
	//		duozidan(order, info, myID);
		
			
		}
		else if (isshu == true)//仍需要测试
		{
			walkpoint(order, 600, 300, info, myID);
			raojiaoluo(order, info, myID);
		}
		else 
		{
			walkpoint(order, 600, 300, info, myID);
			raojiaoluo(order, info, myID);
			duozidan(order, info, myID);
			duozhangai(order, info, myID);
			dajianong(order, info, myID);
			//绕行robot
			/*double dis_xu = y0;
			double dis_xb = 680 - y0;
			double dis_yl = x0;
			double dis_yr = 1366 - x0;
			if (distance < 600)
			{
				if (dis_xu > dis_xb)
				{
					double rr = info.robotInformation[1 - myID].engineRotation - 90;
					walkrotation(order, rr, info, myID);
				}
				else if (dis_xu < dis_xb)
				{
					double rr = info.robotInformation[1 - myID].engineRotation + 90;
					walkrotation(order, rr, info, myID);
				}
			}*/
			
		}
	}
	if (info.robotInformation[myID].remainingAmmo <= 2)
	{
		//info.arsenal[q].respawning_time;
		//捡弹药库
		if (b1 == 2 && info.arsenal[0].respawning_time == 0)
		{
			walkpoint(order, 50, 630, info, myID);
		}
		else if (b1 == 1 && info.arsenal[1].respawning_time == 0)
		{
			walkpoint(order, 1316, 50, info, myID);
		}
		else
		{
			if (dis_meammo1 > dis_meammo2 && info.arsenal[1].respawning_time == 0)
			{
				walkpoint(order, 1316, 50, info, myID);

			}
			else if (info.arsenal[0].respawning_time == 0)
			{
				walkpoint(order, 50, 630, info, myID);
			}
		}
	//	duozidan(order, info, myID);
		raojiaoluo(order, info, myID);
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

	weapon = WT_Cannon;	//啊，我爱加农炮
	engine = ET_GhostTank;	//啊，我爱小蜘蛛
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "EVA0.3";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "YYN";
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
void shoot(RobotAI_Order& order, double x1, double y1, const RobotAI_BattlefieldInformation& info, int myID)
{
	double x0 = info.robotInformation[myID].circle.x;
	double y0 = info.robotInformation[myID].circle.y;
	double dx = x1 - x0;
	double dy = y1 - y0;
	double theta = atan2(dy, dx)*180.0 / PI;
	double mt = info.robotInformation[myID].weaponRotation;
	double dt = theta - mt;
	AngleAdjust(dt);
	const double eps = 1e-3;

	if (dt > eps){
		order.wturn = 1;
	}
	else if (dt < -eps){
		order.wturn = -1;
	}
	else {
		order.wturn = 0;

	}
}

void walkrotation(RobotAI_Order& order, double rio, const RobotAI_BattlefieldInformation& info, int myID)
{
	double mt = info.robotInformation[myID].engineRotation;
	double dt = rio - mt;
	AngleAdjust(dt);
	const double eps = 1e-3;

	if (dt > eps){
		order.eturn = 1;
	}
	else if (dt < -eps){
		order.eturn = -1;
	}
	else {
		order.eturn = 0;

	}
}

void walkpoint(RobotAI_Order& order, double x1, double y1, const RobotAI_BattlefieldInformation& info, int myID)
{
	double x0 = info.robotInformation[myID].circle.x;
	double y0 = info.robotInformation[myID].circle.y;
	double dx = x1 - x0;
	double dy = y1 - y0;
	double theta = atan2(dy, dx)*180.0 / PI;
	double mt = info.robotInformation[myID].engineRotation;
	double dt = theta - mt;
	AngleAdjust(dt);
	const double eps = 1e-3;

	if (dt > eps){
		order.eturn = 1;
	}
	else if (dt < -eps){
		order.eturn = -1;
	}
	else {
		order.eturn = 0;

	}
}
void duozidan(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	double vx_bullet_near, vy_bullet_near, dis_bullet_near = 9999, dis_bullet_0, db, x_b, y_b;
	double x0 = info.robotInformation[myID].circle.x;
	double y0 = info.robotInformation[myID].circle.y;
	bool bullet_me = false;
	int i_b;
	for (i_b = 0; i_b < info.num_bullet; i_b++)
	{
		if (info.bulletInformation[i_b].launcherID == 1 - myID)
		{
			dis_bullet_0 = sqrt((x_b - x0) * (x_b - x0) + (y_b - y0)*(y_b - y0));
		}
		if (dis_bullet_0 < dis_bullet_near)
		{
			dis_bullet_near = dis_bullet_0;
			vx_bullet_near = info.bulletInformation[i_b].vx;
			vy_bullet_near = info.bulletInformation[i_b].vy;
			x_b = info.bulletInformation[i_b].circle.x;
			y_b = info.bulletInformation[i_b].circle.y;
			db = atan2(vy_bullet_near, vx_bullet_near);
			AngleAdjust(db);
		}
	}
	Beam bullet_b = { x_b, y_b, db, x_b + vx_bullet_near, y_b + vy_bullet_near };
	Circle me = { x0, y0, 55 };
	bullet_me = HitTestBeamCircle(bullet_b, me);
	if (bullet_me == true)
	{
		double theta;
		if (db > info.robotInformation[myID].engineRotation)
			theta = db + 90;
		else theta = db - 90;
		walkrotation(order, theta, info, myID);
	}
}

void raojiaoluo(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	//以前的行走方式 绕圈
	if (info.robotInformation[myID].circle.x < 150 && info.robotInformation[myID].circle.y > 400)
		//&& info.robotInformation[myID].engineRotation != -90)
	{

		walkrotation(order, -90, info, myID);
	}
	else if (info.robotInformation[myID].circle.x < 150 && info.robotInformation[myID].circle.y < 100)
		//&& info.robotInformation[myID].engineRotation != 0.0)
	{
		walkrotation(order, 0, info, myID);
	}
	else if (info.robotInformation[myID].circle.x >1300 && info.robotInformation[myID].circle.y < 100)
		//&& info.robotInformation[myID].engineRotation != 90)
	{
		walkrotation(order, 90, info, myID);
	}
	else if (info.robotInformation[myID].circle.x > 1200 && info.robotInformation[myID].circle.y > 500)
		//&& info.robotInformation[myID].engineRotation != 180)
	{
		walkrotation(order, 180, info, myID);
	}
}

void duozhangai(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	double x0 = info.robotInformation[myID].circle.x;
	double y0 = info.robotInformation[myID].circle.y;
	double dis_me_za0 = sqrt((300 - x0)*(300 - x0) + (250 - y0)*(250 - y0));
	double dis_me_za1 = sqrt((1066 - x0)*(1066 - x0) + (430 - y0)*(430 - y0));
	if (dis_me_za0 < 200)
	{
		double theta1 = atan2(250 - y0, 300 - x0)*180.0 / PI;
		double dt = theta1 + 90;
		walkrotation(order, dt, info, myID);
	}
	else if (dis_me_za1 < 200)
	{
		double theta1 = atan2(1066 - y0, 430 - x0)*180.0 / PI;
		double dt = theta1 + 90;
		walkrotation(order, dt, info, myID);
	}
}
void daweisuo(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	double x0 = info.robotInformation[myID].circle.x;
	double y0 = info.robotInformation[myID].circle.y;
	double x1 = info.robotInformation[1 - myID].circle.x;
	double y1 = info.robotInformation[1 - myID].circle.y;
	double distance = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
	double dis_me_za0 = sqrt((300 - x0)*(300 - x0) + (250 - y0)*(250 - y0));
	double dis_me_za1 = sqrt((1066 - x0)*(1066 - x0) + (430 - y0)*(430 - y0));
	double dx1 = x1 - x0;
	double dy1 = y1 - y0;
	double theta1 = atan2(dy1, dx1)*180.0 / PI;
	if (distance > 10)
	{
		if (dis_me_za0 < 200)
		{
			double dt = theta1 + 35;
			walkrotation(order, dt, info, myID);
		}
		else if (dis_me_za1 < 200)
		{
			double dt = theta1 + 35;
			walkrotation(order, dt, info, myID);
		}
		else
		if (dis_me_za0 > 75 || dis_me_za1 > 75)
		{
			double dt = theta1;
			walkrotation(order, dt, info, myID);
		}
	}
	else
	{
		double dt = theta1;
		walkrotation(order, dt, info, myID);
	}
	duozidan(order, info, myID);
}
void dadianju(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	double x0 = info.robotInformation[myID].circle.x;
	double y0 = info.robotInformation[myID].circle.y;
	double x1 = info.robotInformation[1 - myID].circle.x;
	double y1 = info.robotInformation[1 - myID].circle.y;
	double distance = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
	if (distance > 600)
	{
		walkpoint(order, x1, y1, info, myID);
	}
	else if (distance < 600)
	{
		walkrotation(order, info.robotInformation[1 - myID].engineRotation, info, myID);
	}
	raojiaoluo(order, info, myID);
	duozhangai(order, info, myID);
}

void dajiqiang(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	double x0 = info.robotInformation[myID].circle.x;
	double y0 = info.robotInformation[myID].circle.y;
	double x1 = info.robotInformation[1 - myID].circle.x;
	double y1 = info.robotInformation[1 - myID].circle.y;
	double distance = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
	raojiaoluo(order, info, myID);
	duozhangai(order, info, myID);
	if (myID == 1)
	{
		if (info.arsenal[1].respawning_time == 0)
		{
			walkpoint(order, 1316, 50, info, myID);
			if (distance < 600)
			{
				if (y0 > 340)
				{
					if (info.robotInformation[myID].vy > 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta + 90;
						walkrotation(order, rr, info, myID);
					}
					else if (info.robotInformation[myID].vy < 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta - 90;
						walkrotation(order, rr, info, myID);
					}

				}
			}
		}
		else
		if (info.arsenal[0].respawning_time == 0)
		{
			walkpoint(order, 50, 630, info, myID);
			if (distance < 600)
			{
				if (y0 > 340)
				{
					if (info.robotInformation[myID].vy > 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta + 90;
						walkrotation(order, rr, info, myID);
					}
					else if (info.robotInformation[myID].vy < 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta - 90;
						walkrotation(order, rr, info, myID);
					}

				}
			}
		}
		else
		if (distance < 700)
		{
			if (info.robotInformation[1 - myID].remainingAmmo * 7 < info.robotInformation[myID].hp)
			{
				daweisuo(order, info, myID);
			}
			else
			if (y0 > 340)
			{
				if (info.robotInformation[myID].vy > 0)
				{
					double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
					double rr = theta + 90;
					walkrotation(order, rr, info, myID);
				}
				else if (info.robotInformation[myID].vy < 0)
				{
					double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
					double rr = theta - 90;
					walkrotation(order, rr, info, myID);
				}

			}
			else if (y0 < 340)
			{
				double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
				double rr = theta + 90;
				walkrotation(order, rr, info, myID);
			}

		}
	}
	else if (myID == 0)
	{
		if (info.arsenal[0].respawning_time == 0)
		{
			walkpoint(order, 50, 630, info, myID);
			if (distance < 600)
			{
				if (y0 > 340)
				{
					if (info.robotInformation[myID].vy > 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta + 90;
						walkrotation(order, rr, info, myID);
					}
					else if (info.robotInformation[myID].vy < 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta - 90;
						walkrotation(order, rr, info, myID);
					}

				}
			}
		}
		else
		if (info.arsenal[1].respawning_time == 0)
		{
			walkpoint(order, 1316, 50, info, myID);
			if (distance < 600)
			{
				if (y0 > 340)
				{
					if (info.robotInformation[myID].vy > 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta + 90;
						walkrotation(order, rr, info, myID);
					}
					else if (info.robotInformation[myID].vy < 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta - 90;
						walkrotation(order, rr, info, myID);
					}

				}
			}
		}
		else
		if (distance < 700)
		{
			if (info.robotInformation[1 - myID].remainingAmmo * 7 < info.robotInformation[myID].hp)
			{
				daweisuo(order, info, myID);
			}
			else
			if (y0 > 340)
			{
				if (info.robotInformation[myID].vy > 0)
				{
					double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
					double rr = theta + 90;
					walkrotation(order, rr, info, myID);
				}
				else if (info.robotInformation[myID].vy < 0)
				{
					double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
					double rr = theta - 90;
					walkrotation(order, rr, info, myID);
				}

			}
			else if (y0 < 340)
			{
				double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
				double rr = theta + 90;
				walkrotation(order, rr, info, myID);
			}

		}
	}
}
void dajianong(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	double x0 = info.robotInformation[myID].circle.x;
	double y0 = info.robotInformation[myID].circle.y;
	double x1 = info.robotInformation[1 - myID].circle.x;
	double y1 = info.robotInformation[1 - myID].circle.y;
	double distance = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
	raojiaoluo(order, info, myID);
	duozhangai(order, info, myID);
	if (myID == 1)
	{
		if (info.arsenal[1].respawning_time == 0)
		{
			walkpoint(order, 1316, 50, info, myID);
			if (distance < 600)
			{
				if (y0 > 340)
				{
					if (info.robotInformation[myID].vy > 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta + 90;
						walkrotation(order, rr, info, myID);
					}
					else if (info.robotInformation[myID].vy < 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta - 90;
						walkrotation(order, rr, info, myID);
					}

				}
			}
		}
		else
		if (info.arsenal[0].respawning_time == 0)
		{
			walkpoint(order, 50, 630, info, myID);
			if (distance < 600)
			{
				if (y0 > 340)
				{
					if (info.robotInformation[myID].vy > 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta + 90;
						walkrotation(order, rr, info, myID);
					}
					else if (info.robotInformation[myID].vy < 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta - 90;
						walkrotation(order, rr, info, myID);
					}

				}
			}
		}
		else
		if (distance < 700)
		{
			if (info.robotInformation[1 - myID].remainingAmmo * 25 < info.robotInformation[myID].hp)
			{
				daweisuo(order, info, myID);
			}
			else
			if (y0 > 340)
			{
				if (info.robotInformation[myID].vy > 0)
				{
					double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
					double rr = theta + 90;
					walkrotation(order, rr, info, myID);
				}
				else if (info.robotInformation[myID].vy < 0)
				{
					double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
					double rr = theta - 90;
					walkrotation(order, rr, info, myID);
				}

			}
			else if (y0 < 340)
			{
				double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
				double rr = theta + 90;
				walkrotation(order, rr, info, myID);
			}

		}
	}
	else if (myID == 0)
	{
		if (info.arsenal[0].respawning_time == 0)
		{
			walkpoint(order, 50, 630, info, myID);
			if (distance < 600)
			{
				if (y0 > 340)
				{
					if (info.robotInformation[myID].vy > 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta + 90;
						walkrotation(order, rr, info, myID);
					}
					else if (info.robotInformation[myID].vy < 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta - 90;
						walkrotation(order, rr, info, myID);
					}

				}
			}
		}
		else
		if (info.arsenal[1].respawning_time == 0)
		{
			walkpoint(order, 1316, 50, info, myID);
			if (distance < 600)
			{
				if (y0 > 340)
				{
					if (info.robotInformation[myID].vy > 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta + 90;
						walkrotation(order, rr, info, myID);
					}
					else if (info.robotInformation[myID].vy < 0)
					{
						double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
						double rr = theta - 90;
						walkrotation(order, rr, info, myID);
					}

				}
			}
		}
		else
		if (distance < 700)
		{
			if (info.robotInformation[1 - myID].remainingAmmo * 25 < info.robotInformation[myID].hp)
			{
				daweisuo(order, info, myID);
			}
			else
			if (y0 > 340)
			{
				if (info.robotInformation[myID].vy > 0)
				{
					double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
					double rr = theta + 90;
					walkrotation(order, rr, info, myID);
				}
				else if (info.robotInformation[myID].vy < 0)
				{
					double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
					double rr = theta - 90;
					walkrotation(order, rr, info, myID);
				}

			}
			else if (y0 < 340)
			{
				double theta = atan2(y1 - y0, x1 - x0)*180.0 / PI;
				double rr = theta + 90;
				walkrotation(order, rr, info, myID);
			}

		}
	}
}