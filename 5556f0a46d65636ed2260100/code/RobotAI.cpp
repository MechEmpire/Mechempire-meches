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


void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	//order.fire = 1;
	//order.wturn = 1;
	/***********************************
		走位
	******************************/

	// 更新阈值
	if (info.robotInformation[myID].hp < 60 && info.robotInformation[enemyId].hp > 75
		&& info.robotInformation[enemyId].remainingAmmo > 2
		&& dist2Point(info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y,
		info.robotInformation[enemyId].circle.x, info.robotInformation[enemyId].circle.y) > 200)
		attackDistance = 120; // 超级猥琐
	else
		attackDistance = 250;

	// 更新最近的障碍物点
	if (dist2Point(info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y, 300, 250)
		< dist2Point(info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y, 1066, 430)) {
		myObj.x = 300;
		myObj.y = 250;
	} else {
		myObj.x = 1066;
		myObj.y = 430;
	}

	// 如果临近位置有军火库，则抢占
	//cout << "军火库：" << info.arsenal[0].respawning_time << endl;
	Point arsenalPoint; // 目标军火库坐标点
	arsenalPoint.x = arsenalPoint.y = 0;
	farArsenal.x = farArsenal.y = 0;
	if ( dist2Point(info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y,
		info.arsenal[0].circle.x, info.arsenal[0].circle.y) <
		dist2Point(info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y,
		info.arsenal[1].circle.x, info.arsenal[1].circle.y) ) {
			if (info.arsenal[0].respawning_time == 0) {
				arsenalPoint.x = info.arsenal[0].circle.x;
				arsenalPoint.y = info.arsenal[0].circle.y;
			}
			if (info.arsenal[1].respawning_time == 0) {
				farArsenal.x = info.arsenal[1].circle.x;
				farArsenal.y = info.arsenal[1].circle.y;
			}
	} else {
			if (info.arsenal[1].respawning_time == 0) {
				arsenalPoint.x = info.arsenal[1].circle.x;
				arsenalPoint.y = info.arsenal[1].circle.y;
			}
			if (info.arsenal[0].respawning_time == 0) {
				farArsenal.x = info.arsenal[0].circle.x;
				farArsenal.y = info.arsenal[0].circle.y;
			}
	}

	Point enemyPoint;
	enemyPoint.x = info.robotInformation[enemyId].circle.x;
	enemyPoint.y = info.robotInformation[enemyId].circle.y;

	if (hideForEver)
		hide(order,info,myID); // 猥琐
	else if (arsenalPoint.x > 0 &&
		dist2Point(info.robotInformation[enemyId].circle.x, info.robotInformation[enemyId].circle.y,
		arsenalPoint.x, arsenalPoint.y) <
		(dist2Point(info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y,
		arsenalPoint.x, arsenalPoint.y) + 100)) {
		//cout << "pointX, pointY = " << arsenalPoint.x << ", " << arsenalPoint.y << endl;
			// 如果离敌人很近，则直接攻击敌人
		if (dist2Point(info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y,
		info.robotInformation[enemyId].circle.x, info.robotInformation[enemyId].circle.y) < 170)
			moveToPoint(order, info, myID, enemyPoint);	
		else
			moveToPoint(order, info, myID, arsenalPoint);
	} else {
		if ( (double)info.robotInformation[enemyId].remainingAmmo /
			get_weapon_ammo(info.robotInformation[enemyId].weaponTypeName) > attackAlarm // 敌方弹药不小于设定阈值
			&& dist2Point(info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y,
			info.robotInformation[enemyId].circle.x, info.robotInformation[enemyId].circle.y) > attackDistance) { 
			
				hide(order,info,myID); // 猥琐
		} else if ( dist2Point(info.robotInformation[enemyId].circle.x, info.robotInformation[enemyId].circle.y,
			farArsenal.x, farArsenal.y) + 300 < dist2Point(info.robotInformation[myID].circle.x,
			info.robotInformation[myID].circle.y, farArsenal.x, farArsenal.y)){ // 如果敌人离弹药库比我离该弹药库近很多，继续猥琐
			
				hide(order,info,myID); // 猥琐
		} else {
			// 追敌人
			
			moveToPoint(order, info, myID, enemyPoint);
		}
	}
	/***********************************
		进入攻击范围，则攻击
	******************************/
	// 敌我之间的距离
	double distToEnemy = dist2Point(info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y,
		info.robotInformation[enemyId].circle.x, info.robotInformation[enemyId].circle.y);

	// 调整武器朝向
	double theta; // 我方机甲与敌方机甲中心的连线与x轴正向的夹角（单位：度）
	double x = info.robotInformation[enemyId].circle.x - info.robotInformation[myID].circle.x;
	double y = info.robotInformation[enemyId].circle.y - info.robotInformation[myID].circle.y;
	double sinTheta = y / distToEnemy;
	//double cosTheta = x / distToEnemy;
	theta = RadianToAngle( asin(sinTheta) );
	// 针对边界情况，对theta校准
	if ( x < 0 ) {
		if (theta > 0)
			theta = 180 - theta;
		else
			theta = -180 - theta;
	}
	//cout << "sinTheta = " << sinTheta;
	//cout << " Theta = " << RadianToAngle( asin(sinTheta) ) << endl;
	// 最佳旋转方向
	if (theta > 0) {
		if (0 < theta - info.robotInformation[myID].weaponRotation &&
			theta - info.robotInformation[myID].weaponRotation <= 180)
			order.wturn = 1;
		else
			order.wturn = -1;
	} else {
		if (-180 < theta - info.robotInformation[myID].weaponRotation &&
			theta - info.robotInformation[myID].weaponRotation <= 0)
			order.wturn = -1;
		else
			order.wturn = 1;
	}
	/*if ( info.robotInformation[myID].weaponRotation - theta > 180 )
		order.wturn = 1;
	else if ( info.robotInformation[myID].weaponRotation - theta < 180 )
		order.wturn = -1;*/

	// 准备进攻
	double fireScope; // 攻击的范围（按敌我机甲的距离计算）= 敌方机甲半径 + 我方武器半径
	fireScope = info.robotInformation[enemyId].circle.r + 96;// 100 = 95 + 1
	if (distToEnemy < fireScope) // 进入攻击范围
		order.fire = 1;
		
	//order.fire = 1;
	//cout << info.robotInformation[myID].weaponRotation  << endl;
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

	weapon = WT_ElectricSaw;	//电锯
	engine = ET_Quad;	//飞行器
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "景阳冈上打过虎";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "plough";
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
	enemyId = 1 - myID;
	if (myID == 0) {
		myObj.x = 300;
		myObj.y = 250;
		myObj.r = 75;
	} else {
		myObj.x = 1066;
		myObj.y = 430;
		myObj.r = 75;
	}
	attackAlarm = 0.25;
	if (info.robotInformation[enemyId].weaponTypeName == WT_MissileLauncher)
		attackAlarm = 0.35;

	attackDistance = 250;
	if (get_weapon_coolingTime(info.robotInformation[enemyId].weaponTypeName) > 50)
		attackDistance = 500;

	// 如果敌人是电锯，则一直猥琐……以后再修改
	hideForEver = false;
	if (info.robotInformation[enemyId].weaponTypeName == WT_ElectricSaw)
		hideForEver = true;
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
void RobotAI::hide(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	// （使用相似三角形）计算躲避的目标点
	Point aimPoint; // 目标点的坐标
	int x,y,x1,y1;
	double ratio; // 相似三角形的比率
	x1 = info.robotInformation[enemyId].circle.x - myObj.x;
	y1 = info.robotInformation[enemyId].circle.y - myObj.y;
	ratio = (double)(myObj.r + info.robotInformation[myID].circle.r)
		/ sqrt(x1*x1 + y1*y1);
	x = x1 * ratio;
	y = y1 * ratio;
	aimPoint.x = myObj.x - x;
	aimPoint.y = myObj.y - y;
	//--------------------------------------------
	// 让我的机甲按最短路径向目标点移动，并避开障碍物
	moveToPoint(order, info, myID, aimPoint);
}

//让我的机甲按最短路径向目标点移动，并避开障碍物
void RobotAI::moveToPoint(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,
						  int myID, Point aimPoint)
{
	double distNow; // 当前距离
	double tempDist;
	int move = 0; // 1,2,3,4代表左右上下
	//int canMove = 0; // 可以移动的方向
	int myX = info.robotInformation[myID].circle.x;
	int myY = info.robotInformation[myID].circle.y;
	if ( ( distNow = dist2Point(myX, myY,aimPoint.x,aimPoint.y) ) < 3)
		return; // 距离足够小了，不必移动

	// 为了避开障碍物，进行碰撞检测
	Circle tempCircle = info.robotInformation[myID].circle;
	
	// 假设向左移动一步
	tempCircle.x -= 1;
	if (!HitTestCircles(tempCircle, myObj)) {
		//canMove = 1;
		if ((tempDist = dist2Point(tempCircle.x, tempCircle.y, aimPoint.x, aimPoint.y)) < distNow ) {
			move = 1;
			distNow = tempDist;
		}
	}
	tempCircle.x += 1;

	// 假设向右移动一步
	tempCircle.x += 1;
	if (!HitTestCircles(tempCircle, myObj)) {
		//canMove = 2;
		if ((tempDist = dist2Point(tempCircle.x, tempCircle.y, aimPoint.x, aimPoint.y)) < distNow ) {
			move = 2;
			distNow = tempDist;
		}
	}
	tempCircle.x -= 1;

	// 假设向上移动一步
	tempCircle.y -= 1;
	if (!HitTestCircles(tempCircle, myObj)) {
		//canMove = 3;
		if ((tempDist = dist2Point(tempCircle.x, tempCircle.y, aimPoint.x, aimPoint.y)) < distNow ) {
			move = 3;
			distNow = tempDist;
		}
	}
	tempCircle.y += 1;

	// 假设向下移动一步
	tempCircle.y += 1;
	if (!HitTestCircles(tempCircle, myObj)) {
		//canMove = 4;
		if ((tempDist = dist2Point(tempCircle.x, tempCircle.y, aimPoint.x, aimPoint.y)) < distNow ) {
			move = 4;
			distNow = tempDist;
		}
	}
	tempCircle.y -= 1;
	
	if (move == 0)
		move = Random(1,4);
	order.run = move;
}
//计算两点间的距离
double RobotAI::dist2Point(int x1, int y1, int x2, int y2)
{
	return sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
}