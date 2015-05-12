#include "RobotAI.h"
#include<cstdlib>
#include<math.h>
#include<iostream>
#include<vector>
#include<string>
#include<iomanip>
using namespace std;

#define moveOffset 2.0
//类型定义
enum EnemyMovePlan
{
	Straight_Line,
	Straight_Line_Vertical,
	Straight_Line_Horizontal,
	Circle_Line,
	Stop
};
//全局变量定义
static int myTimeCount = 1;
static int roundObstacleTime = 1;
int roundObstacleDirection = 1;
int readyToTurnAround = 0;
int isAvoidingBullet = 0;
int startAvoidBullet = 0;
int MyId;
int avoidTime = 1;
int avoidDirection = 0;
double myEngineRotation;
EnemyMovePlan enemyMovePlan = Straight_Line;
vector<Circle> enemyMoveRecord;

//函数声明
EnemyMovePlan judgeStraghtLine();
bool engineFace(int direction);
bool existObstacle(Circle myCircle, Circle enemyCircle);
bool canHitEngine(Circle engineCircle, RobotAI_BulletInformation info, double offset);


RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}


//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------



//=================================输出信息函数===========================
void outputMessage(double m)
{
	if (myTimeCount % 10 == 0)
	{
		cout << "您要的信息：" << m << endl;
	}
}
void outputMessage(string m)
{
	if (myTimeCount % 10 == 0)
	{
		cout << "您要的信息：" << m << endl;
	}
}

void killDi(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	//获取战场信息===================================================
	RobotAI_RobotInformation enemyInformation;
	RobotAI_RobotInformation myInformation;
	if (myID == 0)
	{
		enemyInformation = info.robotInformation[1];
		myInformation = info.robotInformation[0];
	}
	else
	{
		enemyInformation = info.robotInformation[0];
		myInformation = info.robotInformation[1];
	}
	Circle enemyCircle = enemyInformation.circle;
	Circle myCircle = myInformation.circle;

	Box boundary = info.boundary;

	//===============================================================

	//武器控制

}
//=======================是否进入电锯范围函数=============================
bool inTheSawAttackRange(Circle & circle1, Circle & circle2,double offset)
{
	double account = pow(circle1.x - circle2.x, 2.0) + pow(circle1.y - circle2.y, 2.0);
	if (account <= pow(circle2.r + offset, 2.0))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//=======================是否进入需要避开障碍物范围==================
bool inAvoidObstacleRange(Circle & circle1, Circle circle2)
{
	double minDistance = 1.0;
	double account = pow(circle1.x - circle2.x, 2.0) + pow(circle1.y - circle2.y, 2.0);
	if (account <= pow(circle1.r + circle2.r + minDistance, 2.0))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//=======================武器瞄准函数================================
void weaponAimDirect(RobotAI_Order& order, RobotAI_RobotInformation & myInfo, RobotAI_RobotInformation & enemyInfo, Circle my, Circle enemy)
{
	double rotation = atan2(enemy.y - my.y,enemy.x - my.x) * 180 / 3.1415926 + 180.0;
	double weaponRotation = myInfo.weaponRotation + 180.0;
	
	if (weaponRotation >= (rotation + 1.0) || weaponRotation <= (rotation - 1.0))
	{
		
		if ((rotation - weaponRotation)>1.0)
		{
			if ((rotation - weaponRotation) >= 180.0)
				order.wturn = -1;
			else
				order.wturn = 1;
		}
		else if ((rotation - weaponRotation)<-1.0)
		{
			if ((rotation - weaponRotation) <= -180.0)
				order.wturn = 1;
			else
				order.wturn = -1;
		}
	}
	else
	{
		order.wturn = 0;
	}

}
//==================================预判敌人下一步移动位置======================
Circle forecastEnemyNextPoint(RobotAI_RobotInformation & enemyInfo, double offset)
{
	int size = enemyMoveRecord.size();
	if (size >= 4)
	{
		Circle enemyCircle;
		enemyCircle.r = enemyInfo.circle.r;
		enemyCircle.x = 0;
		enemyCircle.y = 0;
		double x1 = enemyMoveRecord[size - 2].x;
		double x2 = enemyMoveRecord[size - 1].x;
		double y1 = enemyMoveRecord[size - 2].y;
		double y2 = enemyMoveRecord[size - 1].y;
		if (enemyMovePlan == Straight_Line_Horizontal)
		{
			if ((x2 - x1) >= 0)
			{
				enemyCircle.x = x2 + (x2 - x1 + offset);
			}
			else
			{
				enemyCircle.x = x2 - (x1 - x2 + offset);
			}
			enemyCircle.y = y2;
		}
		else if (enemyMovePlan == Straight_Line_Vertical)
		{
			if ((y2 - y1) >= 0)
			{
				enemyCircle.y = y2 + (y2 - y1 + offset);
			}
			else
			{
				enemyCircle.y = y2 - (y1 - y2 + offset);
			}
			enemyCircle.x = x2;
		}
		else if (enemyMovePlan == Straight_Line)
		{
			double k = (y2 - y1) / (x2 - x1);
			double b = y1 - k*x1;
			if ((x2 - x1) >= 0)
			{
				enemyCircle.x = x2 + (x2 - x1 + offset);
			}
			else
			{
				enemyCircle.x = x2 - (x1 - x2 + offset);
			}
			enemyCircle.y = k*(enemyCircle.x) + b;
		}
		else if (enemyMovePlan == Circle_Line)
		{
			enemyCircle.x = x2;
			enemyCircle.y = y2;
		}
		else if (enemyMovePlan == Stop)
		{
			enemyCircle.x = enemyInfo.circle.x;
			enemyCircle.y = enemyInfo.circle.y;
		}
		//cout << "原位置： " << x1 << ',' << y1 << "  " << x2 << ',' << y2 << "  预测位置：" << enemyCircle.x << ',' << enemyCircle.y << endl;
		return enemyCircle;
	}
	else
	{
		//cout << "数据不足无法预测" << endl;
		return enemyInfo.circle;
	}
}
//==================================记录敌人移动位置并判断其移动方案（每N帧调用）======================
void recordEnemyMovement(Circle enemyCircle,int interval,unsigned int maxRecord)
{
	static int count = 0;
	if (count++%interval == 0)
	{
		if (enemyMoveRecord.size() < maxRecord)
		{
			enemyMoveRecord.push_back(enemyCircle);
		}
		else
		{
			enemyMoveRecord.erase(enemyMoveRecord.begin());
			enemyMoveRecord.push_back(enemyCircle);
			//达到maxRecord，开始判断
			enemyMovePlan = judgeStraghtLine();
		}
	}
}
//==================================判断移动方案函数============================
EnemyMovePlan judgeStraghtLine()
{
	double countX = 0;
	double countY = 0;
	for (unsigned int i = 0; i < enemyMoveRecord.size(); i++)
	{
		countX += enemyMoveRecord[i].x;
	}
	for (unsigned int i = 0; i < enemyMoveRecord.size(); i++)
	{
		countY += enemyMoveRecord[i].y;
	}
	double evgX = countX / enemyMoveRecord.size();
	double evgY = countY / enemyMoveRecord.size();
	int flagX = 1;
	int flagY = 1;
	for (unsigned int i = 0; i < enemyMoveRecord.size(); i++)
	{
		if (fabs(evgX - enemyMoveRecord[i].x)>moveOffset)
		{
			flagX = 0;
			break;
		}
	}
	for (unsigned int i = 0; i < enemyMoveRecord.size(); i++)
	{
		if (fabs(evgY - enemyMoveRecord[i].y)>moveOffset)
		{
			flagY = 0;
			break;
		}
	}
	if (flagX == 1 && flagY == 1)
	{
		/*cout << "静止";
		for (int i = 0; i < enemyMoveRecord.size(); i++)
		{
		cout << enemyMoveRecord[i].x << ',' << enemyMoveRecord[i].y << "  ";
		}
		cout << endl;*/
		return Stop;
	}
	else if (flagX == 1 && flagY == 0)
	{
		cout << "垂直" << endl;
		return Straight_Line_Vertical;
	}
	else if (flagX == 0 && flagY == 1)
	{
		cout << "水平" << endl;
		return Straight_Line_Horizontal;
	}
	else
	{
		double x1 = enemyMoveRecord[0].x;
		double x2 = enemyMoveRecord[1].x;
		double y1 = enemyMoveRecord[0].y;
		double y2 = enemyMoveRecord[1].y;
		double k, b;
		if (x1 == x2)
		{
			return Straight_Line;
		}
		else
		{
			k = (y2 - y1) / (x2 - x1);
			b = y1 - k*x1;
			for (unsigned int i = 2; i < enemyMoveRecord.size(); i++)
			{
				if ((k*enemyMoveRecord[i].x + b - enemyMoveRecord[i].y)>moveOffset)
				{
					cout << "曲线";/*
					for (int i = 1; i < enemyMoveRecord.size(); i++)
					{
						cout << setprecision(2) << enemyMoveRecord[i].x - enemyMoveRecord[i - 1].x << ',' << enemyMoveRecord[i].y - enemyMoveRecord[i-1].y << "  ";
					}
					cout << endl;*/
					return Circle_Line;
				}
				else
				{
					cout << "直线";/*
					for (int i = 1; i < enemyMoveRecord.size(); i++)
					{
						cout << setprecision(2) << enemyMoveRecord[i].x - enemyMoveRecord[i - 1].x << ',' << enemyMoveRecord[i].y - enemyMoveRecord[i - 1].y << "  ";
					}
					cout << endl;*/
					return Straight_Line;
				}
			}
		}
		
	}
	
}
//==================================载具绕过障碍物函数==========================
void engineKeepRoundObstacle(RobotAI_Order& order,RobotAI_RobotInformation & info, Circle eCircle, Circle oCircle, int direction)
{
	double rotation = atan2(oCircle.y - eCircle.y, oCircle.x - eCircle.x) * 180 / 3.1415926 + 180.0;
	double engineRotation = info.engineRotation + 180.0;
	if (fabs(engineRotation - rotation) >= 90.5)
	{
		order.eturn = 1 * direction;
	}
	else
	{
		order.eturn = -1 * direction;
	}
	roundObstacleTime++;
}
//==================================载具方向瞄准函数============================
void engineAimDirect(RobotAI_Order& order, RobotAI_RobotInformation & info, Circle my, Circle enemy)
{
	double rotation = atan2(enemy.y - my.y, enemy.x - my.x) * 180 / 3.1415926 + 180.0;
	double engineRotation = info.engineRotation + 180.0;
	double accuracy = 2.0;
	if (engineRotation >= (rotation + accuracy) || engineRotation <= (rotation - accuracy))
	{

		if ((rotation - engineRotation)>accuracy)
		{
			if ((rotation - engineRotation) >= 180.0)
				order.eturn = -1;
			else
				order.eturn = 1;
		}
		else if ((rotation - engineRotation)<(accuracy*-1.0))
		{
			if ((rotation - engineRotation) <= -180.0)
				order.eturn = 1;
			else
				order.eturn = -1;
		}
	}
	else
	{
		order.wturn = 0;
	}

}
//=================================判断载具是否粘着障碍物==============================
bool isEngineLeanObstacle(Circle enemyCircle,const Circle * obstacles,double offset)
{
	int flag = 0;
	for (int i = 0; i < 2; i++)
	{
		double distance_2 = pow(enemyCircle.x - obstacles[i].x, 2.0) + pow(enemyCircle.y - obstacles[i].y, 2.0);
		if (distance_2 <= pow(enemyCircle.r + obstacles[i].r + offset, 2.0))
		{
			flag++;
		}
	}
	if (flag > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//=================================判断两点之间是否有障碍物==============================
string existObstacle(Circle myCircle, Circle enemyCircle, const Circle *  obstacles)
{
	double k, b;
	double distance;
	string result = "";
	if (myCircle.x == enemyCircle.x)
	{
		for (int i = 0; i < 2; i++)
		{
			double x = obstacles[i].x;
			double y = obstacles[i].y;
			double r = obstacles[i].r;
			distance = fabs(myCircle.x - x);
			if (distance < r)
			{
				if (((y < myCircle.y) && (y>enemyCircle.y)) || ((y > myCircle.y) && (y < enemyCircle.y)))
				{
					result += 'Y';
				}
				else
				{
					result += 'N';
				}
			}
			else
			{
				result += 'N';
			}
		}
	}
	else
	{
		k = (enemyCircle.y - myCircle.y) / (enemyCircle.x - myCircle.x);
		b = enemyCircle.y - k*enemyCircle.x;
		for (int i = 0; i < 2; i++)
		{
			double x = obstacles[i].x;
			double y = obstacles[i].y;
			double r = obstacles[i].r;

			distance = fabs(k*x - y + b) / sqrt(pow(k, 2.0) + 1);

			if (distance < r)
			{
				if (k == 0)
				{
					if (((x < myCircle.x) && (x > enemyCircle.x)) || ((x > myCircle.x) && (x < enemyCircle.x)))
					{
						result += 'Y';
					}
					else
					{
						result += 'N';
					}
				}
				else
				{
					double verticle_k, verticle_b;
					verticle_k = -1 / k;
					verticle_b = y - verticle_k * x;
					double intersect_x, intersect_y;
					intersect_x = (verticle_b - b) / (k - verticle_k);
					intersect_y = k*intersect_x + b;
					if (((intersect_x <(myCircle.y - b) / k) && (intersect_x >(enemyCircle.y - b) / k)) ||
						((intersect_x > (myCircle.y - b) / k) && (intersect_x < (enemyCircle.y - b) / k)))
					{
						result += 'Y';
					}
					else
					{
						result += 'N';
					}
				}
			}
			else
			{
				result += 'N';
			}
		}

	}
	return result;
}
//=================================获取躲避子弹方向==============================
int getAvoidBulletDirection(Circle myCircle,const Circle * obstacles,double offset)
{
	if (myCircle.y <= (50 + offset))
	{
		cout << "靠近顶部" << endl;
		if (engineFace(14))
			return 1;
		else
			return -1;
	}
	else if (myCircle.y >= (630 - offset))
	{
		cout << "靠近底部" << endl;
		if (engineFace(14))
			return -1;
		else
			return 1;
	}
	else if (myCircle.x <= (50 + offset))
	{
		cout << "靠近左边" << endl;
		if (engineFace(12))
			return 1;
		else
			return -1;
	}
	else if (myCircle.x >= (1316 - offset))
	{
		cout << "靠近右边" << endl;
		if (engineFace(12))
			return -1;
		else
			return 1;
	}
	for (int i = 0; i < 2; i++)
	{
		if (inAvoidObstacleRange(myCircle, obstacles[i]))
		{
			cout << "靠近障碍物" << endl;
			if (myCircle.y <= obstacles[i].y)
			{
				if (engineFace(14))
					return -1;
				else
					return 1;
			}
			else
			{
				if (engineFace(14))
					return 1;
				else
					return -1;
			}
		}
		else
		{
			continue;
		}
	}
	cout << "不靠近障碍物" << endl;
	return 1;
}
//=================================获取引擎朝向==============================
bool engineFace(int direction)
{
	int dir;
	while (true)
	{
		dir = direction % 10;
		if (myEngineRotation <= 0 && myEngineRotation > -90 && dir == 1)
		{
			return true;
		}
		else if (myEngineRotation <= -90 && myEngineRotation > 180 && dir == 2)
		{
			return true;
		}
		else if (myEngineRotation <= 180 && myEngineRotation > 90 && dir == 3)
		{
			return true;
		}
		else if (myEngineRotation <= 90 && myEngineRotation > 0 && dir == 4)
		{
			return true;
		}
		direction /= 10;
		if (direction == 0)
			break;
	}
}
//=================================判断是否需要躲避子弹==============================
bool needAvoidBullet(Circle myCircle, const RobotAI_BattlefieldInformation& info)
{
	switch (info.robotInformation[1 - MyId].weaponTypeName)
	{
	case WT_Cannon:
	//case WT_Shotgun:
	case WT_RPG:
	case WT_Machinegun:
	//case WT_Prism:
	//case WT_Tesla:
	case WT_PlasmaTorch:
	//case WT_MissileLauncher:
	//case WT_ElectricSaw:

	//		//二期
	//case WT_GrenadeThrower:
	//case WT_MineLayer:
	case WT_Apollo:
		break;
	default:
		return false;
		break;
	}
	cout << "=========================有子弹发射==========================" << endl;
	for (int i = 0; i <= info.num_bullet; i++)
	{
		cout << "循环判断" << endl;
		if (info.bulletInformation[i].launcherID == (1 - MyId))
		{
			//只有当子弹在两引擎之间才进行判断
			if (!((info.bulletInformation[i].circle.x >= myCircle.x &&info.bulletInformation[i].circle.x <= info.robotInformation[1 - MyId].circle.x) ||
				(info.bulletInformation[i].circle.x >= info.robotInformation[1 - MyId].circle.x &&info.bulletInformation[i].circle.x <= myCircle.x)))
			{
				cout << "不在引擎间，下一个" << endl;
				continue;
			}
			if (existObstacle(myCircle, info.bulletInformation[i].circle, info.obstacle) != "NN")
			{
				cout << "有障碍物，下一个" << endl;
				continue;
			}
			else
			{
				if (canHitEngine(myCircle, info.bulletInformation[i],10.0))
				{
					return true;
				}
				else
				{
					continue;
				}
			}
		}
		else
		{
			cout << "不是自己发射的，下一个" << endl;
			continue;
		}
	}
	return false;
}
//=================================判断子弹是否会击中引擎==============================
bool canHitEngine(Circle engineCircle, RobotAI_BulletInformation info,double offset)
{
	if (info.rotation != -90 && info.rotation != 90)
	{
		double k = tan(info.rotation / 180.0*3.1415926);
		double b = info.circle.y - info.circle.x * k;
		double pre_y = k * engineCircle.x + b;
		cout << k << '\t' << b << endl;
		cout << engineCircle.x << '\t' << engineCircle.y << endl;
		cout << pre_y << endl;
		if (pre_y >= (engineCircle.y - engineCircle.r - offset) && pre_y <= (engineCircle.y + engineCircle.r + offset))
		{
			cout << "可击中" << endl;
			return true;
		}
		else
		{
			cout << "不可击中" << endl;
			return false;
		}
	}
	else
	{
		if (info.circle.x >= (engineCircle.x - engineCircle.r) && info.circle.x <= (engineCircle.x - engineCircle.r))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	
}
void myPlay(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	RobotAI_RobotInformation enemyInformation;
	RobotAI_RobotInformation myInformation;
	if (myID == 0)
	{
		enemyInformation = info.robotInformation[1];
		myInformation = info.robotInformation[0];
	}
	else
	{
		enemyInformation = info.robotInformation[0];
		myInformation = info.robotInformation[1];
	}

	Circle enemyCircle = enemyInformation.circle;
	Circle myCircle = myInformation.circle;

	order.run = 1;
	//移动策略************************************************************
	string obstacleResult = existObstacle(myCircle, enemyCircle, info.obstacle);
	if (enemyInformation.engineTypeName == ET_GhostTank)
	{

	}
	//绕环每60帧检测是否需要掉头
	if ((roundObstacleTime % 120 == 0) && isEngineLeanObstacle(enemyCircle, info.obstacle, 20.0) && readyToTurnAround == 0)
	{
		cout << "打开readyToTurnAround" << endl;
		readyToTurnAround = 1;
	}
	//检测是否要开始掉头
	if (readyToTurnAround == 1 && (myCircle.x >= 375 && myCircle.x <= 991))
	{
		roundObstacleDirection = roundObstacleDirection * (-1);
		readyToTurnAround = 0;
	}
	if (obstacleResult.length() == 2)
	{
		if (isAvoidingBullet != 1 && startAvoidBullet != 1)
		{
			if (obstacleResult[0] == 'Y')
			{
				if (inAvoidObstacleRange(myCircle, info.obstacle[0]))
				{
					engineKeepRoundObstacle(order, myInformation, myCircle, info.obstacle[0], roundObstacleDirection);
				}
				else
				{
					engineAimDirect(order, myInformation, myCircle, enemyCircle);
				}
			}
			else if (obstacleResult[1] == 'Y')
			{
				if (inAvoidObstacleRange(myCircle, info.obstacle[1]))
				{
					engineKeepRoundObstacle(order, myInformation, myCircle, info.obstacle[1], roundObstacleDirection);
				}
				else
				{
					engineAimDirect(order, myInformation, myCircle, enemyCircle);
				}
			}
			else if (obstacleResult[0] == 'N'&&obstacleResult[1] == 'N')
			{
				engineAimDirect(order, myInformation, myCircle, enemyCircle);
			}
		}
		else
		{
			if (isAvoidingBullet == 1&&startAvoidBullet == 0 && needAvoidBullet(myCircle, info))
			{
				startAvoidBullet = 1;
				isAvoidingBullet = 0;
				avoidDirection = getAvoidBulletDirection(myCircle, info.obstacle, 10.0);
				cout << "开始躲避，躲避方向为：" << avoidDirection << endl;
			}
			else
			{
				isAvoidingBullet = 0;
			}
			if (startAvoidBullet == 1)
			{
				order.eturn = avoidDirection;
				avoidTime++;
				if (avoidTime >= 30)
				{
					cout << "躲避结束" << endl;
					avoidTime = 1;
					startAvoidBullet = 0;
				}
			}
		}
	}
	else
	{
		cout << "判断两点间是否有障碍物出错" << endl;
	}




	weaponAimDirect(order, myInformation, enemyInformation, myCircle, enemyCircle);
	if (inTheSawAttackRange(myCircle, enemyCircle, 95.0))order.fire = 1;
}
void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	MyId = myID;
	//获取战场信息===================================================
	RobotAI_RobotInformation enemyInformation;
	RobotAI_RobotInformation myInformation;
	if (myID == 0)
	{
		enemyInformation = info.robotInformation[1];
		myInformation = info.robotInformation[0];
	}
	else
	{
		enemyInformation = info.robotInformation[0];
		myInformation = info.robotInformation[1];
	}

	myEngineRotation = myInformation.engineRotation;
	Circle enemyCircle = enemyInformation.circle;
	Circle myCircle = myInformation.circle;


	//===============================================================

	//killDi(order, info, myID);
	

		
	/*if (inAvoidObstacleRange(myCircle, info.obstacle[1]))
		engineKeepRoundObstacle(order, myInformation, myCircle, enemyCircle, 1);
	else
		engineAimDirect(order, myInformation, myCircle, enemyCircle);*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%测试机器%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	/*order.run = 3;
	if (myTimeCount >= 200)order.fire = 1;
	
	int flag = 0;
	if (myTimeCount <= 300)
	{
		if (inAvoidObstacleRange(myCircle, info.obstacle[1]))
			engineKeepRoundObstacle(order, myInformation, myCircle, info.obstacle[1], 1);
		else
			engineAimDirect(order, myInformation, myCircle, info.obstacle[1]);
	}
	else
	{
		if (myCircle.y <= 500)
		{
			flag = 1;
		}
		if (flag == 0)
		{
			engineKeepRoundObstacle(order, myInformation, myCircle, info.obstacle[1], 1);
		}
		else
		{
			if (inAvoidObstacleRange(myCircle, info.obstacle[0]))
				engineKeepRoundObstacle(order, myInformation, myCircle, info.obstacle[0], 1);
			else
				engineAimDirect(order, myInformation, myCircle, info.obstacle[0]);
		}
	}
	myTimeCount++;*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%实战机器%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//if (myTimeCount >= 200)
		myPlay(order, info, myID);
	//##############每帧必执行项目##########################
	//recordEnemyMovement(enemyCircle, 10, 4);
	myTimeCount++;
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

	
	weapon = WT_ElectricSaw;	//
	engine = ET_GhostTank;	//
	/*
	weapon = WT_Apollo;	//
	engine = ET_Quad;	//
	*/
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "测试机器";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "Laijia";
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
}


void RobotAI::onSomeoneFire(int fireID)
{
	//有机甲开火时被调用
	//参数：fireID	... 开火的机甲下标
	if (MyId != fireID)
	{
		isAvoidingBullet = 1;
	}
}


void RobotAI::onHit(int launcherID,bullettypename btn)
{
	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
}



//TODO:这里可以实现你自己的函数