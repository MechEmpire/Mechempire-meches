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


void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)

	double distance;
	const double eps = 1e-3;
	double R = info.robotInformation[myID].circle.r;
	double x0 = info.robotInformation[myID].circle.x;
	double y0 = info.robotInformation[myID].circle.y;
	double x1 = info.robotInformation[1 - myID].circle.x;
	double y1 = info.robotInformation[1 - myID].circle.y;
	double k1 = atan2(y1 - y0, x1 - x0) * 180 / 3.14;
	double k2_jh1 = atan2(y0 - 630, x0 - 50) * 180 / 3.14;
	double k2_jh2 = atan2(y0 - 50, x0 - 1316) * 180 / 3.14;
	double deviation = ((y1 - y0) / (x1 - x0));
	distance = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
	double hittime = distance / 13;
	bool isdj = false, isws = false;
	//判断是否是ws
	if (info.robotInformation[1 - myID].weaponTypeName == WT_Prism || info.robotInformation[1-myID].weaponTypeName == WT_MissileLauncher || info.robotInformation[1-myID].weaponTypeName == WT_Tesla)
		isws = true;
	else
		isws = false;
	//判断是否是电锯
	if (info.robotInformation[1 - myID].weaponTypeName == WT_ElectricSaw)
		isdj = true;
	else
		isdj = false;

	Beam  be1 = { x0, y0, info.robotInformation[myID].weaponRotation, x1, y1 };
	Circle c1 = { 300, 250, 75 }, c2 = { 1066, 430, 75 };
	//射击方式改进

	bool p, p1, p2;
	p = false;
	p1 = HitTestBeamCircle(be1, c1);
	p2 = HitTestBeamCircle(be1, c2);
	if (p1 == true || p2 == true)
		p = true;
	else p = false;
	if (myID == 1)
	{
		if (isws == false)
		{
			double x1n = info.robotInformation[1 - myID].vx * hittime + x1;
			double y1n = info.robotInformation[1 - myID].vy * hittime + y1;
			double dx = x1 - x0;
			double dy = y1 - y0;
			double theta = atan2(dy, dx)*180.0 / PI;

			double dt = theta - info.robotInformation[myID].weaponRotation;
			AngleAdjust(dt);
			if (dt > eps){
				order.wturn = 1;
			}
			else if (dt < -eps){
				order.wturn = -1;
			}
			else {
				order.wturn = 0;

			}
			if (p != 1)
				order.fire = 1;
		}
		else if (isws == true && distance < 500)
		{
			double x1n = info.robotInformation[1 - myID].vx * hittime + x1;
			double y1n = info.robotInformation[1 - myID].vy * hittime + y1;
			double dx = x1 - x0;
			double dy = y1 - y0;
			double theta = atan2(dy, dx)*180.0 / PI;
			double dt = theta - info.robotInformation[myID].weaponRotation;
			AngleAdjust(dt);
			if (dt > eps){
				order.wturn = 1;
			}
			else if (dt < -eps){
				order.wturn = -1;
			}
			else {
				order.wturn = 0;

			}

			if (p != 1)
				order.fire = 1;
		}

	}
	else {
		if (distance > 500 && isws == false)
		{
			double x1n = info.robotInformation[1 - myID].vx * hittime + x1;
			double y1n = info.robotInformation[1 - myID].vy * hittime + y1;
			double dx = x1n - x0;
			double dy = y1n - y0;
			double theta = atan2(dy, dx)*180.0 / PI;
			double dt = theta - info.robotInformation[myID].weaponRotation;
			AngleAdjust(dt);
			if (dt > eps){
				order.wturn = 1;
			}
			else if (dt < -eps){
				order.wturn = -1;
			}
			else {
				order.wturn = 0;

			}

			if (p != 1)
				order.fire = 1;
		}
		else if (distance < 500 && isws == false)
		{
			double dx = x1 - x0;
			double dy = y1 - y0;
			double theta = atan2(dy, dx)*180.0 / PI;

			double dt = theta - info.robotInformation[myID].weaponRotation;
			AngleAdjust(dt);
			if (dt > eps){
				order.wturn = 1;
			}
			else if (dt < -eps){
				order.wturn = -1;
			}
			else {
				order.wturn = 0;

			}
			if (p != 1)
				order.fire = 1;
		}
		else if (isws == true && distance < 500)
		{
			double dx = x1 - x0;
			double dy = y1 - y0;
			double theta = atan2(dy, dx)*180.0 / PI;

			double dt = theta - info.robotInformation[myID].weaponRotation;
			AngleAdjust(dt);
			if (dt > eps){
				order.wturn = 1;
			}
			else if (dt < -eps){
				order.wturn = -1;
			}
			else {
				order.wturn = 0;

			}
			if (p != 1)
				order.fire = 1;
		}

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


	//行走方式改进	
    order.run = 1;
	if (isdj == true)
	{
		if (distance < 800)
		{

			double dt = info.robotInformation[1 - myID].engineRotation;
			AngleAdjust(dt);


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
		if (info.robotInformation[myID].circle.x < 150 && info.robotInformation[myID].circle.y > 400)
			//&& info.robotInformation[myID].engineRotation != -90)
		{

			double dt = -90 - info.robotInformation[myID].engineRotation;
			AngleAdjust(dt);


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
		else if (info.robotInformation[myID].circle.x < 150 && info.robotInformation[myID].circle.y < 100)
			//&& info.robotInformation[myID].engineRotation != 0.0)
		{
			double dt = 0 - info.robotInformation[myID].engineRotation;
			AngleAdjust(dt);


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
		else if (info.robotInformation[myID].circle.x >1300 && info.robotInformation[myID].circle.y < 100)
			//&& info.robotInformation[myID].engineRotation != 90)
		{
			double dt = 90 - info.robotInformation[myID].engineRotation;
			AngleAdjust(dt);


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
		else if (info.robotInformation[myID].circle.x > 1200 && info.robotInformation[myID].circle.y > 500)
			//&& info.robotInformation[myID].engineRotation != 180)
		{
			double dt = 180 - info.robotInformation[myID].engineRotation;
			AngleAdjust(dt);


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
	}
	else if (isws == true)
	{
		double dis_me_za0 = sqrt((300 - x0)*(300 - x0) + (250 - y0)*(250 - y0));
		double dis_me_za1 = sqrt((1066 - x0)*(1066 - x0) + (430 - y0)*(430 - y0));
		double dx1 = x1 - x0;
		double dy1 = y1 - y0;
		double theta1 = atan2(dy1, dx1)*180.0 / PI;
		double dt1 = theta1 - info.robotInformation[myID].engineRotation;
		AngleAdjust(dt1);
		if (distance > 10)
		{
			if (dis_me_za0 < 200)
			{
				double dt11 = dt1 + 35;
				AngleAdjust(dt11);
				if (dt11 > eps){
					order.eturn = 1;
				}
				else if (dt11 < -eps){
					order.eturn = -1;
				}
				else {
					order.eturn = 0;

				}
			}
			else if (dis_me_za1 < 200)
			{
				double dt12 = dt1 + 35;
				AngleAdjust(dt12);
				if (dt12 > eps){
					order.eturn = 1;
				}
				else if (dt12 < -eps){
					order.eturn = -1;
				}
				else {
					order.eturn = 0;

				}
			}else
			if (dis_me_za0 > 75 || dis_me_za1 >75)
			{
				if (dt1 > eps){
					order.eturn = 1;
				}
				else if (dt1 < -eps){
					order.eturn = -1;
				}
				else {
					order.eturn = 0;

				}
			}
			

		}
		else
		{
			if (dt1 > eps){
				order.eturn = 1;
			}
			else if (dt1 < -eps){
				order.eturn = -1;
			}
			else {
				order.eturn = 0;

			}
		}

		
	}else
	{
		double dx1 = 500 - x0;
		double dy1 = 500 - y0;
		double theta1 = atan2(dy1, dx1)*180.0 / PI;
		double dt1 = theta1 - info.robotInformation[myID].engineRotation;
		AngleAdjust(dt1);


		if (dt1 > eps){
			order.eturn = 1;
		}
		else if (dt1 < -eps){
			order.eturn = -1;
		}
		else {
			order.eturn = 0;

		}

		//以前的行走方式 绕圈
		if (info.robotInformation[myID].circle.x < 150 && info.robotInformation[myID].circle.y > 400)
			//&& info.robotInformation[myID].engineRotation != -90)
		{

			double dt = -90 - info.robotInformation[myID].engineRotation;
			AngleAdjust(dt);


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
		else if (info.robotInformation[myID].circle.x < 150 && info.robotInformation[myID].circle.y < 100)
			//&& info.robotInformation[myID].engineRotation != 0.0)
		{
			double dt = 0 - info.robotInformation[myID].engineRotation;
			AngleAdjust(dt);


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
		else if (info.robotInformation[myID].circle.x >1300 && info.robotInformation[myID].circle.y < 100)
			//&& info.robotInformation[myID].engineRotation != 90)
		{
			double dt = 90 - info.robotInformation[myID].engineRotation;
			AngleAdjust(dt);


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
		else if (info.robotInformation[myID].circle.x > 1200 && info.robotInformation[myID].circle.y > 500)
			//&& info.robotInformation[myID].engineRotation != 180)
		{
			double dt = 180 - info.robotInformation[myID].engineRotation;
			AngleAdjust(dt);


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
		else
		{
			order.eturn = 0;
		}
		//绕行robot
		double dis_xu = y0;
		double dis_xb = 680 - y0;
		double dis_yl = x0;
		double dis_yr = 1366 - x0;
		if (distance < 600)
		{
			if (dis_xu > dis_xb)
			{
				double rr = info.robotInformation[1 - myID].engineRotation + 90;
				AngleAdjust(rr);
				if (rr > eps){
					order.eturn = 1;
				}
				else if (rr < -eps){
					order.eturn = -1;
				}
				else {
					order.eturn = 0;

				}
			}
			else if (dis_xu < dis_xb)
			{
				double rr = info.robotInformation[1 - myID].engineRotation + 90;
				AngleAdjust(rr);
				if (rr > eps){
					order.eturn = -1;
				}
				else if (rr < -eps){
					order.eturn = 1;
				}
				else {
					order.eturn = 0;

				}
			}
		}
		/*double vx_bullet_near, vy_bullet_near, dis_bullet_near= 9999, dis_bullet,db;
		int i_b;
		for (i_b = 0; i_b < info.num_bullet; i_b++)
		{
			if (info.bulletInformation[i_b].launcherID == 1 - myID)
			{
				dis_bullet = sqrt((info.bulletInformation[i_b].circle.x - x0) * (info.bulletInformation[i_b].circle.x - x0) + (info.bulletInformation[i_b].circle.y - y0)*(info.bulletInformation[i_b].circle.y - y0));
			}
			if (dis_bullet < dis_bullet_near)
			{
				dis_bullet_near = dis_bullet;
				vx_bullet_near = info.bulletInformation[i_b].vx;
				vy_bullet_near = info.bulletInformation[i_b].vy;
				db = atan2(vy_bullet_near, vx_bullet_near);
			}
		}
		if ()*/



	}

	//碰边检测调整
	/*
	if (dis_xu < R && info.robotInformation[myID].engineRotation < )
	{
	double ro;
	if (info.robotInformation[myID].engineRotation )
	double dt = 180 - info.robotInformation[myID].engineRotation;
	AngleAdjust(dt1);


	if (dt > eps){
	order.eturn = 1;
	}
	else if (dt < -eps){
	order.eturn = -1;
	}
	else {
	order.eturn = 0;

	}

	}*/

	//info.arsenal[q].respawning_time;
	//捡弹药库
	if (info.robotInformation[myID].remainingAmmo == 0)
	{
		if (b1 == 2 && info.arsenal[0].respawning_time == 0)
		{
			double dx11 = 50 - x0;
			double dy11 = 630 - y0;
			double theta11 = atan2(dy11, dx11)*180.0 / PI;

			double dt11 = theta11 - info.robotInformation[myID].engineRotation;
			AngleAdjust(dt11);

			if (dt11 > eps){
				order.eturn = 1;
			}
			else if (dt11 < -eps){
				order.eturn = -1;
			}
			else {
				order.eturn = 0;

			}
		}
		else if (b1 == 1 && info.arsenal[1].respawning_time == 0)
		{
			double dx12 = 1316 - x0;
			double dy12 = 50 - y0;
			double theta12 = atan2(dy12, dx12)*180.0 / PI;

			double dt12 = theta12 - info.robotInformation[myID].engineRotation;
			AngleAdjust(dt12);

			if (dt12 > eps){
				order.eturn = 1;
			}
			else if (dt12 < -eps){
				order.eturn = -1;
			}
			else {
				order.eturn = 0;

			}
		}
		else
		{
			if (dis_meammo1 > dis_meammo2 && info.arsenal[1].respawning_time == 0)
			{
				double dx13 = 1316 - x0;
				double dy13 = 50 - y0;
				double theta13 = atan2(dy13, dx13)*180.0 / PI;

				double dt13 = theta13 - info.robotInformation[myID].engineRotation;
				AngleAdjust(dt13);

				if (dt13 > eps){
					order.eturn = 1;
				}
				else if (dt13 < -eps){
					order.eturn = -1;
				}
				else {
					order.eturn = 0;

				}
			}
			else if (info.arsenal[0].respawning_time == 0)
			{
				double dx14 = 50 - x0;
				double dy14 = 630 - y0;
				double theta14 = atan2(dy14, dx14)*180.0 / PI;

				double dt14 = theta14 - info.robotInformation[myID].engineRotation;
				AngleAdjust(dt14);

				if (dt14 > eps){
					order.eturn = 1;
				}
				else if (dt14 < -eps){
					order.eturn = -1;
				}
				else {
					order.eturn = 0;

				}
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

	weapon = WT_Cannon;	//啊，我爱加农炮
	engine = ET_GhostTank;	//啊，我爱小蜘蛛
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "EVA0.2";
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
	return 30;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return -255;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return 40;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 34;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 9;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 17;
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