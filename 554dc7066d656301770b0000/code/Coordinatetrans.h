#pragma once
#include <cmath>
#include <struct.h>
#include "struct.h"


//声明
double XAxisTransformation(double x);
double YAxisTransformation(double y);
double YAxisTransformation(double y);

//变换坐标系的函数:

//改变为以左下角为原点的右手系://函数安全
Circle CircleTransformation(Circle a){
	//cout << "以下是调用圆变换函数时的输出:" << endl;
	Circle temp;
	temp.x = XAxisTransformation(a.x);
	temp.y = YAxisTransformation(a.y);
	temp.r = a.r;
	//cout << "变换前:" <<"("<< a.x <<","<<a.y<<")"<< endl;
	//cout << "变换后:" << "(" << temp.x << "," << temp.y << ")" << endl;

	return temp;
}

//x轴坐标变换//安全
double XAxisTransformation(double x){
	return x;
}

//y轴坐标变换//安全
double YAxisTransformation(double y){
	return 680 - y;
}

//幅角变换//变更改掉-0
double AngelTransformation(double arg){
	//cout<< "以下是调用幅角变换函数时的输出:" << endl;
	//cout << "ex:" << arg << endl;
	//cout << "ed:" << -arg << endl;
	if (arg == 0)
		return 0;
	else
		return -arg;
}


//变换所有机甲信息;
RobotAI_RobotInformation RobotInfoTrans(const RobotAI_RobotInformation &info){
	RobotAI_RobotInformation temp;
	temp.entityID = info.entityID;
	temp.id = info.id;
	temp.weaponTypeName = info.weaponTypeName;
	temp.circle = CircleTransformation(info.circle);
	temp.engineRotation = AngelTransformation(info.engineRotation);
	temp.weaponRotation = AngelTransformation(info.weaponRotation);
	temp.vx = XAxisTransformation(info.vx);
	if (info.vy == 0){
		temp.vy = info.vy;
	}
	else {
		temp.vy = -info.vy;
		//cout << "Vy逆变换" << endl;
	}

	//在原坐标系下,Vr>0是顺时针旋转引擎,与右手螺旋不符.
	//去w>0时为逆时针旋转.故:
	//----------------------------------------------------这里似乎是有问题
	

	if (info.vr == 0){
		temp.vr = info.vr;
	}
	else {
		temp.vr = -info.vr;
		//cout << "Vr逆变换" << endl;

	}

	temp.hp = info.hp;
	temp.remainingAmmo = info.remainingAmmo;
	temp.cooling = info.cooling;
	return temp;
}



//变换所有子弹信息://检查过
RobotAI_BulletInformation BulletInfoTrans(const RobotAI_BulletInformation& info){
	RobotAI_BulletInformation temp;
	temp.entityID = info.entityID;
	temp.launcherID = info.launcherID;
	temp.type = info.type;
	temp.circle.x = XAxisTransformation(info.circle.x);
	temp.circle.y = YAxisTransformation(info.circle.y);
	temp.circle.r = info.circle.r;

	temp.rotation = AngelTransformation(info.rotation);
	temp.vx = XAxisTransformation(info.vx);
	if (info.vy == 0){
		temp.vy = info.vy;
	}
	else {
		temp.vy = -info.vy;
	}

	if (info.vr == 0){
		temp.vr = info.vr;
	}
	else {
		temp.vr = -info.vr;
	}
	
	temp.rotation = AngelTransformation(info.rotation);//存疑;
	return temp;
}



//变换所有障碍物信息使用圆变换


//变换所有军火库信息
RobotAI_ArsenalInformation AresenalInfoTrans(const RobotAI_ArsenalInformation&info){
	RobotAI_ArsenalInformation temp;
	temp.circle = CircleTransformation(info.circle);
	temp.respawning_time = info.respawning_time;
	return temp;
}

//Box类的变换:

Box BoxTrans(const Box & info){
	Box temp;
	temp.x = XAxisTransformation(info.x);
	temp.y = YAxisTransformation(info.y);
	temp.height = info.height;
	temp.width = info.width;
	return temp;
}


//战场信息的变换函数://更改了障碍物变换的错误
RobotAI_BattlefieldInformation FieldInfoTrans(const RobotAI_BattlefieldInformation& info){
	RobotAI_BattlefieldInformation temp;

	//机甲信息变换:
	temp.num_robot = info.num_robot;
	for (int i = 0; i < Info_MaxRobots; i++)
		temp.robotInformation[i] = RobotInfoTrans(info.robotInformation[i]);

	//子弹信息变换:
	temp.num_bullet = info.num_bullet;
	for (int i = 0; i < Info_MaxBullets; i++)
		temp.bulletInformation[i] = BulletInfoTrans(info.bulletInformation[i]);

	//障碍物信息变换:

	temp.num_obstacle = info.num_obstacle;
	for (int i = 0; i < Info_MaxObstacles; i++)
		temp.obstacle[i] = CircleTransformation(info.obstacle[i]);
	//cout << "障碍物1的坐标:(" << temp.obstacle[0].x << "," << temp.obstacle[0].y << ")" << endl;
	//cout << "障碍物2的坐标:(" << temp.obstacle[1].x << "," << temp.obstacle[1].y << ")" << endl;


	//军火库信息变换
	temp.num_arsenal = info.num_arsenal;
	for (int i = 0; i < Info_MaxArsenals; i++)
		temp.arsenal[i] = AresenalInfoTrans(info.arsenal[i]);

	//边界信息变换:
	temp.boundary = BoxTrans(info.boundary);
	return temp;

}
