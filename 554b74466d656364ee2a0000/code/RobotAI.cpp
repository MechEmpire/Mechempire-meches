#include "RobotAI.h"

RobotAI::RobotAI()
{
	enemy_lastx = 0;
	enemy_lasty = 0;
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

	Circle me = info.robotInformation[myID].circle;
	Circle enemy = info.robotInformation[1 - myID].circle;
	double me_rotation = info.robotInformation[myID].engineRotation;
	double weapon_rotation = info.robotInformation[myID].weaponRotation;
	weapontypename enemy_weapon = info.robotInformation[1 - myID].weaponTypeName;
	double enemy_vx = info.robotInformation[1 - myID].vx;
	double enemy_vy = info.robotInformation[1 - myID].vy;
	Circle dest = enemy;
	double dest_theta = 0;

	bool fire = true;
	if (block(me, info.obstacle[0], weapon_rotation) && distance(me, info.obstacle[0]) < distance(me, enemy))
		fire = false;
	if (block(me, info.obstacle[1], weapon_rotation) && distance(me, info.obstacle[1]) < distance(me, enemy))
		fire = false;

	bool hasArsenal = false;
	if (info.arsenal[0].respawning_time == 0){
		hasArsenal = true;
		if (info.arsenal[1].respawning_time == 0){
			double d1 = distance(me, info.arsenal[0].circle);
			double d2 = distance(enemy, info.arsenal[0].circle);
			if (d1 < d2)
				dest = info.arsenal[0].circle;
			else
				dest = info.arsenal[1].circle;
		}else
			dest = info.arsenal[0].circle;
	}
	else if (info.arsenal[1].respawning_time == 0){
		hasArsenal = true;
		dest = info.arsenal[1].circle;
	}

	double enemy_distance = distance(me, enemy) - 100 - enemy.r;
	Circle preact = forecast(enemy, enemy_vx, enemy_vy, enemy_distance/10);
	double target_theta = theta(me, preact);
	double target_dtheta = dtheta(weapon_rotation, target_theta);
	
	if (info.robotInformation[myID].remainingAmmo > 1 ){
		switch (enemy_weapon)
		{
		case WT_Prism:
		case WT_Tesla:
		case WT_MissileLauncher:
			dest = enemy;
			break;
		default:
			if (distance(me, enemy) > 500){
				if (info.robotInformation[myID].cooling < 10 || info.robotInformation[1 - myID].remainingAmmo < 1)
					dest = preact;
				else
				{
					double d1 = distance(me, info.obstacle[0]);
					double d2 = distance(me, info.obstacle[1]);
					if (d1 < d2){
						double d3 = distance(enemy, info.obstacle[0]);
						dest.x = info.obstacle[0].x - (enemy.x - info.obstacle[0].x)*(75 + me.r) / d3;
						dest.y = info.obstacle[0].y - (enemy.y - info.obstacle[0].y)*(75 + me.r) / d3;
					}
					else
					{
						double d3 = distance(enemy, info.obstacle[1]);
						dest.x = info.obstacle[1].x - (enemy.x - info.obstacle[1].x)*(75 + me.r) / d3;
						dest.y = info.obstacle[1].y - (enemy.y - info.obstacle[1].y)*(75 + me.r) / d3;
					}
				}
			}
			else
			{
				dest.x = enemy.x + (me.x - enemy.x) * 500 / distance(me, enemy);
				dest.y = enemy.y + (me.y - enemy.y) * 500 / distance(me, enemy);
			}
			break;
		}
	}
	else if (!hasArsenal){
		double d1 = distance(me, info.obstacle[0]);
		double d2 = distance(me, info.obstacle[1]);
		if (d1 < d2){
			double d3 = distance(enemy, info.obstacle[0]);
			dest.x = info.obstacle[0].x - (enemy.x - info.obstacle[0].x)*(75 + me.r) / d3;
			dest.y = info.obstacle[0].y - (enemy.y - info.obstacle[0].y)*(75 + me.r) / d3;
		}
		else
		{
			double d3 = distance(enemy, info.obstacle[1]);
			dest.x = info.obstacle[1].x - (enemy.x - info.obstacle[1].x)*(75 + me.r) / d3;
			dest.y = info.obstacle[1].y - (enemy.y - info.obstacle[1].y)*(75 + me.r) / d3;
		}
	}

	dest = adjustdest(dest);
	dest_theta = theta(me, dest);
	double dest_dtheta = dtheta(me_rotation, dest_theta);
	bool check_collision = collision(me, enemy);

	order = rotate(dest_dtheta, target_dtheta, check_collision, fire);

	for (int i = 0; i < info.num_bullet; i++){
		RobotAI_BulletInformation bullet = info.bulletInformation[i];
		if (bullet.launcherID == 1 - myID){
			double vx = info.robotInformation[myID].vx;
			double vy = info.robotInformation[myID].vy;
			bool fire = true;
			if (block(bullet.circle, info.obstacle[0], bullet.rotation) && distance(bullet.circle, info.obstacle[0]) < distance(bullet.circle, me))
				fire = false;
			if (block(bullet.circle, info.obstacle[1], bullet.rotation) && distance(bullet.circle, info.obstacle[1]) < distance(bullet.circle, me))
				fire = false;
			double bullettheta = theta(me, bullet.circle);
			double bulletdtheta = dtheta(atan2(vy, vx), bullettheta);

			if (block(bullet.circle, me, bullet.rotation) && fire){
				if (bulletdtheta > 0)
					order.eturn = 1;
				else
					order.eturn = -1;
				break;
			}
		}
	}
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

	weapon = WT_Shotgun;	//啊，我爱加农炮
	engine = ET_GhostTank;	//啊，我爱小蜘蛛
}

//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "死神4000型";
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
	return 200;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 200;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -40;
}

//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return -75;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return -6;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return -10;
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

RobotAI_Order RobotAI::rotate(double erotation, double wrotation, bool check_collision, bool fire){
	RobotAI_Order order;
	double limit_rotation = 3;
	order.run = 1;

	if (erotation > limit_rotation)
		order.eturn = -1;
	else if (erotation < -limit_rotation)
		order.eturn = 1;
	
	
	if (wrotation > limit_rotation)
		order.wturn = -1;
	else if (wrotation < -limit_rotation)
		order.wturn = 1;
	else if (fire)
		order.fire = 1;
		
	if (check_collision)
		order.eturn = 1;
	return order;
}

double RobotAI::distance(double x1, double y1, double x2, double y2){
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
double RobotAI::distance(Circle c1, Circle c2){
	return distance(c1.x, c1.y, c2.x, c2.y);
}

double RobotAI::theta(double x1, double y1, double x2, double y2){
	return atan2(y2 - y1, x2 - x1) * 180 / PI;
}
double RobotAI::theta(Circle c1, Circle c2){
	return theta(c1.x, c1.y, c2.x, c2.y);
}

double RobotAI::dtheta(double theta1, double theta2){
	double dtheta = theta1 - theta2;
	if (dtheta > 180)
		dtheta -= 360;
	else if (dtheta < -180)
		dtheta += 360;
	return dtheta;
}

Circle RobotAI::forecast(Circle robot, double vx, double vy, double time){
	double correct_x = 0;
	double correct_y = 0;
	Circle preact = robot;
	if (enemy_lastx == 0){
		enemy_lastx = robot.x;
		enemy_lasty = robot.y;
	}
	else{
		correct_x = robot.x - enemy_lastx;
		correct_y = robot.y - enemy_lasty;
		enemy_lastx = robot.x;
		enemy_lasty = robot.y;
	}
	if (vx * correct_x > 0 || vy * correct_y > 0){
		preact.x = robot.x + vx * time;
		preact.y = robot.y + vy * time;
	}

	if (preact.x < 125)
		preact.x = 125;
	else if (preact.x > 1240)
		preact.x = 1240;
	if (preact.y < 125)
		preact.y = 125;
	else if (preact.y > 555)
		preact.y = 555;

	return preact;
}

bool RobotAI::block(Circle start, Circle end, double rotation){
	double dis = distance(start, end);
	double theta1 = theta(start, end);
	double theta2 = asin(end.r / dis) * 180 / PI;
	double dtheta1 = dtheta(theta1, theta2 + 3);
	double dtheta2 = dtheta(theta1, -theta2 - 3);

	if (dtheta2 - dtheta1 > 180)
		return rotation < dtheta1 || rotation > dtheta2;
	else if (dtheta1 - dtheta2 > 180)
		return rotation > dtheta1 || rotation < dtheta2;
	else
		return rotation > dtheta1 && rotation < dtheta2;
}

Circle RobotAI::adjustdest(Circle dest){
	if (dest.x < 50)
		dest.x = 50;
	else if (dest.x > 1316)
		dest.x = 1316;
	if (dest.y < 50)
		dest.y = 50;
	else if (dest.y > 630)
		dest.y = 630;

	return dest;
}

bool RobotAI::collision(Circle me, Circle enemy){
	const Circle obstacle1 = { 300, 250, 75 };
	const Circle obstacle2 = { 1066, 430, 75 };
	if (distance(me, obstacle1) < me.r + obstacle1.r + 10)
		return true;
	else if (distance(me, obstacle2) < me.r + obstacle2.r + 10)
		return true;
	else if (distance(me, enemy) < me.r + enemy.r + 5)
		return true;
	return false;
}