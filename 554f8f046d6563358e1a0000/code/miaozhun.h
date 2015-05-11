#pragma once
#include "struct.h"
#include <map>

struct VECTOR{
	double x;
	double y;
};

void getSpeed(const RobotAI_BattlefieldInformation& info,int myID,double& bulletSpeed)
{
	switch (info.robotInformation[myID].weaponTypeName){
	case WT_Cannon:          bulletSpeed=8;    break;
	case WT_Shotgun:         bulletSpeed=7.5;  break;
	case WT_RPG:             bulletSpeed=6;    break;
	case WT_Machinegun:      bulletSpeed=10;   break;
    case WT_Prism:           bulletSpeed=1000; break;
	case WT_Tesla:           bulletSpeed=1000; break;
	case WT_PlasmaTorch:     bulletSpeed=5;    break;
	case WT_MissileLauncher: bulletSpeed=4;    break;
	case WT_ElectricSaw:     bulletSpeed=1000; break;
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