#include "RobotAI.h"

RobotAI::RobotAI()
{
	enemy_lastx = 0;
	enemy_lasty = 0;
	weapon_lastrotation = 0;
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

	order.run = 1;

	//我的机甲信息
	double robot_x = info.robotInformation[myID].circle.x;
	double robot_y = info.robotInformation[myID].circle.y;
	double robot_rotation = info.robotInformation[myID].engineRotation;
	double robot_r = info.robotInformation[myID].circle.r;
	double robot_vx = info.robotInformation[myID].vx;
	double robot_vy = info.robotInformation[myID].vy;
	double weapon_rotation = info.robotInformation[myID].weaponRotation;

	//敌方机甲信息
	double enemy_x = info.robotInformation[1 - myID].circle.x;
	double enemy_y = info.robotInformation[1 - myID].circle.y;
	double enemy_r = info.robotInformation[1 - myID].circle.r;
	weapontypename enemy_weapon = info.robotInformation[1 - myID].weaponTypeName;
	double enemy_theta;
	double distance = sqrt(pow((enemy_y - robot_y), 2) + pow((enemy_x - robot_x), 2)) - 100 - enemy_r;
	if (enemy_lastx == 0){
		enemy_lastx = enemy_x;
		enemy_lasty = enemy_y;
	}
	double enemy_nextx = enemy_x + (enemy_x - enemy_lastx)*(distance / 14);
	double enemy_nexty = enemy_y + (enemy_y - enemy_lasty)*(distance / 14);
	if (enemy_nextx < 50)
		enemy_nextx = 50;
	else if (enemy_nextx > 1300)
		enemy_nextx = 1300;
	if (enemy_nexty < 50)
		enemy_nexty = 50;
	else if (enemy_nexty > 630)
		enemy_nexty = 630;
	enemy_lastx = enemy_x;
	enemy_lasty = enemy_y;

	//我的机甲与障碍物夹角，用于确定开火方向
	double obstacle1_distance = sqrt(pow(300 - robot_x, 2) + pow(250 - robot_y, 2));
	double obstacle2_distance = sqrt(pow(1066 - robot_x, 2) + pow(430 - robot_y, 2));
	double obstacle1_theta1 = atan2(250 - robot_y, 300 - robot_x) * 180 / PI;
	double obstacle2_theta1 = atan2(430 - robot_y, 1066 - robot_x) * 180 / PI;
	double obstacle1_theta2 = asin(75 / obstacle1_distance) * 180 / PI;
	double obstacle2_theta2 = asin(75 / obstacle2_distance) * 180 / PI;
	double obstacle1_dtheta1 = obstacle1_theta1 - obstacle1_theta2 - 3;
	if (obstacle1_dtheta1 < -180)
		obstacle1_dtheta1 += 360;
	double obstacle1_dtheta2 = obstacle1_theta1 + obstacle1_theta2 + 3;
	if (obstacle1_dtheta2 > 180)
		obstacle1_dtheta2 -= 360;
	double obstacle2_dtheta1 = obstacle2_theta1 - obstacle2_theta2 - 3;
	if (obstacle2_dtheta1 < -180)
		obstacle2_dtheta1 += 360;
	double obstacle2_dtheta2 = obstacle2_theta1 + obstacle2_theta2 + 3;
	if (obstacle2_dtheta2 > 180)
		obstacle2_dtheta2 -= 360;

	bool doFire = true;
	if (weapon_rotation > obstacle1_dtheta1 && weapon_rotation < obstacle1_dtheta2 && distance +100 +enemy_r > obstacle1_distance)
		doFire = false;
	if (weapon_rotation > obstacle2_dtheta1 && weapon_rotation < obstacle2_dtheta2 && distance +100 +enemy_r > obstacle2_distance)
		doFire = false;


	//离我最近的障碍物信息，用于绕其旋转
	double obstacle_distance;
	double obstacle_theta;
	double obstacle_dtheta;
	if (robot_x < 683){
		obstacle_distance = obstacle1_distance;
		if (enemy_weapon == WT_MissileLauncher)
			obstacle_theta = atan2(enemy_y - robot_y, enemy_x - robot_x) * 180 / PI;
		else
			obstacle_theta = obstacle1_theta1;
	}
	else{
		obstacle_distance = obstacle2_distance;
		if (enemy_weapon == WT_MissileLauncher)
			obstacle_theta = atan2(enemy_y - robot_y, enemy_x - robot_x) * 180 / PI;
		else
			obstacle_theta = obstacle2_theta1;
	}
	obstacle_dtheta = robot_rotation - obstacle_theta;
	if (obstacle_dtheta > 180)
		obstacle_dtheta -= 360;
	else if (obstacle_dtheta < -180)
		obstacle_dtheta += 360;

	//军火库信息
	double arsenal_distance;
	double arsenal_theta;
	double arsenal_dtheta;
	bool hasArsenal = false;

	double d0 = sqrt(pow(50 - robot_x, 2) + pow(630 - robot_y, 2));
	double d1 = sqrt(pow(1316 - robot_x, 2) + pow(50 - robot_y, 2));
	double d3 = sqrt(pow(50 - enemy_x, 2) + pow(630 - enemy_y, 2));
	double d4 = sqrt(pow(1316 - enemy_x, 2) + pow(50 - enemy_y, 2));
	if (info.arsenal[0].respawning_time == 0){
		hasArsenal = true;
		if (info.arsenal[1].respawning_time == 0){
			if (d0 < d1){
				if (d0 < d3){
					arsenal_distance = d0;
					arsenal_theta = atan2(630 - robot_y, 50 - robot_x) * 180 / PI;
				}
				else{
					arsenal_distance = d1;
					arsenal_theta = atan2(50 - robot_y, 1316 - robot_x) * 180 / PI;
				}
			}
			else{
				if (d1 < d4){
					arsenal_distance = d1;
					arsenal_theta = atan2(50 - robot_y, 1316 - robot_x) * 180 / PI;
				}
				else{
					arsenal_distance = d0;
					arsenal_theta = atan2(630 - robot_y, 50 - robot_x) * 180 / PI;
				}
			}
		}
		else
		{
			arsenal_distance = d0;
			arsenal_theta = atan2(630 - robot_y, 50 - robot_x) * 180 / PI;
		}
	}
	else
	{
		if (info.arsenal[1].respawning_time == 0){
			hasArsenal = true;
			arsenal_distance = d1;
			arsenal_theta = atan2(50 - robot_y, 1316 - robot_x) * 180 / PI;
		}
	}
	arsenal_dtheta = robot_rotation - arsenal_theta;
	if (arsenal_dtheta > 180)
		arsenal_dtheta -= 360;
	else if (arsenal_dtheta < -180)
		arsenal_dtheta += 360;

	//我的机甲移动命令
	if (info.robotInformation[myID].remainingAmmo > 1 || !hasArsenal){
		if (obstacle_distance > robot_r + 80){
			if (obstacle_dtheta >= -180 && obstacle_dtheta <= -10){
				order.eturn = 1;
			}
			else if (obstacle_dtheta >= 10 && obstacle_dtheta <= 180){
				order.eturn = -1;
			}
		}
		else order.eturn = -1;
	}
	else if (arsenal_dtheta >= -180 && arsenal_dtheta <= -10){
		order.eturn = 1;
	}
	else if (arsenal_dtheta >= 10 && arsenal_dtheta <= 180){
		order.eturn = -1;
	}

	//我的机甲攻击命令
	enemy_theta = atan2(enemy_nexty - robot_y, enemy_nextx - robot_x) * 180 / PI;
	if (weapon_lastrotation == 0)
		weapon_lastrotation = weapon_rotation;
	double dtheta = weapon_rotation - enemy_theta;
	double correct_dtheta = weapon_lastrotation - enemy_theta;
	if (correct_dtheta > 180)
		correct_dtheta -= 360;
	if (correct_dtheta < -180)
		correct_dtheta += 360;

	if (dtheta > 180)
		dtheta -= 360;
	if (dtheta < -180)
		dtheta += 360;

	if (dtheta >= -180 && dtheta <= -3){
		order.wturn = 1;
		if (correct_dtheta > dtheta && doFire)
			order.fire = 1;
	}
	else if (dtheta >= 3 && dtheta <= 180){
		order.wturn = -1;
		if (correct_dtheta < dtheta && doFire)
			order.fire = 1;
	}
	else if (doFire)
		order.fire = 1;
}



void RobotAI::ChooseArmor(weapontypename& weapon, enginetypename& engine, bool a)
{
	//挑选装备函数
	//功能：在战斗开始时为你的机甲选择合适的武器炮塔和引擎载具
	//参数：weapon	...	代表你选择的武器，在函数体中给它赋值
	//		engine	...	代表你选择的引擎，在函数体中给它赋值
	//tip:	括号里的参数是枚举类型 weapontypename 或 enginetypename
	//		开发文档中有详细说明，你也可以在RobotAIstruct.h中直接找到它们的代码
	//tip:	最后一个bool是没用的。。那是一个退化的器官

	weapon = WT_Apollo;	//啊，我爱加农炮
	engine = ET_AFV;	//啊，我爱小蜘蛛
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "强袭坦克";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "米米尔隆";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return 120;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return -10;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -190;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 120;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return -10;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return -190;
}









//-----------------------------------------------------
//3.用不用随你的触发函数
//-----------------------------------------------------


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info, int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标

}

void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info, int myID)
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


void RobotAI::onHit(int launcherID, bullettypename btn)
{
	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
}



//TODO:这里可以实现你自己的函数