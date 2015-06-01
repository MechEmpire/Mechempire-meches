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
	me = info.robotInformation[myID];
	enemy = info.robotInformation[1 - myID];
	arsenal[0] = info.arsenal[0];
	arsenal[1] = info.arsenal[1];
	obstacle[0] = info.obstacle[0];
	obstacle[1] = info.obstacle[1];

	double enemy_dis = distance(me.circle, enemy.circle);
	double dest_theta = 0;
	double dest_dtheta = 0;
	Circle aim = enemy.circle;
	double aim_theta = 0;
	double aim_dtheta = 0;
	bool fire = true;
	bool ars = false;
	bool slow = false;

	if (arsenal[0].respawning_time < 10 || arsenal[1].respawning_time < 10)
		ars = true;

	if (me.remainingAmmo > 1){
		if (enemy.remainingAmmo > 1)
			dest_theta = charge();
		else if (ars)
			dest_theta = supply();
		else
			dest_theta = theta(me.circle, enemy.circle);
	}
	else if (ars)
		dest_theta = supply();
	else
		dest_theta = defend();
	
	
	if (me.circle.x < 75 || me.circle.x>1366 - 75 || me.circle.y<75 || me.circle.y>680 - 75);
	else
	{
		for (int i = 0; i < info.num_bullet; i++){
			RobotAI_BulletInformation bullet = info.bulletInformation[i];
			if (bullet.launcherID == 1 - myID){
				Circle offsetme = { me.circle.x + 75 * cos(me.engineRotation*PI / 180), me.circle.y + 75 * sin(me.engineRotation*PI / 180), 75 };
				if (block(bullet.circle, offsetme, bullet.rotation)){
					double bt = theta(bullet.circle, me.circle);
					double dbt = dtheta(bullet.rotation, bt);
					if (dbt > 0)
						dest_theta = dtheta(bullet.rotation, 90);
					else
						dest_theta = dtheta(bullet.rotation, -90);
					break;
				}
			}
		}
	}
	if (dtheta(me.engineRotation, dest_theta) < -90 && dtheta(me.engineRotation, dest_theta) > 90)
		slow = true;
	
	if (block(me.circle, info.obstacle[0], me.weaponRotation) && distance(me.circle, info.obstacle[0]) < enemy_dis)
		fire = false;
	if (block(me.circle, info.obstacle[1], me.weaponRotation) && distance(me.circle, info.obstacle[1]) < enemy_dis)
		fire = false;
	if (enemy.weaponTypeName == WT_RPG && enemy.remainingAmmo > 1 && enemy_dis > 500)
		fire = false;

	Circle preact = forecast((enemy_dis - 100 - enemy.circle.r) / 11);
	aim_theta = theta(me.circle, preact);
	aim_dtheta = dtheta(me.weaponRotation, aim_theta);

	dest_theta = adjustdest(dest_theta);
	dest_dtheta = dtheta(me.engineRotation, dest_theta);
	order = rotate(dest_dtheta, aim_dtheta, slow, fire);
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
	return 195;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 100;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -255;
}

//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 195;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 100;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return -255;
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

double RobotAI::charge(){
	Circle dest = { 683, 340, 75 };
	double dt = theta(me.circle, dest);
	double dis = distance(me.circle, enemy.circle);

	switch (enemy.weaponTypeName)
	{
	case WT_Tesla:
	case WT_Prism:
	case WT_MissileLauncher:
		if (enemy.engineTypeName != ET_Shuttle)
			dt = theta(me.circle, enemy.circle);
		break;
	case WT_RPG:
		if (enemy.circle.x < 683){
			dest.x += obstacle[0].x / 2;
			dest.y += obstacle[0].y / 2;
			if (enemy.circle.x >683 || enemy.circle.y > 340)
				dest = enemy.circle;
		}
		else{
			dest.x = obstacle[1].x / 2;
			dest.y = obstacle[1].y / 2;
			if (enemy.circle.x <683 || enemy.circle.y < 340)
				dest = enemy.circle;
		}
		dt = theta(me.circle, dest);
		break;
	case WT_ElectricSaw:
	case WT_Cannon:
		if (dis < 500)
			dt = theta(enemy.circle, me.circle);
		break;
	case WT_Machinegun:
		dt = defend();
		break;
	default:
		dt = theta(me.circle, enemy.circle);
		break;
	}
	if (enemy.weaponTypeName != WT_Machinegun){
		if (me.circle.y < 340)
			dt = dtheta(dt, -dis * 45 / 1525);
		else
			dt = dtheta(dt, dis * 45 / 1525);
		dt = adjustobstacle(dt);
	}

	return dt;
}

double RobotAI::supply(){
	double dt = 0;
	double dis = 0;

	if (arsenal[0].respawning_time < 10){
		if (arsenal[1].respawning_time < 10){
			double d1 = distance(me.circle, arsenal[0].circle);
			double d2 = distance(enemy.circle, arsenal[0].circle);
			if (d1 < d2 || me.remainingAmmo > enemy.remainingAmmo){
				dt = theta(me.circle, arsenal[0].circle);
				dis = distance(me.circle, arsenal[0].circle);
			}
			else{
				dt = theta(me.circle, arsenal[1].circle);
				dis = distance(me.circle, arsenal[1].circle);
			}
		}
		else{
			dt = theta(me.circle, arsenal[0].circle);
			dis = distance(me.circle, arsenal[0].circle);
		}
	}
	else{
		dt = theta(me.circle, arsenal[1].circle);
		dis = distance(me.circle, arsenal[1].circle);
	}
	if (me.circle.y < 340)
		dt = dtheta(dt, -dis * 45 / 1525);
	else
		dt = dtheta(dt, dis * 45 / 1525);
	return dt;
}

double RobotAI::defend(){
	Circle dest;
	double dt = 0;

	double d1 = distance(me.circle, obstacle[0]);
	double d2 = distance(me.circle, obstacle[1]);
	if (d1 < d2){
		if (distance(me.circle, enemy.circle) > 300){
			dest = { obstacle[0].x / 2, obstacle[0].y / 2, obstacle[0].r };
			double ot = theta(me.circle, dest);
			dt = dtheta(ot, 80);
		}
		else{
			double evt = atan2(enemy.vy, enemy.vx) * 180 / PI;
			double eot = theta(enemy.circle, obstacle[0]);
			double edot = dtheta(evt, eot);
			if (edot > 0)
				dt = dtheta(eot, 90);
			else
				dt = dtheta(eot, -90);
		}
	}
	else
	{
		if (distance(me.circle, enemy.circle) > 300){
			dest = { obstacle[1].x / 2 + 683, obstacle[1].y / 2 + 340, obstacle[1].r };
			double ot = theta(me.circle, dest);
			dt = dtheta(ot, 80);
		}
		else{
			double evt = atan2(enemy.vy, enemy.vx) * 180 / PI;
			double eot = theta(enemy.circle, obstacle[1]);
			double edot = dtheta(evt, eot);
			if (edot > 0)
				dt = dtheta(eot, 90);
			else
				dt = dtheta(eot, -90);
		}
	}

	return dt;
}

RobotAI_Order RobotAI::rotate(double erotation, double wrotation, bool slow, bool fire){
	RobotAI_Order order;
	double limit_rotation = 3;
	order.run = 1;
	if (slow && distance(0, 0, me.vx, me.vy) > 3)
		order.run = 0;

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

Circle RobotAI::forecast(double time){
	double correct_x = 0;
	double correct_y = 0;
	Circle preact = enemy.circle;
	if (enemy_lastx == 0){
		enemy_lastx = enemy.circle.x;
		enemy_lasty = enemy.circle.y;
	}
	else{
		correct_x = enemy.circle.x - enemy_lastx;
		correct_y = enemy.circle.y - enemy_lasty;
		enemy_lastx = enemy.circle.x;
		enemy_lasty = enemy.circle.y;
	}
	if (enemy.vx * correct_x > 0 || enemy.vy * correct_y > 0){
		preact.x = enemy.circle.x + enemy.vx * time;
		preact.y = enemy.circle.y + enemy.vy * time;
	}

	if (preact.x < enemy.circle.r)
		preact.x = enemy.circle.r;
	else if (preact.x > 1366 - enemy.circle.r)
		preact.x = 1366 - enemy.circle.r;
	if (preact.y < enemy.circle.r)
		preact.y = enemy.circle.r;
	else if (preact.y > 680 - enemy.circle.r)
		preact.y = 680 - enemy.circle.r;

	return preact;
}

bool RobotAI::block(Circle start, Circle end, double rotation){
	double dis = distance(start, end);
	double theta1 = theta(start, end);
	double theta2 = asin(end.r / dis) * 180 / PI + 3;
	double dt = dtheta(rotation, theta1);
	if (dt > -theta2 && dt < theta2)
		return true;
	else
		return false;
}
double RobotAI::adjustdest(double dir){
	double x = me.circle.x;
	double y = me.circle.y;
	double r = me.circle.r;
	double vt = atan2(me.vy, me.vx) * 180 / PI;
	double newdir = dir;

	if (x < r){
		if (y < r){
			if (vt < -135 && vt > -180)
				newdir = 90;
			else if (vt > -135 && vt < -90)
				newdir = 0;
		}
		else if (y > 680 - r){
			if (vt < 135 && vt > 90)
				newdir = 0;
			else if (vt > 135 && vt < 180)
				newdir = -90;
		}
		else if (vt > 0)
			newdir = 90;
		else
			newdir = -90;
	}
	else if (x > 1366 - r){
		if (y < r){
			if (vt < -45 && vt > -90)
				newdir = -180;
			else if (vt > -45 && vt < 0)
				newdir = 90;
		}
		else if (y > 680 - r){
			if (vt < 45 && vt > 0)
				newdir = -90;
			else if (vt > 45 && vt < 90)
				newdir = 180;
		}
		else if (vt > 0)
			newdir = 90;
		else
			newdir = -90;
	}
	else if (y < r || y > 680 - r){
		if (vt > -90 && vt < 90)
			newdir = 0;
		else
			newdir = 180;
	}

	return newdir;
}
double RobotAI::adjustobstacle(double dir){
	double x = me.circle.x;
	double y = me.circle.y;
	double r = me.circle.r;
	double offsetx = x + r * cos(dir*PI / 180);
	double offsety = y + r * sin(dir*PI / 180);
	double vt = atan2(me.vy, me.vx) * 180 / PI;
	double newdir = dir;

	if (distance(offsetx, offsety, obstacle[0].x, obstacle[0].y) < r + obstacle[0].r){
		double ot = theta(offsetx, offsety, obstacle[0].x, obstacle[0].y);
		double dot = dtheta(vt, ot);
		if (dot > 0)
			newdir = dtheta(ot, -90);
		else
			newdir = dtheta(ot, 90);
	}
	else if (distance(offsetx, offsety, obstacle[1].x, obstacle[1].y) < r + obstacle[1].r){
		double ot = theta(offsetx, offsety, obstacle[1].x, obstacle[1].y);
		double dot = dtheta(vt, ot);
		if (dot > 0)
			newdir = dtheta(ot, -90);
		else
			newdir = dtheta(ot, 90);
	}
	if (distance(offsetx, offsety, enemy.circle.x, enemy.circle.y) < r + enemy.circle.r){
		double ot = theta(offsetx, offsety, enemy.circle.x, enemy.circle.y);
		double dot = dtheta(vt, ot);
		if (dot > 0)
			newdir = dtheta(ot, -90);
		else
			newdir = dtheta(ot, 90);
	}
	return newdir;
}