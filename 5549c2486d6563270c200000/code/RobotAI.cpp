﻿#include "RobotAI.h"
#include "math.h"
#include "algorithm"
#include "fstream"
#include <iostream>
#include <queue>
using namespace std;
RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}
#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b
ofstream fout("h:\\test.txt");
Circle lastPlace[2];//上一帧的位置，0是我，1是敌人
queue<Circle> queue_lastFivePoint;//用于记录前五帧敌人的位置
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
	double distance_me_armor = Distance(me.circle.x, me.circle.y, armor.circle.x, armor.circle.y);//我和敌人的距离
	double fire_angle = howToRotate(me.circle, armor.circle, me.weaponRotation, armor.vx, armor.vy);//应该旋转的角度
	//根据是否有障碍物决定是否开火
	Circle obs[5];
	for(int i = 0; i < info.num_obstacle; i++)
		obs[i] = info.obstacle[i];
	
	//针对太空要塞做优化，贴近点打！
	if( armor.engineTypeName != ET_Shuttle)
		order.fire = doIFire(me.circle, armor, obs, info.num_obstacle, me.weaponRotation, fire_angle);
	else{
		if(distance_me_armor > 700)
			order.fire = 0;
		else
			order.fire = 1;
	}

	//根据对方的位置调整炮塔角度
	order.wturn = Rotate(fire_angle, me.weaponRotation);

	//杀向敌人！
	order.run = 1;
	order.eturn =  runAndrunAFV(me.circle, armor.circle, me.engineRotation);
	
		
	
	
	
	

	//检查子弹打光没	
	if(info.robotInformation[myID].remainingAmmo <= 2 )
	{	
			Circle arsenal = whichArsenal(info.arsenal[0], info.arsenal[1], me.circle);
			//没子弹了就去仓库领取
			if(!(arsenal.x == 0 && arsenal.y == 0))
				order.eturn = runAndrunAFV(me.circle,arsenal ,me.engineRotation);	
	}
	//等等！有子弹再打我？
	//Circle bu[200];
	int bu_num = info.num_bullet + 1;//子弹数目有问题，应该是少了一个
	bullettypename bu_type;//子弹类型
	for(int i = 0; i < bu_num; i++){
		if(info.bulletInformation[i].launcherID == 1 - myID){			
			if(BulletShotMe(info.bulletInformation[i], me)){
				//赶紧躲开！
				bu_type = info.bulletInformation[i].type;
				if( bu_type == BT_Cannonball || bu_type == BT_ApolloBall || bu_type == BT_RPGBall)//针对阿波罗、加农炮、火箭有不一样的躲法
					order.eturn = AvoidCannonAFV(info.bulletInformation[i], me);
				else
					order.eturn = AvoidBulletAFV(info.bulletInformation[i], me);
				
				break;//先检测一颗子弹吧
			}
		}
	}

	//判断距离，不能太近	
	double min_dist = 550;
	//打猥琐飞弹和光棱只要贴脸干！其他的要远离，尤其是电锯！
	if(armor.weaponTypeName == WT_MissileLauncher || armor.weaponTypeName ==WT_Prism || armor.weaponTypeName == WT_PlasmaTorch)
		min_dist = 150;
	if(armor.weaponTypeName == WT_RPG)
		min_dist = 300;
	if(distance_me_armor <= min_dist)//太近了就往回走
		order.eturn =  runAndrunAFV(armor.circle, me.circle, me.engineRotation);
	
	
	//靠墙？
	if(me.circle.x <= 60 || me.circle.y <= 60 || 680 - me.circle.y <= 60 || 1366 - me.circle.x <= 60)
		order.eturn = avoidWall(me);
	//遇到障碍物？
	int obs_n = StrikeObstacle(me.circle, obs, info.num_obstacle);
	if(obs_n != -1)
		order.eturn = avoidObstacleAFV(me, obs[obs_n]);
	
	if(queue_lastFivePoint.size() >= 5)//仅保存5个点
		queue_lastFivePoint.pop();
	queue_lastFivePoint.push(armor.circle);
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

	weapon = WT_Cannon;	//啊，我爱旋转机枪
	engine = ET_AFV;	//啊，我爱战锤坦克
}




//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "装逼程序已启动";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "啦啦啦小凯";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return 205;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 255;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -30;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 7;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return -223;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 61;
}









//-----------------------------------------------------
//3.用不用随你的触发函数
//-----------------------------------------------------


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//lastPlace[0] = info.robotInformation[myID].circle;
	//lastPlace[1] = info.robotInformation[1-myID].circle;
	queue_lastFivePoint.push(info.robotInformation[1-myID].circle);
}

void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗结束时被调用，可能可以用来析构你动态分配的内存空间（如果你用了的话）
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//system("pause");
	for(int i = 0; i < 5; i++)
	{
		queue_lastFivePoint.pop();
	}
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
//gogogo，去我想去的地方
int RobotAI::runAndrun(Circle me,Circle armor, Circle obstacle[], int num_obs){
	double dx = abs(me.x - armor.x);
	double dy = abs(me.y - armor.y);
	double dist, angle;
	for(int i = 0; i < num_obs; i++)
	{
		dist = Distance(me.x, me.y, obstacle[i].x, obstacle[i].y);
		if(dist == 120)
		{
			angle = atan2(me.y - obstacle[i].y, me.x - obstacle[i].y) * 180 / PI;
			if(angle>=0 && angle<90)
				return 4;
			else if(angle<0 && angle>=-90)
				return 3;
			else 
				return 1;
		}
	}

	if(dx < dy){
		if(me.y < armor.y)
			return 4;
		else
			return 3;
	}else{
		if(me.x < armor.x)
			return 2;
		else
			return 1;
	}
}
//战锤坦克行动！注意返回的是eturn
int RobotAI::runAndrunAFV(Circle me,Circle armor,double engine_rotation)
{
	double an = atan2(armor.y - me.y, armor.x - me.x) * 180 / PI;	
	return Rotate(an,engine_rotation);
	
}
//躲避障碍物
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
//如果有子弹靠近我
bool RobotAI::BulletShotMe(RobotAI_BulletInformation bu, RobotAI_RobotInformation me)
{
	double dist = Distance(me.circle.x, me.circle.y, bu.circle.x, bu.circle.y);//我和子弹的距离
	if(dist >= 800 && bu.type != BT_RPGBall && bu.type != BT_ApolloBall)//太远就不检测了，针对火箭弹和阿波罗要优化
		return false;
	double v1 = Distance(bu.vx,bu.vy,0,0), v2 = Distance(me.vx,me.vy,0,0);//v1是子弹速度，v2是我的速度
	double me_angle = atan2(me.vy, me.vx) * 180 / PI, //我的角度
		bu_me_angle = atan2(me.circle.y - bu.circle.y, me.circle.x - bu.circle.x) * 180 / PI,//我和子弹的角度
		bu_angle = atan2(bu.vy, bu.vx) * 180 / PI;//子弹角度
	double B_angle = abs(bu_angle - bu_me_angle),
		C_angle = abs(me_angle - bu_me_angle),
		A_angle = 360 - B_angle - C_angle;
	AngleAdjust(B_angle);
	AngleAdjust(C_angle);
	C_angle = 180 - C_angle;
	double a = dist, b = a * sin(B_angle) / sin(A_angle), c = a * sin(C_angle) / sin(A_angle);
	double t = c / v1;//t是时间
	if( b - me.circle.r / sin(A_angle) <= v2 * t || v2 * t <= b + me.circle.r / sin(A_angle) )
		return true;
	return false;
}
//小蜘蛛躲子弹
int RobotAI::AvoidBullet(Circle bu, double vx, double vy, Circle myself)
{
	//针对蜘蛛坦克V3，躲避子弹用
	if(vy > vx)
	{
		if(bu.x < myself.x)
			return 2;
		else
			return 1;
	}else
	{
		if(bu.y < myself.y)
			return 4;
		else
			return 3;
	}
	
}
//战锤坦克躲子弹
int RobotAI::AvoidBulletAFV(RobotAI_BulletInformation bu, RobotAI_RobotInformation me)
{
	double bu_angle = atan2(bu.vy, bu.vx) * 180 / PI;
	//double angle = atan2(me.circle.y - bu.circle.y, me.circle.x - bu.circle.x) *180/PI;
	if(me.circle.x >341 && me.circle.x < 1025)
	{
		//上半部分就往下躲，下半往上
		if(me.circle.y <= 340)
			bu_angle = 90;
		else
			bu_angle = -90;
	}else{
		//左边往右，右边往左
		if(me.circle.x <= 341)
			bu_angle = 0;
		else
			bu_angle = 180;
	}
	
	return Rotate(bu_angle, me.engineRotation);	
}
//战锤坦克躲加农炮和阿波罗电磁炮和火箭
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
//遇到墙
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
//确定旋转角方向,旋转机枪Mashinegun射速11,返回应该旋转的角度
double RobotAI::howToRotate(Circle me, Circle armor,double weapon_rotation, double vx, double vy)
{
	double angle = atan2(armor.y - me.y, armor.x - me.x) * 180 / PI;//我和敌人之间的角度
	double v1Angle;//敌人速度方向
	double offset = 0;//这是偏移角
	double dist_factor = 0.420;//修正因子,根据我的想法和距离来调整
	double armor_move = Distance(queue_lastFivePoint.front().x, queue_lastFivePoint.front().y,
		queue_lastFivePoint.back().x, queue_lastFivePoint.back().y);//敌人前5帧运动的长度
	if( armor_move > 6)//当敌人运动的时候计算偏移角
	{
		double v1 = Distance(vx, vy, 0, 0);//计算armorSpeed速度也可以套用distance函数
		double v2 = 11.0;//加农炮射速11
		double d = Distance(me.x,me.y,armor.x,armor.y);//我和敌人之间的距离
		double r = d * v1 / sqrt( v2 * v2 - v1 * v1); // 辅助圆的半径
		v1Angle = atan2(vy, vx) * 180 / PI;
		double ABC_angle = 180 - v1Angle + angle;//辅助角CBD
		double d_AC = sqrt(d*d + r*r - 2*d*r*cos(ABC_angle / 180 * PI));//三角形的AC边
		if(d < 300)
			dist_factor *= d / 300;
		if(d > 500)
			dist_factor *= 1.2;
		offset = acos((d - r*cos(ABC_angle / 180 * PI)) / d_AC) * 180 / PI * dist_factor;		
		//加上偏移角
		if(angle > 0 && angle < 180){
			if(angle - 180 < v1Angle && v1Angle < angle)
				angle -= offset;
			else{
				angle += offset;
				angle = (angle > 180)?(360-angle):angle;
			}
		}else if(angle < 0 && angle > -180){
			if(angle < v1Angle && v1Angle < angle + 180)
				angle += offset;
			else{
				angle -= offset;
				angle = (angle < -180) ? (360+angle) : angle;
			}
		}else if(angle == 0){
			if(v1Angle > 0)
				angle += offset;
			else
				angle -= offset;
		}else{
			if(v1Angle < 0)
				angle += offset;
			else
				angle = 180 - offset;
		}
	}
	return angle;
}

//何时开炮？有障碍物的时候要节约子弹哦
int RobotAI::doIFire(Circle me, RobotAI_RobotInformation armor,Circle obstacle[],int num_obs,double now_angle, double fire_angle)
{
	double min_dist = 850;
	if( abs(now_angle - fire_angle) > 10 )//角度太偏就不射
		return 0;
	
	double A,C,dis;
	Circle obs;
	double distWithAr = Distance(me.x, me.y, armor.circle.x, armor.circle.y), //与敌人的距离
		distWithObs1 = Distance(me.x, me.y, obstacle[0].x, obstacle[0].y),
		distWithObs2 = Distance(me.x, me.y, obstacle[1].x, obstacle[1].y),distWithObs;
	if(distWithAr > min_dist)//太远就不射
		return 0;
	if(distWithObs1 < distWithObs2)
	{
		obs = obstacle[0];
		distWithObs = distWithObs1;
	}
	else
	{
		obs = obstacle[1];
		distWithObs = distWithObs2;
	}	
	A = tan(now_angle / 180 * PI);
	C = me.y - A * me.x;
	dis = abs(A * obs.x - obs.y + C) / sqrt( A * A + 1 );//直线到圆心的距离	
	
	if( dis <= obs.r && distWithAr > distWithObs)//判断子弹是否被挡住		
		return 0;
	
	return 1;
}
//选则一个可以去的军火库
Circle RobotAI::whichArsenal(RobotAI_ArsenalInformation ar1, RobotAI_ArsenalInformation ar2, Circle me)
{
	if(ar1.respawning_time == ar2.respawning_time)//剩余时间相等,包括0
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
	Circle c;
	return c;
}
//距离函数
double RobotAI::Distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
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
