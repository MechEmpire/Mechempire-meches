#include "RobotAI.h"

RobotAI::RobotAI()
{
	me = RobotAI_RobotInformation();
	enemy = RobotAI_RobotInformation();
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
	static int seed = 0, counter = 19;
	counter++;

	me = info.robotInformation[myID];
	enemy = info.robotInformation[1 - myID];
	double enemy_dis = distance(me.circle, enemy.circle);
	Circle dest = enemy.circle;
	double dest_theta = 0;
	double dest_dtheta = 0;
	Circle aim = enemy.circle;
	double aim_theta = 0;
	double aim_dtheta = 0;
	bool fire = true;
	bool ars = false;				//Arsenal
	int cd = 0;					//Collison Detected

	if (block(me.circle,info.obstacle[0],me.weaponRotation) && distance(me.circle,info.obstacle[0]) < enemy_dis)
		fire = false;
	if (block(me.circle, info.obstacle[1], me.weaponRotation) && distance(me.circle, info.obstacle[1]) < enemy_dis)
		fire = false;

	if (info.arsenal[0].respawning_time == 0){
		ars = true;
		if (info.arsenal[1].respawning_time == 0){
			double d1 = distance(me.circle, info.arsenal[0].circle);
			double d2 = distance(enemy.circle, info.arsenal[0].circle);
			if (d1 < d2)
				dest = info.arsenal[0].circle;
			else
				dest = info.arsenal[1].circle;
		}else
			dest = info.arsenal[0].circle;
	}
	else if (info.arsenal[1].respawning_time == 0){
		ars = true;
		dest = info.arsenal[1].circle;
	}
	if (distance(me.circle, info.obstacle[0]) < 75 + me.circle.r + 50)
		cd = block(me.circle, info.obstacle[0], me.engineRotation);
	if (distance(me.circle, info.obstacle[1]) < 75 + me.circle.r + 50)
		cd = block(me.circle, info.obstacle[1], me.engineRotation);
	if (enemy_dis < me.circle.r + enemy.circle.r + 100)
		cd = block(me.circle, enemy.circle, me.engineRotation);

	Circle preact = forecast(enemy.circle, enemy.vx, enemy.vy, (enemy_dis - 100 - enemy.circle.r) / 11);
	aim_theta = theta(me.circle, preact);
	aim_dtheta = dtheta(me.weaponRotation, aim_theta);
	
	if (me.remainingAmmo > 1 ){
		if (counter == 20){
			srand((time(0)) << seed);
			seed++;
			counter = 0;
		}
		switch (enemy.weaponTypeName)
		{
		case WT_Tesla:
		case WT_Prism:
		case WT_MissileLauncher:
			if (enemy.engineTypeName != ET_Shuttle){
				dest = enemy.circle;
				break;
			}
		case WT_ElectricSaw:
		case WT_Cannon:
		case WT_Machinegun:
			if (enemy_dis > 500)
				dest = { (enemy.circle.x + 3 * rand() % 1366) / 4, (enemy.circle.y + 2 * rand() % 680) / 3, enemy.circle.r };
			else
			{
				dest.x = enemy.circle.x + (me.circle.x - enemy.circle.x) * 500 / distance(me.circle, enemy.circle);
				dest.y = enemy.circle.y + (me.circle.y - enemy.circle.y) * 500 / distance(me.circle, enemy.circle);
			}
			break;
		default:
			if (distance(me.circle, enemy.circle) > 200)
				dest = { (enemy.circle.x + 3 * rand() % 1366) / 4, (enemy.circle.y + 2*rand() % 680) / 3, enemy.circle.r };
			else
			{
				dest.x = enemy.circle.x + (me.circle.x - enemy.circle.x) * 200 / enemy_dis;
				dest.y = enemy.circle.y + (me.circle.y - enemy.circle.y) * 200 / enemy_dis;
			}
			break;
		} 
	}
	else if (!ars){
		double d1 = distance(me.circle, info.obstacle[0]);
		double d2 = distance(me.circle, info.obstacle[1]);
		if (d1 < d2){
			double d3 = distance(enemy.circle, info.obstacle[0]);
			dest.x = info.obstacle[0].x - (enemy.circle.x - info.obstacle[0].x)*(75 + me.circle.r) / d3;
			dest.y = info.obstacle[0].y - (enemy.circle.y - info.obstacle[0].y)*(75 + me.circle.r) / d3;
		}
		else
		{
			double d3 = distance(enemy.circle, info.obstacle[1]);
			dest.x = info.obstacle[1].x - (enemy.circle.x - info.obstacle[1].x)*(75 + me.circle.r) / d3;
			dest.y = info.obstacle[1].y - (enemy.circle.y - info.obstacle[1].y)*(75 + me.circle.r) / d3;
		}
	}

	dest = adjustdest(dest);
	dest_theta = theta(me.circle, dest);

	for (int i = 0; i < info.num_bullet; i++){
		RobotAI_BulletInformation bullet = info.bulletInformation[i];
		if (bullet.launcherID == 1 - myID){
			int cd = block(bullet.circle, me.circle, bullet.rotation);
			if (cd != 0){
				dest_theta = dtheta(bullet.rotation, cd * 90);
				break;
			}
		}
	}

	dest_dtheta = dtheta(me.engineRotation, dest_theta);
	order = rotate(dest_dtheta, aim_dtheta, cd, fire);
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

	weapon = WT_Cannon;	//啊，我爱加农炮
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

RobotAI_Order RobotAI::rotate(double erotation, double wrotation, int cd, bool fire){
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
		
	if (cd != 0)
		order.eturn = cd;
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

	if (preact.x < 75)
		preact.x = 50;
	else if (preact.x > 1290)
		preact.x = 1300;
	if (preact.y < 75)
		preact.y = 50;
	else if (preact.y > 605)
		preact.y = 615;

	return preact;
}

int RobotAI::block(Circle start, Circle end, double rotation){
	double dis = distance(start, end);
	double theta1 = theta(start, end);
	double theta2 = asin(end.r / dis) * 180 / PI + 3;
	double dt = dtheta(rotation, theta1);
	if (dt > -theta2 && dt < theta2) {
		if (dt < 0)
			return -1;
		else
			return 1;
	}
	else
		return 0;
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