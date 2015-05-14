#include "RobotAI.h"
//TODO:这里可以实现你自己的函数
#define Min -1000
#include<iostream>
using namespace std;
void getSpeed(const RobotAI_BattlefieldInformation& info, int myID, double& bulletSpeed)
{
	switch (info.robotInformation[myID].weaponTypeName){
	case WT_Cannon:          bulletSpeed = 8;    break;
	case WT_Shotgun:         bulletSpeed = 7.5;  break;
	case WT_RPG:             bulletSpeed = 6;    break;
	case WT_Machinegun:      bulletSpeed = 10;   break;
	case WT_Prism:           bulletSpeed = 2000; break;
	case WT_Tesla:           bulletSpeed = 2000; break;
	case WT_PlasmaTorch:     bulletSpeed = 5;    break;
	case WT_MissileLauncher: bulletSpeed = 4;    break;
	case WT_ElectricSaw:     bulletSpeed = 2000; break;
	default:break;
	}
}

/*double getAngle(const RobotAI_BattlefieldInformation& info,int myID){
int enermyID=myID^1;
double bulletSpeed;
getSpeed(info,myID,bulletSpeed);
VECTOR MYPOINT, ENERNMYPOINT, EV_Vector, BV_Vector, Shoot_Vector;
MYPOINT.x = info.robotInformation[myID].circle.x;
MYPOINT.y = info.robotInformation[myID].circle.y;
ENERNMYPOINT.x = info.robotInformation[enermyID].circle.x;
ENERNMYPOINT.y = info.robotInformation[enermyID].circle.y;
double DIS;
DIS = sqrt((MYPOINT.x-ENERNMYPOINT.x)*(MYPOINT.x-ENERNMYPOINT.x)+(MYPOINT.y-ENERNMYPOINT.y)*(MYPOINT.y-ENERNMYPOINT.y));
BV_Vector.x = (ENERNMYPOINT.x-MYPOINT.x)/DIS*bulletSpeed;
BV_Vector.y = (ENERNMYPOINT.y-MYPOINT.y)/DIS*bulletSpeed;
EV_Vector.x = info.robotInformation[enermyID].vx;
EV_Vector.y = info.robotInformation[enermyID].vy;
Shoot_Vector.x = BV_Vector.x+EV_Vector.x;
Shoot_Vector.y = BV_Vector.y+EV_Vector.y;
double angle;
if (Shoot_Vector.x>=0) angle = RadianToAngle(atan(Shoot_Vector.y/Shoot_Vector.x));
else angle = (RadianToAngle(atan(Shoot_Vector.y/Shoot_Vector.x))+180);
angle = AngleAdjust(angle);
return angle;
}*/
struct VECTOR{
	double x;
	double y;
};//自定义结构体
double RPGShoot(double t1, double t2, double DIS, double EV, double cosA){
	if (abs(t2 - t1)<0.01) return (t1 + t2) / 2;
	double x1 = DIS*DIS + EV*t1*EV*t1 - pow(120 * (pow(1.05, t1) - 1), 2) - 2 * DIS*EV*t1*cosA;
	double x2 = DIS*DIS + EV*t2*EV*t1 - pow(120 * (pow(1.05, t2) - 1), 2) - 2 * DIS*EV*t2*cosA;
	double t = (t1 + t2) / 2;
	double x = DIS*DIS + EV*t*EV*t1 - pow(120 * (pow(1.05, t) - 1), 2) - 2 * DIS*EV*t*cosA;
	if (x*x1<0) return RPGShoot(t1, (t2 + t1) / 2, DIS, EV, cosA);
	if (x*x2<0) return RPGShoot((t2 + t1) / 2, t2, DIS, EV, cosA);
	return (t1 + t2) / 2;
}

VECTOR getAngle(const RobotAI_BattlefieldInformation& info, int myID)
{
	int enermyID = myID ^ 1;
	double bulletSpeed;
	getSpeed(info, myID, bulletSpeed);
	VECTOR MYPOINT, ENERNMYPOINT, EV_Vector, Shoot_Vector, package;

	MYPOINT.x = info.robotInformation[myID].circle.x;
	MYPOINT.y = info.robotInformation[myID].circle.y;
	ENERNMYPOINT.x = info.robotInformation[enermyID].circle.x;
	ENERNMYPOINT.y = info.robotInformation[enermyID].circle.y;
	double DIS;
	DIS = sqrt((MYPOINT.x - ENERNMYPOINT.x)*(MYPOINT.x - ENERNMYPOINT.x) + (MYPOINT.y - ENERNMYPOINT.y)*(MYPOINT.y - ENERNMYPOINT.y));

	double BV = bulletSpeed;
	EV_Vector.x = info.robotInformation[enermyID].vx;
	EV_Vector.y = info.robotInformation[enermyID].vy;
	double EV = sqrt(EV_Vector.x*EV_Vector.x + EV_Vector.y*EV_Vector.y);
	double t;
	double angle;
	if (abs(EV)>1){
		double cosA = -((ENERNMYPOINT.x - MYPOINT.x)*EV_Vector.x + (ENERNMYPOINT.y - MYPOINT.y)*EV_Vector.y) / (DIS*EV);
		if (info.robotInformation[myID].weaponTypeName != WT_RPG){
			double a = (EV*EV - BV*BV);
			double b = -2 * DIS*EV*cosA;
			double c = DIS*DIS;
			t = (-b - sqrt(b*b - 4 * a*c)) / 2 / a;
		}
		else{
			t = RPGShoot(0, 1000, DIS, EV, cosA);
		}
		Shoot_Vector.x = ENERNMYPOINT.x - MYPOINT.x + t*EV_Vector.x;
		Shoot_Vector.y = ENERNMYPOINT.y - MYPOINT.y + t*EV_Vector.y;
	}
	else {
		Shoot_Vector.x = ENERNMYPOINT.x - MYPOINT.x;
		Shoot_Vector.y = ENERNMYPOINT.y - MYPOINT.y;
	}

	if (Shoot_Vector.x >= 0) angle = RadianToAngle(atan(Shoot_Vector.y / Shoot_Vector.x));
	else angle = (RadianToAngle(atan(Shoot_Vector.y / Shoot_Vector.x)) + 180);
	angle = AngleAdjust(angle);
	package.x = t;
	package.y = angle;
	return package;
}

double getEnermyAngle(int myID, const RobotAI_BattlefieldInformation& info, VECTOR Aim)
{
	VECTOR MYPOINT, ENERNMYPOINT, Shoot_Vector;

	MYPOINT.x = info.robotInformation[myID].circle.x;
	MYPOINT.y = info.robotInformation[myID].circle.y;
	ENERNMYPOINT.x = info.robotInformation[myID ^ 1].circle.x;
	ENERNMYPOINT.y = info.robotInformation[myID ^ 1].circle.y;

	Shoot_Vector.x = ENERNMYPOINT.x - MYPOINT.x;
	Shoot_Vector.y = ENERNMYPOINT.y - MYPOINT.y;
	double angle;
	if (Shoot_Vector.x >= 0) angle = RadianToAngle(atan(Shoot_Vector.y / Shoot_Vector.x));
	else angle = (RadianToAngle(atan(Shoot_Vector.y / Shoot_Vector.x)) + 180);
	angle = AngleAdjust(angle);

	return angle;
}

/*double forceShoot(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID,int choice)
{
static long long int times=0;
double temp;
VECTOR Aim;
switch (choice){
case 0: Aim.x = info.robotInformation[myID^1].circle.x;
Aim.y = info.robotInformation[myID^1].circle.y;
break;//滞后射击
case 1: Aim.x=info.arsenal[0].circle.x;
Aim.y=info.arsenal[0].circle.y;
break; //向军火库0射击
case 2: Aim.x=info.arsenal[1].circle.x;
Aim.y=info.arsenal[1].circle.y;
break; //向军火库1射击
case 3: Aim.x=info.robotInformation[myID^1].circle.x+info.robotInformation[myID^1].circle.r*cos(AngleToRadian(info.robotInformation[myID^1].engineRotation)+info.robotInformation[myID^1].vr*times);
Aim.y=info.robotInformation[myID^1].circle.y+info.robotInformation[myID^1].circle.r*sin(AngleToRadian(info.robotInformation[myID^1].engineRotation)+info.robotInformation[myID^1].vr*times);
break;//弹幕射击
case 4: Aim.x=1310;
Aim.y=570;
break;//出生点压制
case 5: Aim.x=50;
Aim.y=50;
break;//出生点压制
default:break;
}
order.fire=1;
times++;
times=((int)pow(37,times))%17;
temp=getEnermyAngle(myID,info,Aim);
return temp;
}*/

void turn(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, double temp)
{
	if (temp>0) order.wturn = 1;
	if (temp<0) order.wturn = -1;
	if (info.robotInformation[myID].weaponRotation>temp) order.wturn = -1; else order.wturn = 1;
	if ((info.robotInformation[myID].weaponRotation - temp)>180) order.wturn = 1;
	if ((temp - info.robotInformation[myID].weaponRotation)>180) order.wturn = -1;
}

VECTOR takeAim(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	double temp;
	VECTOR p;
	p = getAngle(info, myID);
	temp = p.y;
	//temp = forceShoot(order,info,myID,3);
	turn(order, info, myID, temp);
	return p;
}


double dist(const Point&a, const Point& b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

bool willShoot(const RobotAI_BattlefieldInformation& info, int myID, Beam shootLine, double time)
{
	int MMAX = 20000;
	Point p1, p2, p3;
	double d1, d2, d3;
	Point myPoint, special, ENERNMYPOINT;
	Circle future, newobstacle1, newobstacle2;
	future = info.robotInformation[myID ^ 1].circle;
	//MYPOINT.x = info.robotInformation[myID].circle.x;
	//MYPOINT.y = info.robotInformation[myID].circle.y;
	//ENERNMYPOINT.x = info.robotInformation[myID^1].circle.x;
	//ENERNMYPOINT.y = info.robotInformation[myID^1].circle.y;
	//double DIS;
	//DIS = dist(MYPOINT,ENERNMYPOINT);
	future.x += time*info.robotInformation[myID ^ 1].vx;
	future.y += time*info.robotInformation[myID ^ 1].vy;
	future.r -= 5;
	myPoint.x = info.robotInformation[myID].circle.x;
	myPoint.y = info.robotInformation[myID].circle.y;
	newobstacle1 = info.obstacle[0];
	newobstacle1.r += 3;
	special.x = newobstacle1.x;
	special.y = newobstacle1.y;
	newobstacle2 = info.obstacle[1];
	newobstacle2.r += 3;
	bool b[2];
	//cout<<shootLine.rotation<<' '<<shootLine.x<<' '<<shootLine.y<<'\n';
	b[0] = HitTestBeamCircle(shootLine, newobstacle1);
	b[1] = HitTestBeamCircle(shootLine, newobstacle2);
	//if (info.robotInformation[myID].weaponTypeName==WT_RPG) if(abs(myPoint.y-special.y)<40) return false; 
	if (!HitTestBeamCircle(shootLine, future)) return false;

	p1 = GetHitPoint(shootLine, future);
	d1 = dist(p1, myPoint);
	if ((!b[0]) && (!b[1])) return true;

	if (b[0]) {
		p2 = GetHitPoint(shootLine, newobstacle1);
		d2 = dist(p2, myPoint);
	}
	else d2 = MMAX;
	if (b[1]) {
		p3 = GetHitPoint(shootLine, newobstacle2);
		d3 = dist(p3, myPoint);
	}
	else d3 = MMAX;
	if ((d1<d2) && (d1<d3)) return true;
	else return false;
}
void RobotAI:: detectBullet(const RobotAI_BattlefieldInformation& info, int myID)
{
	for (int i = 0; i < length; i++)
	for (int j = 0; j < width; j++)
		matrix[i][j] = 0;
	for (int i = 300 - 75; i <= 300 + 75;i++)
	for (int j = 250 - 75; j <= 250 + 75; j++)
		matrix[i][j] = Min;
	for (int i = 1066 - 75; i <= 1066 + 75;i++)
	for (int j = 430 - 75; j <= 430 + 75; j++)
		matrix[i][j] = Min;
	for (int i = 0; i < info.num_bullet; i++)
	
	{

		RobotAI_BulletInformation temp = info.bulletInformation[i];
		int x = temp.circle.x + temp.vx;
		int y = temp.circle.y + temp.vy;
		if (x < 0)x = 0;
		if (y < 0) y = 0;
		if (x >= length) x = length - 1;
		if (y >= width) y = width - 1;
		matrix[x][y] += -100;
		//detect your opponent and set forbidden area as well
	//	cout << i << "bullet" << matrix[x][y]<<" ";
		/*		int xup = ceil(x), xdown = floor(x), yup = ceil(y), ydown = floor(y);
				if (xup >= length) xup = length - 1;
				if (yup >= width) yup = width - 1;
				if (xdown < 0) xdown = 0;
				if (ydown < 0) ydown = 0;
				matrix[xup][yup] = -10;
				matrix[xup][ydown] = -10;
				matrix[xdown][yup] = -10;
				matrix[xdown][ydown] = -10;*/
		//}
	}
}

int RobotAI::check(int xUp,int xDown,int yUp,int yDown)
{
	//edit check function here
	int result = 0;
	for (int i = xUp; i < xDown; i++)
	{
		if (i < 0) continue;
		if (i >= length) break;
		for (int j = yUp; j < yDown; j++)
		{
			if (j < 0) continue;
			if (j >= length) break;
			if (matrix[i][j]>Min)
				result += matrix[i][j];
			else
			{
				if ((i <= xOb1 + obr&&i >= xOb1 - obr&&j <= yOb1 + obr&&j >= yOb1 - obr) || (i <= xOb2 + obr&&i >= xOb2 - obr&&j <= yOb2 + obr&&j >= yOb2 - obr))
				{
					result += matrix[i][j];
				}
			}
			//if (yDown <= 60 && matrix[i][j] != 0)
			//	cout << matrix[i][j] << " ";
		}
	}
	return result;
}
void RobotAI::goal(const RobotAI_BattlefieldInformation& info,int myID)
{
	
	int triggerNum = 1;
	int xGoal = info.arsenal[0].circle.x;
	int yGoal = info.arsenal[0].circle.y;
	int xNow = info.robotInformation[myID].circle.x;
	int yNow = info.robotInformation[myID].circle.y;
	int xE = info.robotInformation[(myID + 1) % 2].circle.x;
	int yE = info.robotInformation[(myID + 1) % 2].circle.y;
	if (yE == lastY&&xE == lastX)
	{
		superAttack++;
	}
	lastX = xE; lastY = yE;
	if (superAttack > 60)
	{
		xGoal = xE; yGoal = yE;
	
		//cout << "Attack!!" << " ";
		if (abs(xE - xNow) + abs(yE + yNow) <= 1000)
		{
			//run
		//	cout << "run" << endl;
			//special judgement here in case trapped at the corner
			xGoal = xNow + (xNow - xE);
			yGoal = yNow + (yNow - yE);
			if (xGoal < 0) xGoal = 0;
			if (xGoal >= length) xGoal = length - 1;
			if (yGoal<0) yGoal = 0;
			if (yGoal >= width) yGoal = width - 1;
			superAttack = 0;
		}
	}
	else
	{
		if (abs(xE - xNow) + abs(yE + yNow) <= 1000)
		{
			//run
			//special judgement here in case trapped at the corner
		//	cout << "run,too" << endl;
			xGoal = xNow + (xNow - xE);
			yGoal = yNow + (yNow - yE);
			if (xGoal < 0) xGoal = 0;
			if (xGoal >= length) xGoal = length - 1;
			if (yGoal<0) yGoal = 0;
			if (yGoal >= width) yGoal = width - 1;
			superAttack = 0;
		}
		else
		{
			if (info.arsenal[0].respawning_time > 10 || (abs(info.robotInformation[(myID + 1) % 2].circle.x - xGoal) + abs(info.robotInformation[(myID + 1) % 2].circle.y - yGoal) < 100))
			{

				xGoal = info.arsenal[1].circle.x;
				yGoal = info.arsenal[1].circle.y;
				if (abs(xE - xGoal) + abs(yE - yGoal) <= 100)
				{
					xGoal = info.arsenal[0].circle.x;
					yGoal = info.arsenal[0].circle.y;
				}
			}


			if (info.robotInformation[myID].remainingAmmo >= 2)
			{
				if (info.robotInformation[myID].cooling <= 0)
				{
					//		cout << "Attack" << " ";
					triggerNum = 2;
					int xO = info.robotInformation[(myID + 1) % 2].circle.x;
					int yO = info.robotInformation[(myID + 1) % 2].circle.y;
					double k1 = (yO - yNow) / (double)(xO - xNow);
					double b1 = yNow - k1*xNow;
					int tempY = k1 * 300 + b1;
					if (xO != xNow&&tempY <= 250 + 75 && tempY >= 250 - 75)
					{
						if (tempY < 250)
						{
							xGoal = xNow; yGoal = 50;
						}
						else
						{
							xGoal = xNow; yGoal = width - 50;
						}
					}
					tempY = k1 * 1066 + b1;
					if (xO != xNow&&tempY <= 430 + 75 && tempY >= 430 - 75)
					{
						if (tempY < 430)
						{
							xGoal = xNow; yGoal = 50;
						}
						else
						{
							xGoal = xNow; yGoal = width - 50;
						}
					}
					//		cout << xGoal << " " << yGoal << " ";
				}
				else
				{
					//			cout << "defend" << " ";
					triggerNum = 1;
					int obstacleX1 = 300, obstacleY1 = 250;
					int obstacleX2 = 1066, obstacleY2 = 430;
					int move1, move2;
					int x1, y1, x2, y2;
					int xO = info.robotInformation[(myID + 1) % 2].circle.x;
					int yO = info.robotInformation[(myID + 1) % 2].circle.y;
					double kt = (yO - obstacleY1) / (double)(xO - obstacleX1);
					double bt = yO - kt*xO;
					//doesn't consider the time when they are in a vertical line! could be improved later
					//doesn't consider what if the way is cut down by the obstacle,either
					if (abs(xO - xNow) < abs(xNow - obstacleX1))
					{
						y1 = yNow; x1 = (y1 - bt) / kt;
						move1 = abs(x1 - xNow);
						if (x1 >= length)
							move1 = 10000;
					}
					else
					{
						x1 = xNow; y1 = kt*x1 + bt;
						move1 = abs(y1 - yNow);
						if (y1 >= width)
							move1 = 10000;
					}
					kt = (yO - obstacleY2) / (double)(xO - obstacleX2);
					bt = yO - kt*xO;
					if (abs(xO - xNow) < abs(xNow - obstacleX2))
					{
						y2 = yNow; x2 = (y2 - bt) / kt;

						move2 = abs(x2 - xNow);
						if (x2 >= length)
							move2 = 10000;
					}
					else
					{
						x2 = xNow; y2 = kt*x2 + bt;
						move2 = abs(y2 - yNow);
						if (y2 >= width)
							move2 = 10000;
					}
					if (move1 < move2)
					{
						xGoal = x1; yGoal = y1;
						//       cout << "move1 " << move1 << " ";
					}
					else
					{
						xGoal = x2; yGoal = y2;
						//		cout << "move2" << move2 << " ";
					}
					//	cout << xGoal << " " << yGoal << endl;
				}
			}
		}
		
	}
		double k = (yGoal - yNow) / (double)(xGoal - xNow);
		double b = yNow - k*(xNow);
		//	cout << k << " " << b << endl;
		for (int i = 1; i <= previewNum; i++)
		{
			if (xGoal != xNow)
			{

				int x, y;
				if (xGoal < xNow) x = xNow - i;
				else x = xNow + i;
				if (xGoal == x) break;
				y = k*x + b;
				if (y < 0) y = 0;
				if (x >= length) x = length - 1;
				if (y >= width) y = width - 1;
			//	cout << x << " " << y << endl;
				matrix[x][y] += triggerNum;
			}
			else
			{
				if (yGoal>yNow)
					matrix[xNow][yNow + i] += triggerNum;
				else
					matrix[xNow][yNow - i] += triggerNum;
			}
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
	double temp;
	int tt = info.robotInformation[myID].remainingAmmo;
	VECTOR p;
	p = takeAim(order, info, myID);
	temp = p.y;
	Beam shootLine;
	shootLine.x = info.robotInformation[myID].circle.x;
	shootLine.y = info.robotInformation[myID].circle.y;
	shootLine.rotation = info.robotInformation[myID].weaponRotation;
	if (willShoot(info, myID, shootLine, p.x)) order.fire = 1;
	else order.fire = 0;
	if (start) order.fire = 0;
	start = false;
	
	//aim function
	int xNow = info.robotInformation[myID].circle.x;
	int yNow = info.robotInformation[myID].circle.y;
	int r = info.robotInformation[myID].circle.r+5;
	for (int i = 0; i < length;i++)
	for (int j = 0; j < width; j++)
		matrix[i][j] = 0;
	detectBullet(info, myID);
	goal(info, myID);
	int checkRec[5];
	//left right up down

	checkRec[1] = check(xNow - r - previewNum, xNow,yNow-r,yNow+r);

	checkRec[2] = check(xNow, xNow + r + previewNum, yNow - r, yNow + r);

	checkRec[3] = check(xNow - r, xNow + r, yNow-r-previewNum,yNow);

	checkRec[4] = check(xNow - r, xNow + r, yNow, yNow + r + previewNum);
	//y left equals to right!?
//	cout << endl;
	if (xNow <= 55)
		checkRec[1] = Min;
	if (xNow >= length - 55)
		checkRec[2] = Min;
	if (yNow <= 55)
		checkRec[3] = Min;
	if (yNow >= width - 55)
		checkRec[4] = Min;
//		cout << checkRec[1] << " ";
//		cout << checkRec[2] << " ";
//		cout << checkRec[3] << " ";
//		cout << checkRec[4] << " "<<endl;
	//cout << endl;
	int Ma = Min, rec = 0;
	for (int i = 1; i <= 4; i++)
	if (checkRec[i] > Ma)
	{

		rec = i; Ma = checkRec[i];
	}
	order.run = rec;
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

	weapon = WT_Prism;	//啊，我爱加农炮
	engine = ET_Quad;
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "利维坦二世";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "怪客二人组";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return -99;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return -12;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -75;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 0;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 117;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 0;
}









//-----------------------------------------------------
//3.用不用随你的触发函数
//-----------------------------------------------------


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	start = true;
	for (int i = 0; i < length;i++)
	for (int j = 0; j < width; j++)
	{
		matrix[i][j] = 0;
	}
	int t = 0;
	switch (info.robotInformation[(myID + 1) % 2].weaponTypeName)
	{
	case WT_Cannon:
		t = 11;
		break;
	case WT_Shotgun:
		t = 10; break;
	case WT_RPG:
		t = 9; break;
	case WT_Machinegun:
		t = 11; break;
	case WT_PlasmaTorch:
		t = 5; break;
	case WT_MissileLauncher:
		t = 5; break;
	case WT_GrenadeThrower:
		t = 6; break;
	case WT_MineLayer:
		t = 5; break;
	case WT_Apollo:
		t = 14; break;
	default:
		t = 5; break;
	}
	previewNum = 10 * t;
	xOb1 = info.obstacle[0].x;
	xOb2 = info.obstacle[1].x;
	yOb1 = info.obstacle[0].y;
	yOb2 = info.obstacle[1].y;
	obr = info.obstacle[0].r;
	superAttack = 0;
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



