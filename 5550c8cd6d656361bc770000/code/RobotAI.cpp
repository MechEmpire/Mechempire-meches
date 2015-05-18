#include "RobotAI.h"
#include <iostream>
#include <queue>
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


void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	RobotAI_RobotInformation me = info.robotInformation[myID], armor = info.robotInformation[1 - myID];
	Circle arsenal = whichArsenal(info.arsenal[0], info.arsenal[1], me.circle);
	Circle obs[5];
	for(int i = 0; i < info.num_obstacle; i++)
		obs[i] = info.obstacle[i];
	Circle obstacle = whichObstacle(obs[0],obs[1],me.circle);//离得近的障碍物
	double dist_me_armor = Distance(me.circle.x, me.circle.y, armor.circle.x, armor.circle.y);//我和敌人的距离
	double dist_me_obstacle = Distance(me.circle.x, me.circle.y, obstacle.x, obstacle.y);//到障碍物的距离
	double fire_angle = howToRotate(me.circle, armor.circle, me.weaponRotation, armor.vx, armor.vy);//应该旋转的炮台角度
	double engine_angle = howToRotate(me.circle, armor.circle, me.engineRotation, armor.vx, armor.vy);//应该旋转的引擎角度
	
	
	//何时开火
	if( dist_me_armor < me.circle.r + armor.circle.r + 45)
		order.fire = 1;
		//Distance(me.circle.x, me.circle.y, armor.circle.x, armor.circle.y) < 110 ? 1: 0;

	//根据对方的位置调整炮塔角度
	order.wturn = Rotate(fire_angle, me.weaponRotation);
	order.run = 1;

	if(dist_me_obstacle > 140)
		order.eturn = runAndrun(me, obstacle);
	else{
		double engine_angle2 = -90 + howToRotate(me.circle, obstacle, me.engineRotation, armor.vx, armor.vy);
		AngleAdjust(engine_angle2);
		order.eturn = Rotate(engine_angle2, me.engineRotation);
	}
	if(armor.remainingAmmo <= 0)
		order.eturn = Rotate(engine_angle, me.engineRotation);
	if(dist_me_armor <= 200)
		order.eturn = Rotate(engine_angle, me.engineRotation);
	//抢占弹药库！
	//Circle ars = whichArsenal(info.arsenal[0], info.arsenal[1], me.circle);
	//if(!(ars.x == 0 && ars.y == 0))
	//	order.eturn = runAndrun(me,ars);

	//靠墙
	if(me.circle.x <= 60 || me.circle.y <= 60 || 680 - me.circle.y <= 60 || 1366 - me.circle.x <= 60)
		order.eturn = avoidWall(me);
	//等等！有子弹再打我？
	//Circle bu[200];
	int bu_num = info.num_bullet;
	for(int i = 0; i < bu_num; i++){
		if(info.bulletInformation[i].launcherID == 1 - myID){			
			if(BulletShotMe(info.bulletInformation[i].circle, me.circle, info.bulletInformation[i].vx, info.bulletInformation[i].vy, armor.weaponTypeName)){
				//赶紧躲开！
				if(armor.weaponTypeName == WT_Apollo || armor.weaponTypeName == WT_Cannon)
					order.eturn = AvoidCannonAFV(info.bulletInformation[i],me);
				else
					order.eturn = AvoidBulletAFV(info.bulletInformation[i], me);				
				break;//先检测一颗子弹吧
			}
		}
	}
	if(armor.weaponTypeName == WT_MissileLauncher || armor.weaponTypeName == WT_ElectricSaw || armor.weaponTypeName == WT_Tesla
		|| armor.weaponTypeName == WT_Prism)
	{
		order.eturn = runAndrun(me, armor.circle);
		int obs_n = StrikeObstacle(me.circle, obs, info.num_obstacle);
		if(obs_n != -1)
			order.eturn = avoidObstacleAFV(me, obs[obs_n]);
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

	weapon = WT_ElectricSaw;	//啊，我爱加农炮
	engine = ET_GhostTank;	//啊，我爱小蜘蛛
}




//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------

string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "猥琐小锯子";
}
string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "GUIGE";
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
	//system("pause");
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
int RobotAI::runAndrun(RobotAI_RobotInformation me, Circle target)
{
	double an = atan2(target.y - me.circle.y, target.x - me.circle.x) * 180 / PI;		
	return Rotate(an,me.engineRotation);
}
//选则一个可以去的军火库
Circle RobotAI::whichArsenal(RobotAI_ArsenalInformation ar1, RobotAI_ArsenalInformation ar2, Circle me)
{
	if(ar1.respawning_time == 0 && ar2.respawning_time == 0)
	{
		double dis_ars1 = Distance(ar1.circle.x, ar1.circle.y, me.x, me.y);
		double dis_ars2 = Distance(ar2.circle.x, ar2.circle.y, me.x, me.y);
		if(dis_ars1 < dis_ars2){
			return ar1.circle;
		}else{
			return ar2.circle;
		}
	}else if(ar1.respawning_time == 0 || ar1.respawning_time < ar2.respawning_time)	
		return ar1.circle;
	else if(ar2.respawning_time == 0 || ar1.respawning_time > ar2.respawning_time)
		return ar2.circle;
	else if(ar1.respawning_time == ar2.respawning_time)
		return ar2.circle;
	Circle c;
	return c;
}
bool RobotAI::BulletShotMe(Circle bu, Circle me, double vx, double vy, weapontypename weapontype)
{
	int min_dis = 350;
	if(weapontype == WT_Apollo || weapontype == WT_RPG || weapontype == WT_Cannon)
		min_dis = 650;
	//Distance(bu.x, bu.y, me.x, me.y)
	//打向我的角度
	double bulletAngle = atan2(vy,vx) * 180 / PI;
	//我与子弹的角度
	double A = vy / vx;
	double C = bu.y - A * bu.x;
	double dist = abs(A * me.x - me.y + C) / sqrt(A*A + 1);
	if(dist <= 50 && Distance(bu.x, bu.y, me.x, me.y) <= min_dis)
		return true;	
	return false;
}
int RobotAI::avoidObstacleAFV(RobotAI_RobotInformation me, Circle obstacle)
{
	double angle = atan2(obstacle.y - me.circle.y, obstacle.x - me.circle.x) * 180 / PI;//我和障碍物的角度
	if( Rotate(me.engineRotation, angle) == 1)
		angle += 90;
	else
		angle -= 90;
	AngleAdjust(angle);
	return Rotate(angle, me.engineRotation);
}
int RobotAI::StrikeObstacle(Circle me, Circle obstacle[],const int num_obs)
{
	double *dist = new double[num_obs];
	for(int i = 0; i < num_obs; i++)
	{
		dist[i] = Distance(me.x, me.y, obstacle[i].x, obstacle[i].y);
		if(dist[i]<=135)
		{
			delete dist;
			return i;
		}
	}
	delete dist;
	return -1;
}
Circle RobotAI::whichObstacle(Circle ob1, Circle ob2, Circle me)
{
	double d1 = Distance(me.x,me.y,ob1.x,ob1.y);
	double d2 = Distance(me.x,me.y,ob2.x,ob2.y);
	if(d1 <= d2)
		return ob1;
	else
		return ob2;
}
int RobotAI::AvoidBulletAFV(RobotAI_BulletInformation bu, RobotAI_RobotInformation me)
{
	double bu_angle = atan2(bu.vy, bu.vx) * 180 / PI;
	//double angle = atan2(me.circle.y - bu.circle.y, me.circle.x - bu.circle.x) *180/PI;
	if(me.circle.x >341 && me.circle.x < 1025)
	{
		if(me.circle.y <= 340)
			bu_angle = 90;
		else
			bu_angle = -90;
	}else{
		if(me.circle.x <= 341)
			bu_angle = 0;
		else
			bu_angle = 180;
	}
	return Rotate(bu_angle, me.engineRotation);	
}
//旋转函数
int RobotAI::Rotate(double angle_to, double angle_now){
	if(angle_to > 0 && angle_to < 180){
		if(angle_to - 180 < angle_now && angle_now < angle_to)
			return +1;
		else
			return -1;
	}
	else if(angle_to < 0 && angle_to > -180){		
		if(angle_to < angle_now && angle_now < angle_to + 180)
			return -1;
		else
			return 1;
	}else if(angle_to == 0){
		if(angle_now > 0)
			return -1;
		else
			return 1;
	}else{
		if(angle_now >0)
			return 1;
		else
			return -1;
	}
}

//距离函数
double RobotAI::Distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}
double RobotAI::howToRotate(Circle me, Circle armor,double weapon_rotation, double vx, double vy)
{
	return atan2(armor.y - me.y, armor.x - me.x) * 180 / PI;//我和敌人之间的角度
}
//得到与敌人距离500的点
Circle RobotAI::get500Point(Circle me, Circle armor, double dist)
{
	Circle p;
	double factor = 850 / dist;
	p.x = factor * (me.x - armor.x) + armor.x;
	p.y = factor * (me.y - armor.y) + armor.y;
	return p;
}
int RobotAI::avoidWall(RobotAI_RobotInformation me)
{
	double angle;//应该转的方向
	//遇到墙则反弹
	if(680 - me.circle.y <= 70)
	{
		if(Rotate(-90, me.engineRotation) == 1)
			angle = -145;
		else
			angle = -45;
	}else if(me.circle.y <= 70){
		if(Rotate(90, me.engineRotation) == 1)
			angle = 45;
		else
			angle = 135;
	}else if(me.circle.x <= 70){
		if(Rotate(0, me.engineRotation) == 1)
			angle = -45;
		else
			angle = 45;
	}else if(1366 - me.circle.x <= 70)
		if(Rotate(180, me.engineRotation) == 1)
			angle = 135;
		else
			angle = -135;	
	return Rotate(angle, me.engineRotation);
}
int RobotAI::AvoidCannonAFV(RobotAI_BulletInformation bu, RobotAI_RobotInformation me)
{
	double bu_angle = atan2(bu.vy, bu.vx) * 180 / PI;//子弹角度
	double angle = atan2(me.circle.y - bu.circle.y, me.circle.x - me.circle.x) * 180 / PI;//子弹与我的角度
	if(Rotate(angle, bu_angle) == 1) //子弹攻击上方
		bu_angle += 90;
	else                             //子弹攻击下方
		bu_angle -= 90;
	bu_angle = AngleAdjust(bu_angle);
	return Rotate(bu_angle, me.engineRotation);
}
