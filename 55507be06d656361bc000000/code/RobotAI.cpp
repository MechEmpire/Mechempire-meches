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
void RobotAI::detectBullet(const RobotAI_BattlefieldInformation& info, int myID)
{
	int harm = 120;

	for (int i = 0; i < length; i++)
	for (int j = 0; j < width; j++)
		matrix[i][j] = 0;
	for (int i = 300 - 75; i <= 300 + 75; i++)
	for (int j = 250 - 75; j <= 250 + 75; j++)
		matrix[i][j] = Min;
	for (int i = 1066 - 75; i <= 1066 + 75; i++)
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
		matrix[x][y] += -harm;
		if (abs(xNow - x) < R&&abs(yNow - y) < R)
		{
			double k = temp.vy / (double)temp.vx;
			double b = y - k*x; int aX = xNow, bY = yNow;
			for (int i = 1; i < 10; i++)
			{
				if (x < xNow)
					aX = xNow + i;
				else
					aX = xNow - i;
				bY = aX*k + b;
				matrix[aX][bY] += -3;
			}
		}
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
	int xE = info.robotInformation[(myID + 1) % 2].circle.x;
	int yE = info.robotInformation[(myID + 1) % 2].circle.y;
	int R = info.robotInformation[(myID + 1) % 2].circle.r + 25;
	for (int i = xE - R; i <= xE + R; i++)
	{
		if (i < 0) continue;
		if (i >= length) break;
		for (int j = yE - R; j <= yE + R; j++)
		{
			if (j < 0) continue;
			if (j >= length) break;
			//		if (info.robotInformation[(myID + 1) % 2].weaponTypename == WT_ElectricSaw)
			//			matrix[i][j] += Min;
			//		else
			//			matrix[i][j] += -200;
		}
	}
}

int RobotAI::check(int xUp, int xDown, int yUp, int yDown)
{	
	//edit check function here
	int result = 0;
	for (int i = xUp; i < xDown; i++)
	{
		if (i < 50) continue;
		if (i >= length-50) break;
		for (int j = yUp; j < yDown; j++)
		{
			if (j < 50) continue;
			if (j >= length-50) break;
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
void RobotAI::setGoal()
{
	//cout << "goal "<<xGoal << " " << yGoal;
	if (xGoal == xNow&&yGoal == yNow) return;
	double k = (yGoal - yNow) / (double)(xGoal - xNow);
	double b = yNow - k*(xNow);
	//	cout << k << " " << b << endl;
	for (int i = 1; i <= previewNum; i++)
	{
		int ob = 0;
		if (xGoal != xNow)
		{

			int x, y;
			if (xGoal < xNow) x = xNow - i;
			else x = xNow + i;
			if (xGoal == x) break;
			y = k*x + b;
			if (y < 0 || x < 0) break;
			if (x >= length || y >= width) break;
			//cout << x << " " << y << endl;
			if (sqrt(pow(x - xOb1, 2) + pow(y - yOb1, 2)<=obr))
			{
				ob = 1; break;
			}
			else if (sqrt(pow(x - xOb2, 2) + pow(y - yOb2, 2) <= obr)){
				ob = 2; break;
			}
			matrix[x][y] += triggerNum;
		}
		else
		{
			//cout << "vertical  ";
			if (yGoal > yNow)
			{
				if (sqrt(pow(xNow - xOb1, 2) + pow(yNow+i - yOb1, 2) <= obr))
				{
					ob = 1; break;
				}
				else if (sqrt(pow(xNow - xOb2, 2) + pow(yNow +i- yOb2, 2) <= obr)){
					ob = 2; break;
				}
				if (yNow + i < width) matrix[xNow][yNow + i] += triggerNum;

				else break;
			}
			else
			{
				if (sqrt(pow(xNow - xOb1, 2) + pow(yNow - i - yOb1, 2) <= obr))
				{
					ob = 1; break;
				}
				else if (sqrt(pow(xNow - xOb2, 2) + pow(yNow - i - yOb2, 2) <= obr)){
					ob = 2; break;
				}
				if (yNow - i >= 0)	{
					matrix[xNow][yNow - i] += triggerNum;// cout << yNow - i << " ";
				}
				else break;
			}
		}
	}
	if (obr)
	{
		if (obr == 1)
		{
			for (int i = 0; i < previewNum; i++)
			{
				if (xNow - i < 0 || yNow - i < 0)break;
				matrix[xNow - i][yNow - i] += triggerNum;

			}
		}
		else
		{
			for (int i = 0; i < previewNum; i++)
			{
				if (xNow + i >= length || yNow + i >= length) break;
				matrix[xNow + i][yNow + i] += triggerNum;
			}
		}
	}
	
}
void RobotAI::goal(const RobotAI_BattlefieldInformation& info, int myID)
{

	triggerNum = 1;
	int xE = info.robotInformation[(myID + 1) % 2].circle.x;
	int yE = info.robotInformation[(myID + 1) % 2].circle.y;

	int xA1 = info.arsenal[0].circle.x, yA1 = info.arsenal[0].circle.y;
	int xA2 = info.arsenal[1].circle.x, yA2 = info.arsenal[1].circle.y;
	if (abs(xNow - xA1) + abs(yNow - yA1) < abs(xNow - xA2) + abs(yNow - yA2))
	{
		//cout << "First arsenal locked.  ";
		xGoal = xA1; yGoal = yA1;
		if (abs(abs(xE - xGoal) + abs(yE - yGoal) <= 500) )
		{
			xGoal = xA2; yGoal = yA2;
			//cout << "Second arsenal locked due to enemy's appearance. ";
		}
		if (info.arsenal[0].respawning_time>50)
		{
			if (info.arsenal[1].respawning_time < info.arsenal[0].respawning_time&&abs(xE - xA2) + abs(yE - yA2) >= 500)
			{
			//	cout << "Second aresenal locked due to respawning time. ";
				xGoal = xA2; yGoal = yA2;
			}
		}
	}
	else
	{
		xGoal = xA2; yGoal = yA2;
		if (abs(abs(xE - xGoal) + abs(yE - yGoal) <= 500))
		{
			xGoal = xA1; yGoal = yA1;
		}
		if (info.arsenal[1].respawning_time>50)
		{
			if (info.arsenal[0].respawning_time < info.arsenal[0].respawning_time&&abs(xE - xA1) + abs(yE - yA1) >= 500)
			{
				xGoal = xA1; yGoal = yA1;
			}
		}
	}



	/*if (info.arsenal[0].respawning_time > 10 || (abs(xE - xGoal) +abs(yE - yGoal) < 400))
	{

		xGoal = info.arsenal[1].circle.x;
		yGoal = info.arsenal[1].circle.y;
		if (abs(xE - xGoal) + abs(yE - yGoal) <= 500 || ((abs(xE - info.arsenal[0].circle.x) + abs(yE - info.arsenal[0].circle.y) >400) && abs(xNow - info.arsenal[0].circle.x) + abs(yNow - info.arsenal[0].circle.y)<abs(xNow-xGoal)+abs(yNow-yGoal)))
		{
			xGoal = info.arsenal[0].circle.x;
			yGoal = info.arsenal[0].circle.y;
		}
	}*/
	triggerNum = 2;
	if (info.robotInformation[myID].remainingAmmo <= 2) {
		//cout << xGoal << " " << yGoal << endl;
		setGoal(); return;
	}
	bool attackAllowed = true;
	if (abs(xE - xNow) + abs(yE - yNow) <= 600||info.robotInformation[(myID+1)%2].weaponTypeName==WT_ElectricSaw )
	{
		attackAllowed = false;
		//run stratagy here
		double k = info.robotInformation[(myID + 1) % 2].vy / (double)info.robotInformation[(myID + 1) % 2].vx;
		int pre = 10;
		int xENext = xE + info.robotInformation[(myID + 1) % 2].vx*pre, yENext = yE + info.robotInformation[(myID + 1) % 2].vy*pre;
		if (info.robotInformation[(myID + 1) % 2].vx == 0)
			k = 1000000 * info.robotInformation[(myID + 1) % 2].vy;
		double b = yE - k*xE;
		double k1 = -1 / k;  double b1 = yNow - k1*xNow;
	/*	int testX = xNow - 4*pre, testY = k1*xNow + b1;
		if (k == 0) {
			testX = xNow; testY = yNow - 4 * pre;
		}
		bool po = false;
		int dist1 = sqrt(pow(testX - xENext, 2) + pow(testY - yENext, 2));
		//int dist = sqrt(pow(xNow - xENext, 2) + pow(yNow - yENext, 2));
		if (testY-150 >= 0 && testY+150<width&&xNow-150>0&&dist1>R+160) {
			xGoal = testX; yGoal = testY; po = true;
			cout << "Back avoid. "<<dist1<<" ";
		}
		int testX2 = xNow + 4*pre; int testY2 = k1*testX + b1;
		if (k == 0)
		{
			testX2 = xNow; testY2 = yNow + 4 * pre;
		}
		if (testY2 -150>= 0 && testY2+150 < width&&testX2+150 < length)
		{
			int dis2=sqrt(pow(testX2 - xENext, 2) + pow(testY2 - yENext, 2));
			if (sqrt(pow(xE - testX2, 2) + pow(yE - testY2, 2)) > sqrt(pow(xE - testX, 2) + pow(yE - testY, 2))&&dis2>=R+160)
			{
				xGoal = testX2; yGoal = testY2; po = true;
				cout << "Forward avoid. "<<dis2<<" ";
			}
		}
		if (!po)
		{*/
		//cout << "Traditional way failed. ";
			int cmp[4] = { 0 };//left right up down

			if (xNow - 300 >= 0)
			{
				//compare each of them
				cmp[0] = abs(xNow - 4 * pre - xENext) + abs(yNow - yENext);
				if (k<-1 || k>1) cmp[0] += 10;
			}
			else
			{
				cmp[0] = -10000;
			}
			if (xNow + 300 < length)
			{
				cmp[1] = abs(xNow + 4 * pre - xENext) + abs(yNow - yENext);
				if (k<-1 || k>1) cmp[1] += 10;
			}
			else cmp[1] = -10000;
			if (yNow - 300 >= 0)
			{
				cmp[2] = abs(xNow - xENext) + abs(yNow - 4 * pre - yENext);
				if (-1 <= k&&k <= 1) cmp[2] += 10;
			}
			else cmp[2] = -10000;
			if (yNow + 300 < width)
			{
				cmp[3] = abs(xNow - xENext) + abs(yNow + 4 * pre - yENext);
				if (-1 <= k&&k <= 1) cmp[3] += 10;
			}
			else cmp[3] = -10000;
			int largest = 0, cho = -1;
			for (int i = 0; i < 4; i++)
			{
				//cout << cmp[i] << " ";
				if (cmp[i] >= largest)
				{
					largest = cmp[i];
					cho = i;
					
				}
			}
			//if (num<=200)
			//	cout << cho << " "<<largest<<" ";
			switch (cho)
			{
			case 0: xGoal = xNow - 100; yGoal = yNow; break;
			case 1: xGoal = xNow + 100; yGoal = yNow; break;
			case 2:xGoal = xNow; yGoal = yNow - 100; break;
			case 3:xGoal = xNow; yGoal = yNow + 100; break;
			}
		//	cout << xGoal << " " << yGoal;
		//}
		triggerNum = 2;
		//cout << xGoal << " " << yGoal << endl;
		setGoal();
		//cout << endl;
	}
	if (info.robotInformation[myID].remainingAmmo >= 2)
	{
		if (info.robotInformation[myID].cooling <= 0&&attackAllowed)
		{
			//cout << "Attack" << " ";
			triggerNum = 1;
			double k1 = (yE - yNow) / (double)(xE - xNow);
			double b1 = yNow - k1*xNow;
			int tempY = k1 * xOb1 + b1;
			if (xE != xNow&&tempY <= yOb1 + obr && tempY >= yOb1 - obr)
			{
				if (tempY < yOb1)
				{
					xGoal = xNow; yGoal = 50;
				}
				else
				{
					xGoal = xNow; yGoal = width - 50;
				}
			}
			tempY = k1 * xOb2 + b1;
			if (xE != xNow&&tempY <= 430 + 75 && tempY >= 430 - 75)
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
		//	cout << xGoal << " " << yGoal << " ";
		}
		else
		{
		//	cout << "defend" << " ";
			triggerNum = 1;
			int move1, move2;
			int x1, y1, x2, y2;
			int xO = info.robotInformation[(myID + 1) % 2].circle.x;
			int yO = info.robotInformation[(myID + 1) % 2].circle.y;
			
			double kt = (yO - yOb1) / (double)(xO - xOb1);
			double bt = yO - kt*xO;
			//doesn't consider the time when they are in a vertical line! could be improved later
			//doesn't consider what if the way is cut down by the obstacle,either
			if (abs(xO - xNow) < abs(xNow - xOb1))
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
			/*if (xO > xOb1)
			{
				x1 = xOb1 - obr - 10;
			}
			else
			{
				x1 = xOb1 + obr + 10;
			}
			y1 = kt*x1 + bt;
			move1 = abs(x1 - xNow) + abs(y1 - yNow);
			if (y1 >= width)
			{
				y1 = width - 50; move1 = 10000;
			}
			if (y2 <= 0)
			{
				y2 = 50; move2 = 10000;
			}
			kt = (yO - yOb2) / (double)(xO - xOb2);
			bt = yO - kt*xO;
			/*if (abs(xO - xNow) < abs(xNow - xOb2))
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
			if (xO > xOb2)
				x2 = xOb2 - obr - 10;
			else
				x2 = xOb2 + obr + 10;
			y2 = x2*kt + bt;
			move2 = abs(xNow - x2) + abs(yNow - y2);
			if (y2 >= width)
			{
				y2 = width - 50; move2 = 10000;
			}
			if (y2 < 0)
			{
				y2 = 50; move2 = 10000;
			}*/
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
				
		}
	}
	//cout << xGoal << " " << yGoal<< endl;
	//cout << "defendSet"; 
	setGoal();





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


void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	double temp;
	int tt = info.robotInformation[myID].remainingAmmo;
	xNow = info.robotInformation[myID].circle.x;
	yNow = info.robotInformation[myID].circle.y;
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
	
	int r = info.robotInformation[myID].circle.r + 5;
	for (int i = 0; i < length; i++)
	for (int j = 0; j < width; j++)
		matrix[i][j] = 0;
	detectBullet(info, myID);
	goal(info, myID);
	int checkRec[5];
	//left right up down
	//cout << "Yes" << endl;
	checkRec[1] = check(xNow - r - previewNum, xNow, yNow - r, yNow + r);

	checkRec[2] = check(xNow, xNow + r + previewNum, yNow - r, yNow + r);

	checkRec[3] = check(xNow - r, xNow + r, yNow - r - previewNum, yNow);

	checkRec[4] = check(xNow - r, xNow + r, yNow, yNow + r + previewNum);

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
	{
		//cout << checkRec[i] << " ";
		if (checkRec[i] > Ma)
		{

			rec = i; Ma = checkRec[i];
		}
	}
	order.run = rec;
/*	num++;
	if (num < 15)
	{
		if (xNow < 500)
			order.run = 4;
		else
			order.run = 3;
	}*/
	//if(num<200) cout << Ma<<" "<<rec << endl;
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

	weapon = WT_Prism;
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


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info, int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//freopen("report", "w", stdout);
	start = true; num = 0;
	for (int i = 0; i < length; i++)
	for (int j = 0; j < width; j++)
	{
		matrix[i][j] = 0;
	}
	int t = 0;
	switch (info.robotInformation[(myID + 1) % 2].weaponTypeName)
	{
	case WT_Cannon:
		previewNum = 110;
		break;
	case WT_Shotgun:
		previewNum = 110; break;
	case WT_RPG:
		previewNum = 100; break;
	case WT_Machinegun:
		previewNum = 120; break;
	case WT_PlasmaTorch:
		previewNum = 50; break;
	case WT_MissileLauncher:
		previewNum = 100; break;
	case WT_GrenadeThrower:
		previewNum = 70; break;
	case WT_MineLayer:
		previewNum = 50; break;
	case WT_Apollo:
		previewNum = 110; break;
	default:
		previewNum = 50; break;
	}
	
	xOb1 = info.obstacle[0].x;
	xOb2 = info.obstacle[1].x;
	yOb1 = info.obstacle[0].y;
	yOb2 = info.obstacle[1].y;
	obr = info.obstacle[0].r+10;
	xNow = info.robotInformation[myID].circle.x;
	yNow = info.robotInformation[myID].circle.y;
	R = info.robotInformation[myID].circle.r;
//	superAttack = 0;
}

void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info, int myID)
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


void RobotAI::onHit(int launcherID, bullettypename btn)
{
	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
}



