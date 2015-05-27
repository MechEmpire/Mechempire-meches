#include "RobotAI.h"
#include<iostream>
#define Rleft (order.run =1)
#define Rright (order.run =2)
#define Rup (order.run =3)
#define Rdown (order.run =4)
double howfar(double x, double y, double x1, double y1)
{
	return sqrt(pow(x-x1,2) + pow(y-y1,2));
}
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

aa turnfire(Circle B, double rf, Circle o ,int state)
{
	double A;
	int fire = 0;
	int wturn = 0;
	aa a;
	A = howfar(B.x, B.y, o.x, o.y);
	if (state == 0)
	{
		if (A <= o.r) 
		{
			fire = 1; 
			double ro0;
			ro0 = getAngle(B, o);
			double rf0;
			rf0 = AnglePlus(rf, -ro0);
			if (rf0 > 0) wturn = -1;
			else wturn = 1;

		}
		else
		{
			double ro;
			ro = RadianToAngle(asin(o.r / A));
			double ro0;
			ro0 = getAngle(B, o);
			double rf0;
			rf0 = AnglePlus(rf, -ro0);
			if (abs(rf0) > 2.5 + ro)
			{
				fire = 0;
				if (rf0 > 0) wturn = -1;
				else wturn = 1;
			}
			if (abs(rf0) <= 2.5 + ro && abs(rf0) >= ro - 2.5)
			{
				double a;
				a = (ro - abs(rf0) + 2.5) / 5;
				if (a > 0.999) fire = 1;
				else  fire = 0;
				if (rf0 > 0) wturn = -1;
				else wturn = 1;
			}
			if (abs(rf0) < ro - 2.5) fire = 1;
		}
	}
	else
	{
		if (A <= o.r)
		{ 
			fire = 0; 
			double ro0;
			ro0 = getAngle(B, o);
			double rf0;
			rf0 = AnglePlus(rf, -ro0);
			if (rf0 > 0) wturn = 1;
			else wturn = -1;
		}
		else
		{
			double ro;
			ro = RadianToAngle(asin(o.r / A));
			double ro0;
			ro0 = getAngle(B, o);
			double rf0;
			rf0 = AnglePlus(rf, -ro0);
			if (abs(rf0) > 2.5 + ro) fire = 1;
			if (abs(rf0) <= 2.5 + ro && abs(rf0) >= ro - 2.5)
			{
				double a;
				a = (abs(rf0) - ro + 2.5) / 5;
				if (a > 0.999) fire = 1;
				else  fire = 0;
				if (rf0 > 0) wturn = 1;
				else wturn = -1;
			}
			if (abs(rf0) < ro - 2.5) 
			{
				fire = 0; 
				if (rf0 > 0) wturn = 1;
				else wturn = -1;
			}
		}
	}
	a.x = fire;
	a.y = wturn;
	return a;
}

void controlfire(Circle m, Circle o, double rf, int&fire, int&wturn)
{
	fire = 0;
	Circle B = {0};
	B.x = m.x + 76 * cos(AngleToRadian(rf));
	B.y = m.y + 76 * sin(AngleToRadian(rf));
	aa a;
	a = turnfire(B, rf, o, 0);
	fire = a.x;
	wturn = a.y;
	Circle A[2] = { 300, 250, 76, 1066, 430, 76 };
	if (howfar(B.x, B.y, A[0].x, A[0].y) <= howfar(B.x, B.y, o.x, o.y))
	{
		a = turnfire(B, rf, A[0], 1);
		if (fire == 1) fire = a.x;
		if (wturn == 0) wturn = a.y;
	}
	if (howfar(B.x, B.y, A[1].x, A[1].y) <= howfar(B.x, B.y, o.x, o.y))
	{
		a = turnfire(B, rf, A[1], 1);
		if (fire == 1) fire = a.x;
		if (wturn == 0) wturn = a.y;
	}
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

	double rf = info.robotInformation[myID].weaponRotation;
	Circle m = info.robotInformation[myID].circle;
	Circle o = info.robotInformation[1 - myID].circle;
	o.r = o.r - 1;
	Circle v;
	v.x = info.robotInformation[1 - myID].vx;
	v.y = info.robotInformation[1 - myID].vy;
	v.r = info.robotInformation[1 - myID].vr;
	double l, re;
	l = howfar(0, 0, v.x, v.y);
	re = AngleToRadian(info.robotInformation[1 - myID].engineRotation + v.r);
	v.x = l*cos(re);
	v.y = l*sin(re);
	//cout << v.x << '\n';
	//system("pause");
	controlfire(m, o, rf, order.fire, order.wturn);

	RobotAI_BulletInformation a[200];
	for (int i = 0; i < info.num_bullet; ++i)
		a[i] = info.bulletInformation[i];

	order.run = 1;
	double ro;
	ro = getAngle(m, o);
	ro = AnglePlus(info.robotInformation[myID].engineRotation, -ro);
	if (info.robotInformation[1 - myID].weaponTypeName != WT_ElectricSaw)
		{
			if (howfar(m.x, m.y, o.x, o.y) > 900)
			{
				if (ro > 3) order.eturn = -1;
				if (ro < -3) order.eturn = 1;

			}
			if (howfar(m.x, m.y, o.x, o.y) < 700)
			{
				if (ro > 0) order.eturn = 1;
				if (ro < 0) order.eturn = -1;
			}
			int a12 = myID;
			if (info.robotInformation[myID].remainingAmmo <= 1)
			{
				if (howfar(info.arsenal[myID].circle.x, info.arsenal[myID].circle.y, m.x, m.y) <= howfar(info.arsenal[1 - myID].circle.x, info.arsenal[1 - myID].circle.y, m.x, m.y))
					a12 = myID;
				else
					a12 = 1 - myID;
				if (info.arsenal[a12].respawning_time>100)
					a12 = 1 - a12;
				if (info.arsenal[a12].respawning_time > 100)
					a12 = 3;
				if (a12 == 3){}
				else
				{
					double ro1;
					ro1 = getAngle(m, info.arsenal[a12].circle);
					ro1 = AnglePlus(info.robotInformation[a12].engineRotation, -ro1);
					if (ro1 > 3)order.eturn = -1;
					if (ro1 < -3)order.eturn = 1;
				}

			}

			if (howfar(info.obstacle[1 - myID].x, info.obstacle[1 - myID].y, m.x, m.y) < 150)
			{
				double ro1;

				ro1 = getAngle(m, info.obstacle[1 - myID]);
				if (abs(AnglePlus(AnglePlus(ro1, 90), -info.robotInformation[myID].engineRotation))
			> abs(AnglePlus(AnglePlus(ro1, -90), -info.robotInformation[myID].engineRotation))
			)ro1 = AnglePlus(ro1, -90);
				else ro1 = AnglePlus(ro1, 90);

				ro1 = AnglePlus(info.robotInformation[myID].engineRotation, -ro1);
				if (howfar(info.obstacle[1 - myID].x, info.obstacle[1 - myID].y, m.x, m.y)
					> howfar(info.obstacle[1 - myID].x + info.robotInformation[myID].vx, info.obstacle[1 - myID].y + info.robotInformation[myID].vy, m.x, m.y))
				{
					if (ro1 > 0)order.eturn = -1;
					if (ro1 < 0)order.eturn = 1;
				}
			}

			if (howfar(info.obstacle[myID].x, info.obstacle[myID].y, m.x, m.y) < 150)
			{

				double ro1;
				ro1 = getAngle(m, info.obstacle[myID]);
				if (abs(AnglePlus(AnglePlus(ro1, 90), -info.robotInformation[myID].engineRotation))
			> abs(AnglePlus(AnglePlus(ro1, -90), -info.robotInformation[myID].engineRotation))
			)ro1 = AnglePlus(ro1, -90);
				else ro1 = AnglePlus(ro1, 90);

				ro1 = AnglePlus(info.robotInformation[myID].engineRotation, -ro1);
				if (howfar(info.obstacle[1 - myID].x, info.obstacle[1 - myID].y, m.x, m.y)
					> howfar(info.obstacle[1 - myID].x + info.robotInformation[myID].vx, info.obstacle[1 - myID].y + info.robotInformation[myID].vy, m.x, m.y))
				{
					if (ro1 > 0)order.eturn = -1;
					if (ro1 < 0)order.eturn = 1;
				}
			}
			double rem = info.robotInformation[myID].engineRotation;
			if (m.y < 80)
			{
				if (rem >= -90 && rem < 0)order.eturn = 1;
				if (rem < -90)order.eturn = -1;
			}
			if (m.x > 1286)
			{
				if (rem >= 0 && rem < 90)order.eturn = 1;
				if (rem <= 0 && rem>-90)order.eturn = -1;
			}

			if (m.x < 80)
			{
				if (rem >90)order.eturn = -1;
				if (rem <-90)order.eturn = 1;
			}
			if (m.y > 600)
			{
				if (rem>0 && rem <= 90)order.eturn = -1;
				if (rem>90)order.eturn = 1;
			}

			Circle a1 = { 0 };
			for (int i = 0; i < 200; ++i)
			{
				if (a[i].launcherID == 1 - myID&&howfar(o.x, o.y, m.x, m.y) > howfar(o.x, o.y, a[i].circle.x, a[i].circle.y) + 50)
				{
					Circle B = a[i].circle;
					B.r = a[i].vy / a[i].vx;
					v.x = info.robotInformation[myID].vx;
					v.y = info.robotInformation[myID].vy;
					v.r = info.robotInformation[myID].vr;
					Circle p = { 0 };
					double k = v.y / v.x;
					if (v.x != 0)
					{
						p.x = (o.y - k*o.x + B.r*B.x - B.y) / (B.r - k);
						p.y = (B.r*o.y - k*B.y + B.r*k*(B.x - o.x)) / (B.r - k);
					}
					if (v.x == 0)
					{
						p.x = o.x;
						p.y = B.r*(o.x - B.x) + B.y;
					}
					if (p.x > 0 && p.x < 1366 && p.y>0 && p.y < 680)
					{
						double t1, t2 = 0;
						t1 = (p.x - B.x) / (11 * cos(rf*PI / 180));
						if (t1 < 1 && t1>0){ t1 = (p.y - B.y) / (11 * sin(rf*PI / 180)); }
						Circle p1;
						if (t1 > 0)
						{
							p1.x = o.x + t1*v.x;
							p1.y = o.y + t1*v.y;
							if (howfar(p1.x, p1.y, p.x, p.y) < 1.6*o.r || abs(a[i].rotation - rem)<2 || abs(AnglePlus(a[i].rotation, 180) - rem)<2)
							{
								a1.x = a[i].circle.x;
								a1.y = a[i].circle.y;
								a1.r++;
							}
						}
					}
				}
			}
			a1.x /= a1.r;
			a1.y /= a1.r;
			//system("pause");
			double pp = getAngle(m, a1);
			pp = AnglePlus(rem, -pp);
			if (pp >= 0)order.eturn = 1;
			if (pp< 0)order.eturn = -1;
			//	cout << pp << '\n' << rem<< '\n';
			if (info.robotInformation[myID].remainingAmmo > 1 || info.arsenal[a12].respawning_time > 0)
			{
				if (m.x < 80 && m.y < 80)
				{
					if (rem >= -135 && rem <= -45)order.eturn = 1;
					if (rem>135 || rem <= -135)order.eturn = -1;
				}
				if (m.x > 1286 && m.y < 80)
				{
					if (rem >= -45 && rem <= 45)order.eturn = 1;
					if (rem>-135 && rem < -45)order.eturn = -1;
				}
				if (m.x > 1286 && m.y > 600)
				{
					if (rem <= 135 && rem >= 45)order.eturn = 1;
					if (rem<45 && rem > -45)order.eturn = -1;
				}
				if (m.x < 80 && m.y>600)
				{
					if (rem <= 135 && rem >= 45)order.eturn = -1;
					if (rem>135 || rem <= -135)order.eturn = 1;
				}
			}
		static int tt = 0;
		tt++;
		if (tt < 45)order.eturn = 0;
	}
	else
	{
		if (howfar(m.x, m.y, o.x, o.y) > 700)
		{
			if (ro>3) order.eturn = -1;
			if (ro < 3) order.eturn = 1;

		}
		else
		{
			if (ro>0) order.eturn = 1;
			if (ro < 0) order.eturn = -1;
		}

		int a12 = myID;
		if (info.robotInformation[myID].remainingAmmo <= 1)
		{
			if (howfar(info.arsenal[myID].circle.x, info.arsenal[myID].circle.y, m.x, m.y) <= howfar(info.arsenal[1 - myID].circle.x, info.arsenal[1 - myID].circle.y, m.x, m.y))
				a12 = myID;
			else
				a12 = 1 - myID;
			if (info.arsenal[a12].respawning_time>100)
				a12 = 1 - a12;
			if (info.arsenal[a12].respawning_time > 100)
				a12 = 3;
			if (a12 == 3){}
			else
			{
				double ro1;
				ro1 = getAngle(m, info.arsenal[a12].circle);
				ro1 = AnglePlus(info.robotInformation[a12].engineRotation, -ro1);
				if (ro1 > 3)order.eturn = -1;
				if (ro1 < -3)order.eturn = 1;
			}

		}

		if (howfar(info.obstacle[1 - myID].x, info.obstacle[1 - myID].y, m.x, m.y) < 150)
		{
			double ro1;

			ro1 = getAngle(m, info.obstacle[1 - myID]);
			if (abs(AnglePlus(AnglePlus(ro1, 90), -info.robotInformation[myID].engineRotation))
		> abs(AnglePlus(AnglePlus(ro1, -90), -info.robotInformation[myID].engineRotation))
		)ro1 = AnglePlus(ro1, -90);
			else ro1 = AnglePlus(ro1, 90);

			ro1 = AnglePlus(info.robotInformation[myID].engineRotation, -ro1);
			if (howfar(info.obstacle[1 - myID].x, info.obstacle[1 - myID].y, m.x, m.y)
				> howfar(info.obstacle[1 - myID].x + info.robotInformation[myID].vx, info.obstacle[1 - myID].y + info.robotInformation[myID].vy, m.x, m.y))
			{
				if (ro1 > 0)order.eturn = -1;
				if (ro1 < 0)order.eturn = 1;
			}
		}

		if (howfar(info.obstacle[myID].x, info.obstacle[myID].y, m.x, m.y) < 150)
		{

			double ro1;
			ro1 = getAngle(m, info.obstacle[myID]);
			if (abs(AnglePlus(AnglePlus(ro1, 90), -info.robotInformation[myID].engineRotation))
		> abs(AnglePlus(AnglePlus(ro1, -90), -info.robotInformation[myID].engineRotation))
		)ro1 = AnglePlus(ro1, -90);
			else ro1 = AnglePlus(ro1, 90);

			ro1 = AnglePlus(info.robotInformation[myID].engineRotation, -ro1);
			if (howfar(info.obstacle[1 - myID].x, info.obstacle[1 - myID].y, m.x, m.y)
				> howfar(info.obstacle[1 - myID].x + info.robotInformation[myID].vx, info.obstacle[1 - myID].y + info.robotInformation[myID].vy, m.x, m.y))
			{
				if (ro1 > 0)order.eturn = -1;
				if (ro1 < 0)order.eturn = 1;
			}
		}
		double rem = info.robotInformation[myID].engineRotation;
		if (m.y < 150)
		{
			if (rem >= -90 && rem < 0)order.eturn = 1;
			if (rem < -90)order.eturn = -1;
		}
		if (m.x > 1206)
		{
			if (rem >= 0 && rem < 90)order.eturn = 1;
			if (rem <= 0 && rem>-90)order.eturn = -1;
		}

		if (m.x < 150)
		{
			if (rem >90)order.eturn = -1;
			if (rem <-90)order.eturn = 1;
		}
		if (m.y > 520)
		{
			if (rem>0 && rem <= 90)order.eturn = -1;
			if (rem>90)order.eturn = 1;
		}
		if (info.robotInformation[myID].remainingAmmo > 1 || info.arsenal[a12].respawning_time > 0)
		{
			if (m.x < 150 && m.y < 150)
			{
				if (rem >= -135 && rem <= -45)order.eturn = 1;
				if (rem>135 || rem <= -135)order.eturn = -1;
			}
			if (m.x > 1286 && m.y < 80)
			{
				if (rem >= -45 && rem <= 45)order.eturn = 1;
				if (rem>-135 && rem < -45)order.eturn = -1;
			}
			if (m.x > 1206 && m.y > 520)
			{
				if (rem <= 135 && rem >= 45)order.eturn = 1;
				if (rem<45 && rem > -45)order.eturn = -1;
			}
			if (m.x < 80 && m.y>600)
			{
				if (rem <= 135 && rem >= 45)order.eturn = -1;
				if (rem>135 || rem <= -135)order.eturn = 1;
			}
		}
		static int tt = 0;
		tt++;
		if (tt < 45)order.eturn = 0;
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
	engine = ET_GhostTank;	//啊，我爱
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "细雨朦胧";
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