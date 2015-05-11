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
double getAngle(const RobotAI_BattlefieldInformation& info,int myID)
{
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
	
	double BV=bulletSpeed;
	EV_Vector.x = info.robotInformation[enermyID].vx;
	EV_Vector.y = info.robotInformation[enermyID].vy;
	double EV=sqrt(EV_Vector.x*EV_Vector.x+EV_Vector.y*EV_Vector.y);
	
	double cosA=-((ENERNMYPOINT.x-MYPOINT.x)*EV_Vector.x+(ENERNMYPOINT.y-MYPOINT.y)*EV_Vector.y)/(DIS*EV);
	
	double a=(EV*EV-BV*BV);
	double b=-2*DIS*EV*cosA;
	double c=DIS*DIS;
	double t=(-b-sqrt(b*b-4*a*c))/2/a;
	
	Shoot_Vector.x = ENERNMYPOINT.x - MYPOINT.x + t*EV_Vector.x;
	Shoot_Vector.y = ENERNMYPOINT.y - MYPOINT.y + t*EV_Vector.y;
	double angle;
	if (Shoot_Vector.x>=0) angle = RadianToAngle(atan(Shoot_Vector.y/Shoot_Vector.x));
	else angle = (RadianToAngle(atan(Shoot_Vector.y/Shoot_Vector.x))+180);
	angle = AngleAdjust(angle);
	
	return angle;
}



double takeAim(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{    
	double temp;
	temp = getAngle(info,myID);
	if (temp>0) order.wturn=1;
	if (temp<0) order.wturn=-1;
	if (info.robotInformation[myID].weaponRotation>temp) order.wturn=-1; else order.wturn=1;
	if ((info.robotInformation[myID].weaponRotation-temp)>180) order.wturn=1;
	if ((temp-info.robotInformation[myID].weaponRotation)>180) order.wturn=-1;
	return temp;
}

double dist(const Point&a, const Point& b)
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

bool willShoot(const RobotAI_BattlefieldInformation& info,int myID, Beam shootLine)
{
	int MMAX=2000;
	Point p1,p2,p3;
	double d1,d2,d3;
	Point myPoint,MYPOINT,ENERNMYPOINT;
	Circle future;
	future=info.robotInformation[myID^1].circle;
	MYPOINT.x = info.robotInformation[myID].circle.x;
	MYPOINT.y = info.robotInformation[myID].circle.y;
	ENERNMYPOINT.x = info.robotInformation[myID^1].circle.x;
	ENERNMYPOINT.y = info.robotInformation[myID^1].circle.y;
	double DIS;
	DIS = sqrt((MYPOINT.x-ENERNMYPOINT.x)*(MYPOINT.x-ENERNMYPOINT.x)+(MYPOINT.y-ENERNMYPOINT.y)*(MYPOINT.y-ENERNMYPOINT.y));
	double bulletSpeed;
	getSpeed(info,myID,bulletSpeed);
	double time=DIS/bulletSpeed;
	future.x+=time*info.robotInformation[myID^1].vx; 
	future.y+=time*info.robotInformation[myID^1].vy;
	myPoint.x=info.robotInformation[myID].circle.x;
	myPoint.y=info.robotInformation[myID].circle.y;
	bool b[2];
	b[0]=HitTestBeamCircle(shootLine,info.obstacle[0]);
	b[1]=HitTestBeamCircle(shootLine,info.obstacle[1]);
	if (!HitTestBeamCircle(shootLine,future)) return false;
	p1=GetHitPoint(shootLine,future);
	d1=dist(p1,myPoint);
	if (!b[0]&&!b[1]) return true;
	if (b[0]) {
		p2=GetHitPoint(shootLine,info.obstacle[0]); 
		d2=dist(p2,myPoint);
	}
	else d1=MMAX;
	if (b[1]) {
		p3=GetHitPoint(shootLine,info.obstacle[1]);
		d3=dist(p3,myPoint);
	}
	else d3=MMAX;
	if ((d1<d2)&&(d1<d3)) return true;
	else return false;
}
