#include "RobotAI.h"
//#include<fstream>
//using namespace std;
//ofstream fout("旋转测试.txt",ios::out|ios::app);
#define ARSENAL0_X 50
#define ARSENAL0_Y 630
#define ARSENAL1_X 1316
#define ARSENAL1_Y 50
#define OBSTACLE0_X 300
#define OBSTACLE0_Y 250
#define OBSTACLE1_X 1066
#define OBSTACLE1_Y 430

//#define 左 1
//#define 右 2
//#define 上 3
//#define 下 4
//状态
#define ATTACK	1
#define BACK		2
#define HIDE		3
#define ARSENAL0	4
#define ARSENAL1	5
#define OBSTACLE0	6
#define OBSTACLE1	7
#define WIND		8//对敌方跟踪弹
#define MIN_BULLET 3
int STATE = HIDE;
//bool attack_yet = false;
//帧数测试用
int flame = 0;
int span = 1000;
int attack_flame = 0;
int Max_attack_flame = 0;
int stay_flame = 0;
double gsafe_x, gsafe_y;
RobotAI_RobotInformation gme, genermy;
RobotAI_RobotInformation pre_gme, pre_genermy;

Beam To_enermy, To_target;
Beam To_Arsenal0, To_Arsenal1;
Beam To_Obstacle0, To_Obstacle1;

Circle Obstacle_Zone[2] = { { OBSTACLE0_X, OBSTACLE0_Y, 145 }, { OBSTACLE1_X, OBSTACLE1_Y, 145 } };
Point Key_Point[6] = { { 150, 0 }, { 75, 129.9 }, { -75, 129.9 },
					{ -150, 0 }, { -75, -129.9 }, { 75, -129.9 } };//以OBSTACLE物为原点的绕圈坐标点

//Point Att_Point[4] = {};//以OBSTACLE物为原点的攻击坐标点

RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}
void AdjustBeam(Beam&);
void AdjustWeapon(RobotAI_Order&, double);
bool AdjustEngine(RobotAI_Order& order, double r_engine, int x, int y);
bool GotoPoint(RobotAI_Order& order, int x, int y);
bool Isinzone(Box zone, Circle circle);
bool Isinzone(Circle zone, Circle circle);
bool Isarrive(int, int, Circle);

double Distance(Point a, Point b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
double Distance(int x1, int x2, int y1, int y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}
double Distance(Circle c1, Circle c2)
{
	return sqrt((c1.x - c2.x)*(c1.x - c2.x) + (c1.y - c2.y)*(c1.y - c2.y));
}

void AdjustBeam(Beam &b)
{
	if (b.x == b.ex)
	{
		if (b.ey > b.y)
			b.rotation = 90;
		else
			b.rotation = -90;
	}
	else if (b.y == b.ey)
	{
		if (b.ex > b.x)
			b.rotation = 0;
		else
			b.rotation = 180;
	}
	else
		b.rotation = RadianToAngle(atan2((double)(b.ey - b.y), (double)(b.ex - b.x)));

	//fout << "帧:" << flame << "  To_enermy:" << To_enermy.rotation << "  weaponRotation：" << gme.weaponRotation << endl;
}

void Aimenermy(RobotAI_Order& order, double r_weapon)
{
	/*double theta;
	if (genermy.circle.x == gme.circle.x && genermy.circle.y > gme.circle.y)
	theta = -90;
	else if (genermy.circle.x == gme.circle.x && genermy.circle.y < gme.circle.y)
	theta = 90;
	else
	{
	theta = RadianToAngle(atan2((double)(genermy.circle.y - gme.circle.y), (double)(genermy.circle.x - gme.circle.x)));
	AngleAdjust(theta);
	}
	if (r_weapon < theta)
	order.wturn = 10;
	else if (r_weapon > theta )
	order.wturn = -10;*/
	int theta;
	if (genermy.circle.x == gme.circle.x)
	{
		if (genermy.circle.y > gme.circle.y)
			theta = 90;
		else
			theta = -90;
	}
	else if (genermy.circle.y == gme.circle.y)
	{
		if (genermy.circle.x > gme.circle.x)
			theta = 0;
		else
			theta = 180;
	}
	else
	{
		theta = RadianToAngle(atan2((double)(genermy.circle.y - gme.circle.y), (double)(genermy.circle.x - gme.circle.x)));
		if (theta % 5 == 1 || theta % 5 ==2)
			theta = (theta / 5) * 5;
		else if (theta % 3 == 3 || theta % 3 == 4)
			theta = (theta / 5) * 5 + 5;
	}

	if (abs(theta - r_weapon) > 180)
	{
		if (r_weapon > theta)
		{
			order.wturn = 1;
			//fout << "逆旋转中，引擎：" << r_weapon << "  目标角:" << theta << "\n";
		}
		else
		{
			order.wturn = -1;
			//fout << "顺旋转中，引擎：" << r_weapon << "  目标角:" << theta << "\n";
		}
	}
	else
	{
		if (theta > r_weapon)
		{
			order.wturn = 1;
			//fout << "逆旋转中，引擎：" << r_weapon << "  目标角:" << theta << "\n";
		}
		else
		{
			order.wturn = -1;
			//fout << "顺旋转中，引擎：" << r_weapon << "  目标角:" << theta << "\n";
		}
	}
}
void Aim_(RobotAI_Order& order, double r_weapon,int x,int y)
{
	int theta;
	if (x == gme.circle.x)
	{
		if (y > gme.circle.y)
			theta = 90;
		else
			theta = -90;
	}
	else if (y == gme.circle.y)
	{
		if (x > gme.circle.x)
			theta = 0;
		else
			theta = 180;
	}
	else
	{
		theta = RadianToAngle(atan2((double)(y - gme.circle.y), (double)(x - gme.circle.x)));
		if (theta % 5 == 1 || theta % 5 == 2)
			theta = (theta / 5) * 5;
		else if (theta % 3 == 3 || theta % 3 == 4)
			theta = (theta / 5) * 5 + 5;
	}

	if (abs(theta - r_weapon) <= 5)
		return;

	if (abs(theta - r_weapon) > 180)
	{
		if (r_weapon > theta)
			order.wturn = 1;
		else
			order.wturn = -1;
	}
	else
	{
		if (theta > r_weapon)
			order.wturn = 1;
		else
			order.wturn = -1;
	}
}

//毁灭者专用
bool AdjustEngine(RobotAI_Order& order, double r_engine, int x, int y)
{ 
	int theta;
	double _engine = r_engine + 180;
	while (_engine > 180)
		_engine -= 360;


	if (x == gme.circle.x)
	{
		if (y > gme.circle.y)
			theta = 90;
		else
			theta = -90;
	}
	else if (y == gme.circle.y)
	{
		if (x > gme.circle.x)
			theta = 0;
		else
			theta = 180;
	}
	else
	{
		theta = RadianToAngle(atan2((double)(y - gme.circle.y), (double)(x - gme.circle.x)));
		if (theta % 3 == 1)
			theta = (theta / 3) * 3;
		else if (theta % 3 == 2)
			theta = (theta / 3) * 3 + 3;
	}
	if (abs(r_engine - theta) <= 3 || abs(_engine - theta) <= 3)
		return false;

	else{
		double ring = abs(r_engine - theta);
		while (ring > 180)
			ring -= 360;
		if (ring < 0)
			ring = -ring;
		double _ring = abs(_engine - theta);
		while (_ring > 180)
			_ring -= 360;
		if (_ring < 0)
			_ring = -_ring;

		if (ring > _ring)//逆向的夹角更小，接下来为后退
		{
			if (theta - _engine > 180)
				order.eturn = -1;
			else if (theta > _engine)
				order.eturn = 1;
			if (_engine - theta > 180)
				order.eturn = 1;
			else if (theta < _engine)
				order.eturn = -1;
		}
		else
		{
			if (theta - r_engine > 180)
				order.eturn = -1;
			else if (theta > r_engine)
				order.eturn = 1;
			if (r_engine - theta > 180)
				order.eturn = 1;
			else if (theta < r_engine)
				order.eturn = -1;
		}
		return false;
	}
}
bool GotoPoint(RobotAI_Order& order, int x, int y)
{
	//fout << "帧：" << flame << "  位置：" << gme.circle.x << "," << gme.circle.y << "  目标：" << x << "," << y << endl;
	bool arrive = false;
	
	if (Isarrive(x, y, gme.circle))
	{
		arrive = true;
		return true;
	}
	else
	{
		if (!AdjustEngine(order, gme.engineRotation, x, y))
		{
			if (gme.circle.x < x && abs(gme.engineRotation) < 87)
					order.run = 1;
			else if (gme.circle.x < x && abs(gme.engineRotation) > 93)
					order.run = -1;
			else if (gme.circle.x > x && abs(gme.engineRotation) < 87)
					order.run = -1;
			else if (gme.circle.x > x && abs(gme.engineRotation) > 93)
					order.run = 1;
			else
			{
				if (gme.engineRotation > 0)
				{
					if (gme.circle.y > y)
						order.run = -1;
					else
						order.run = 1;
				}
				else
				{
					if (gme.circle.y > y)
						order.run = 1;
					else
						order.run = -1;
				}
			}
		}
		return false;
	}
}
bool GotoZone(RobotAI_Order& order, Box zone)
{
	//fout << "帧：" << flame << "  位置：" << gme.circle.x << "," << gme.circle.y << "  目标：" << x << "," << y << endl;
	bool arrive = false;

	if (Isinzone(zone, gme.circle))
	{
		return true;
	}
	if (!AdjustEngine(order, gme.engineRotation, zone.x, zone.y))
	{
		if (arrive)
		{
			order.run = 0;
			//fout <<"目标："<<x<<","<<y<< "  当前" << "(" << gme.circle.x << "," << gme.circle.y << "  已到达"<<endl ;
		}
		else
		{
			order.run = 1;
			//fout << "目标：" << x << "," << y << "  当前" << "(" << gme.circle.x << "," << gme.circle.y << ") 前进中"<<endl;
		}
	}
	return arrive;
}

void Strategy1(const RobotAI_BattlefieldInformation& info, int myID)
{
	int self_hp = info.robotInformation[myID].hp;
	int ener_hp = info.robotInformation[1 - myID].hp;

	int self_Bullet = info.robotInformation[myID].remainingAmmo;
	int ener_Bullet = info.robotInformation[1 - myID].remainingAmmo;

	double distance = sqrt((info.robotInformation[myID].circle.x - info.robotInformation[1 - myID].circle.x)*(info.robotInformation[myID].circle.x - info.robotInformation[1 - myID].circle.x)
		+ (info.robotInformation[myID].circle.y - info.robotInformation[1 - myID].circle.y)*(info.robotInformation[myID].circle.y - info.robotInformation[1 - myID].circle.y));

	double forward = gme.engineRotation;
	double backward = 180 + gme.engineRotation;
	if (forward < 0)
		forward = 360 + forward;


	if (myID == 0)
	{
		switch (STATE)
		{
		case HIDE:
			stay_flame++;
			if (Distance(gme.circle.x, gsafe_x, gme.circle.y, gsafe_y) <= 15 && 
				abs(((gme.circle.y - OBSTACLE0_Y) / (gme.circle.x - OBSTACLE0_X))*tan(AngleToRadian(gme.engineRotation)) + 1) <= 3 &&
				self_Bullet > MIN_BULLET && Distance(gme.circle, genermy.circle) > 450)
			{
				STATE = ATTACK;
				Max_attack_flame = 25 + 35 * (distance - 430) / (double)1366;
				attack_flame = 0;
			}
			if (!info.arsenal[0].respawning_time && self_Bullet <= MIN_BULLET && gme.circle.x < OBSTACLE0_X && genermy.circle.x > 683)
				STATE = ARSENAL0;
			else if (!info.arsenal[1].respawning_time && self_Bullet <= MIN_BULLET && gme.circle.x > OBSTACLE0_X && genermy.circle.x < OBSTACLE0_X)
				STATE = ARSENAL1;
			break;

		case ATTACK:
			attack_flame++;
			if (attack_flame >= Max_attack_flame)
				STATE = BACK;
			break;
		case BACK:
			attack_flame--;
			if (attack_flame <= 0)
				STATE = HIDE;
			break;
		case ARSENAL0://要考虑敌人的位置
			if (info.arsenal[0].respawning_time)// || self_Bullet > MIN_BULLET)
				STATE = OBSTACLE0;
			break;
		case ARSENAL1://要考虑敌人的位置
			if (info.arsenal[1].respawning_time)//(self_Bullet > MIN_BULLET)
				STATE = OBSTACLE1;
			break;
		case OBSTACLE0:
			if (Isinzone(Obstacle_Zone[0], gme.circle))
			{
				STATE = HIDE;
				stay_flame = 0;
			}
			break;
		case OBSTACLE1:
			if (Isinzone(Obstacle_Zone[1], gme.circle))
			{
				STATE = HIDE;
				stay_flame = 0;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		switch (STATE)
		{
		case HIDE:
			stay_flame++;
			if (Distance(gme.circle.x, gsafe_x, gme.circle.y, gsafe_y) <= 15 &&
				abs(((gme.circle.y - OBSTACLE1_Y) / (gme.circle.x - OBSTACLE1_X))*tan(AngleToRadian(gme.engineRotation)) + 1) <= 3 &&
				self_Bullet > MIN_BULLET && Distance(gme.circle, genermy.circle) > 450)
			{
				STATE = ATTACK;
				Max_attack_flame = 25 + 35 * (distance - 430) / (double)1366;
				attack_flame = 0;
			}
			if (!info.arsenal[1].respawning_time && self_Bullet <= MIN_BULLET && gme.circle.x > OBSTACLE1_X && genermy.circle.x < 683)
				STATE = ARSENAL1;
			else if (!info.arsenal[0].respawning_time && self_Bullet <= MIN_BULLET && gme.circle.x < OBSTACLE1_X && genermy.circle.x > OBSTACLE1_X)
				STATE = ARSENAL0;
			break;

		case ATTACK:
			attack_flame++;
			if (attack_flame >= Max_attack_flame)
				STATE = BACK;
			break;
		case BACK:
			attack_flame--;
			if (attack_flame <= 0)
				STATE = HIDE;
			break;
		case ARSENAL0://要考虑敌人的位置
			if (info.arsenal[0].respawning_time)// || self_Bullet > MIN_BULLET)
				STATE = OBSTACLE0;
			break;
		case ARSENAL1://要考虑敌人的位置
			if (info.arsenal[1].respawning_time)//(self_Bullet > MIN_BULLET)
				STATE = OBSTACLE1;
			break;
		case OBSTACLE0:
			if (Isinzone(Obstacle_Zone[0], gme.circle))
				STATE = HIDE;
			break;
		case OBSTACLE1:
			if (Isinzone(Obstacle_Zone[1], gme.circle))
				STATE = HIDE;
			break;
		default:
			break;
		}
	}
	span++;
}
//只判断圆心是否在内
bool Isinzone(Box zone, Circle circle)
{
	//fout << "帧：" << flame << "  位置：" << gme.circle.x << "," << gme.circle.y << "  区域中心：" << zone.x << "," << zone.y;
	if (circle.x <= zone.x + zone.width / 2 &&
		circle.x >= zone.x - zone.width / 2 &&
		circle.y <= zone.y + zone.height / 2 &&
		circle.y >= zone.y - zone.height / 2)
	{
		//fout << "位置：" << circle.x << "," << circle.y << "  区域中心：" << zone.x << "," << zone.y << endl;
		//fout << "边界：" << zone.x + zone.width/2 << "," << zone.x - zone.width/2 << ";;;" << zone.y + zone.height / 2 << "," << zone.y - zone.height / 2 << endl;
		//fout << " 在内" << endl;
		return true;
	}
	else
	{
		//fout << "位置：" << gme.circle.x << "," << gme.circle.y << "  区域中心：" << zone.x << "," << zone.y << endl;
		//fout << "边界：" << zone.x + zone.width/2 << "," << zone.x - zone.width/2 << ";;;" << zone.y + zone.height / 2 << "," << zone.y - zone.height / 2 << endl;
		//fout << " 在外" << endl;
		return false;
	}
}
//只判断圆心是否在内
bool Isinzone(Circle zone, Circle circle)
{
	if (Distance(zone.x, circle.x, zone.y, circle.y) < zone.r)
		return true;
	else
		return false;
}
//判断圆心是否在以目标点为中心的边长为20的方形区域
bool Isarrive(int x, int y, Circle c)
{
	if (Distance(x,c.x,y,c.y) < 12)
		return true;
	else
		return false;
}

int Pre_Wind_Obstacle(RobotAI_Order& order)
{
	int l = 0;
	if (0.4978*gme.circle.x + gme.circle.y - 680 <= 0/*Isinzone(Obstacle_Zone[0], gme.circle)*/)
	{
		//fout << "帧：" << flame << " 左上角障碍区";
		if (gme.circle.x >= OBSTACLE0_X + 129.9 && gme.circle.y > OBSTACLE0_Y)
			l = 6;
		else if (gme.circle.x >= OBSTACLE0_X + 129.9 && gme.circle.y <= OBSTACLE0_Y)
			l = 5;
		else if (abs(gme.circle.x) < OBSTACLE0_X + 129.9 && gme.circle.y <= OBSTACLE0_Y)
			l = 4;
		else if (gme.circle.x <= OBSTACLE0_X - 129.9 && gme.circle.y <= OBSTACLE0_Y)
			l = 3;
		else if (gme.circle.x <= OBSTACLE0_X - 129.9 && gme.circle.y > OBSTACLE0_Y)
			l = 2;
		else
			l = 1;
	}
	else /*if (Isinzone(Obstacle_Zone[1], gme.circle))*/
	{
		//fout << "帧：" << flame << " 右下角障碍区";
		if (gme.circle.x >= OBSTACLE1_X + 129.9 && gme.circle.y > OBSTACLE1_Y)
			l = 12;
		else if (gme.circle.x >= OBSTACLE1_X + 129.9 && gme.circle.y <= OBSTACLE1_Y)
			l = 11;
		else if (abs(gme.circle.x) < OBSTACLE1_X + 129.9 && gme.circle.y <= OBSTACLE1_Y)
			l = 10;
		else if (gme.circle.x <= OBSTACLE1_X - 129.9 && gme.circle.y <= OBSTACLE1_Y)
			l = 9;
		else if (gme.circle.x <= OBSTACLE1_X - 129.9 && gme.circle.y > OBSTACLE1_Y)
			l = 8;
		else
			l = 7;
	}
	//fout << "  位于:" << l << "区" << endl;
	return l;
}
void Wind_Obstacle(RobotAI_Order& order,int &status)
{

	switch (status)
	{
	//case -1:
	//	GotoPoint(order, OBSTACLE1_X, OBSTACLE1_Y);
	//	break;
	//case -2:
	//	GotoPoint(order, OBSTACLE0_X, OBSTACLE0_Y);
	//	break;
	case 1:
		if(GotoPoint(order, OBSTACLE0_X + Key_Point[1].x, OBSTACLE0_Y + Key_Point[1].y))
			status = 6;
		break;
	case 2:
		if(GotoPoint(order, OBSTACLE0_X + Key_Point[2].x, OBSTACLE0_Y + Key_Point[2].y))
			status = 1;
		break;
	case 3:
		if(GotoPoint(order, OBSTACLE0_X + Key_Point[3].x, OBSTACLE0_Y + Key_Point[3].y))
			status = 2;
		break;
	case 4:
		if(GotoPoint(order, OBSTACLE0_X + Key_Point[4].x, OBSTACLE0_Y + Key_Point[4].y))
			status = 3;
		break;
	case 5:
		if(GotoPoint(order, OBSTACLE0_X + Key_Point[5].x, OBSTACLE0_Y + Key_Point[5].y))
			status = 4;
		break;
	case 6:
		if(GotoPoint(order, OBSTACLE0_X + Key_Point[0].x, OBSTACLE0_Y + Key_Point[0].y))
			status = 5;
		break;
	case 7:
		if(GotoPoint(order, OBSTACLE1_X + Key_Point[1].x, OBSTACLE1_Y + Key_Point[1].y))
			status = 12;
		break;
	case 8:
		if(GotoPoint(order, OBSTACLE1_X + Key_Point[2].x, OBSTACLE1_Y + Key_Point[2].y))
			status = 7;
		break;
	case 9:
		if(GotoPoint(order, OBSTACLE1_X + Key_Point[3].x, OBSTACLE1_Y + Key_Point[3].y))
			status = 8;
		break;
	case 10:
		if(GotoPoint(order, OBSTACLE1_X + Key_Point[4].x, OBSTACLE1_Y + Key_Point[4].y))
			status = 9;
		break;
	case 11:
		if(GotoPoint(order, OBSTACLE1_X + Key_Point[5].x, OBSTACLE1_Y + Key_Point[5].y))
			status = 10;
		break;
	case 12:
		if(GotoPoint(order, OBSTACLE1_X + Key_Point[0].x, OBSTACLE1_Y + Key_Point[0].y))
			status = 11;
	default:
		break;
	}
}
void Behind_Obstacle(RobotAI_Order& order, int myID)
{
	if (myID == 0)
	{
		double theta = atan2((double)(genermy.circle.y - OBSTACLE0_Y), (double)(genermy.circle.x - OBSTACLE0_X));
		gsafe_x = OBSTACLE0_X - 125 * cos(theta);
		gsafe_y = OBSTACLE0_Y - 125 * sin(theta);
		GotoPoint(order, gsafe_x, gsafe_y);
		//fout << "theta:" << theta << " enermy:" << genermy.circle.x << "," << genermy.circle.y;
		//fout << "  目标：" << OBSTACLE0_X - 130 * cos(theta) << "," << OBSTACLE0_Y - 130 * sin(theta) << endl;
	}
	else
	{
		double theta = atan2((double)(genermy.circle.y - OBSTACLE1_Y), (double)(genermy.circle.x - OBSTACLE1_X));
		gsafe_x = OBSTACLE1_X - 125 * cos(theta);
		gsafe_y = OBSTACLE1_Y - 125 * sin(theta);
		GotoPoint(order, gsafe_x, gsafe_y);
	}
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

	//if (info.robotInformation[1-myID].weaponTypeName == WT_MissileLauncher)

	/*后退的攻击*/
	/*躲子弹（。。）。。。预判断敌人方向射击*/



	flame++;
	gme = info.robotInformation[myID];
	genermy = info.robotInformation[1 - myID];
	//根据距离确定炮台偏移角度
	double dx = genermy.circle.x - pre_genermy.circle.x;
	double dy = genermy.circle.y - pre_genermy.circle.y;
	To_target.ex = genermy.circle.x + dx;
	To_target.ey = genermy.circle.y + dy;
	To_enermy.x = To_target.x = gme.circle.x;
	To_enermy.y = To_target.y = gme.circle.y;
	To_enermy.ex = genermy.circle.x;
	To_enermy.ey = genermy.circle.y;
	AdjustBeam(To_enermy);
	AdjustBeam(To_target);
	//fout << "帧：" << flame << "  位置：" << gme.circle.x << "," << gme.circle.y << " !!!状态：" << STATE << endl;
	//fout << "weapon:" << gme.weaponRotation << "  Beam:" << To_enermy.rotation << endl;
	//fout << "0respawning_time:" << info.arsenal[0].respawning_time;
	//fout << "  1respawning_time:" << info.arsenal[1].respawning_time;
	//fout << "  span:" << span << "  remain_ammo:" << gme.remainingAmmo << endl;
	//if (Distance(genermy.circle, info.arsenal[0].circle) <= 75 && info.arsenal[0].respawning_time <= 5)
	//	Aim_(order, info.robotInformation[myID].weaponRotation, info.arsenal[0].circle.x, info.arsenal[0].circle.y);
	//else if (Distance(genermy.circle, info.arsenal[1].circle) <= 75 && info.arsenal[1].respawning_time <= 5)
	//	Aim_(order, info.robotInformation[myID].weaponRotation, info.arsenal[1].circle.x, info.arsenal[1].circle.y);
	//else
	Aim_(order, info.robotInformation[myID].weaponRotation,To_target.ex,To_target.ey);

	Strategy1(info, myID);

	static int pre_order = 0;

	switch (STATE)
	{
	case HIDE:
		Behind_Obstacle(order, myID);
		break;
	case ATTACK:
		//fout << "engine:" << gme.engineRotation << "  me.x:" << gme.circle.x << "  en.x:" << genermy.circle.x;
		//if (gme.circle.x > genermy.circle.x)
		//{
		//	if (abs(gme.engineRotation) < 90)
		//		order.run = -1;
		//	else
		//		order.run = 1;
		//}
		//else
		//{
		//	if (abs(gme.engineRotation) < 90)
		//		order.run = 1;
		//	else
		//		order.run = -1;
		//}

		/*if (gme.circle.y <= 300 && gme.circle.y >= 100)
		{
			if (dy * gme.engineRotation > 0)
				pre_order = order.run = 1;
			else if (dy * gme.engineRotation < 0)
				pre_order = order.run = -1;
		}
		else if (gme.circle.x <= 350 && gme.circle.x >= 250)
		{
			if (dy * gme.engineRotation > 0)
				pre_order = order.run = 1;
			else if (dy * gme.engineRotation < 0)
				pre_order = order.run = -1;
		}*/
		order.run = 1;
		break;
	case BACK:
		//if (gme.circle.x > genermy.circle.x)
		//{
		//	if (abs(gme.engineRotation) < 90)
		//		order.run = 1;
		//	else
		//		order.run = -1;
		//}
		//else
		//{
		//	if (abs(gme.engineRotation) < 90)
		//		order.run = -1;
		//	else
		//		order.run = 1;
		//}
		/*order.run = -pre_order;*/
		order.run = -1;
		break;

	case ARSENAL0:
		GotoPoint(order, ARSENAL0_X, ARSENAL0_Y);
		break;
	case ARSENAL1:
		GotoPoint(order, ARSENAL1_X, ARSENAL1_Y);
		break;
	case OBSTACLE0:
		GotoPoint(order, OBSTACLE0_X - 130, OBSTACLE0_Y);
		break;
	case OBSTACLE1:
		GotoPoint(order, OBSTACLE1_X + 130, OBSTACLE1_Y);
		break;
	default:
		break;
	}




	//double x = (double)(gme.circle.y*(genermy.circle.x - gme.circle.x)) / (double)(genermy.circle.y + gme.circle.y) + gme.circle.x;
	//Aim_(order, info.robotInformation[myID].weaponRotation, x, 0);
	//往下/左/右反弹


	//改进瞄准。。。。。。。。。。。。。。。。。。。。
	Circle Bigger[2] = { info.obstacle[0], info.obstacle[1] };
	Bigger[1].r += 10;
	Bigger[0].r += 10;
	if (abs(gme.weaponRotation - To_target.rotation) <= 5)
	{//y相等时有问题，自己写。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
		//fout << "aimed";
		if (!HitTestBeamCircle(To_target, /*info.obstacle[0]*/Bigger[0]))
		{
			//fout << "//not hit 0 ";
			if (!HitTestBeamCircle(To_target, /*info.obstacle[1]*/Bigger[1]))
			{
				//fout << "//not hit 1";
				order.fire = 1;
			}
			//else
				//fout << "//hit 1";
		}
		else
		{
			//fout << "//hit 0";
		}
	}
	pre_genermy = info.robotInformation[1 - myID];
	pre_gme = info.robotInformation[myID];
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

	weapon = WT_Tesla;	//啊，我爱加农炮
	engine = ET_Robotman;	//啊，我爱小蜘蛛
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "Burglar";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "no name";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	SetSeed();
	return Random(-255,255);
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	//SetSeed();
	return Random(-255, 255);
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	//SetSeed();
	return Random(-255, 255);
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	//SetSeed();
	return Random(-255, 255);
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	//SetSeed();
	return Random(-255, 255);
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	//SetSeed();
	return Random(-255, 255);
}









//-----------------------------------------------------
//3.用不用随你的触发函数
//-----------------------------------------------------


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info, int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//if (!myID)
	//	l = 左;
	//else
	//	l = 右;
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
	//if (fireID == myID)

}


void RobotAI::onHit(int launcherID,bullettypename btn)
{
	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
	
}



//TODO:这里可以实现你自己的函数