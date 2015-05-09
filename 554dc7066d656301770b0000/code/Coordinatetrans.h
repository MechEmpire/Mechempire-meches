#pragma once
#include <cmath>
#include <struct.h>
#include "struct.h"


//����
double XAxisTransformation(double x);
double YAxisTransformation(double y);
double YAxisTransformation(double y);

//�任����ϵ�ĺ���:

//�ı�Ϊ�����½�Ϊԭ�������ϵ://������ȫ
Circle CircleTransformation(Circle a){
	//cout << "�����ǵ���Բ�任����ʱ�����:" << endl;
	Circle temp;
	temp.x = XAxisTransformation(a.x);
	temp.y = YAxisTransformation(a.y);
	temp.r = a.r;
	//cout << "�任ǰ:" <<"("<< a.x <<","<<a.y<<")"<< endl;
	//cout << "�任��:" << "(" << temp.x << "," << temp.y << ")" << endl;

	return temp;
}

//x������任//��ȫ
double XAxisTransformation(double x){
	return x;
}

//y������任//��ȫ
double YAxisTransformation(double y){
	return 680 - y;
}

//���Ǳ任//����ĵ�-0
double AngelTransformation(double arg){
	//cout<< "�����ǵ��÷��Ǳ任����ʱ�����:" << endl;
	//cout << "ex:" << arg << endl;
	//cout << "ed:" << -arg << endl;
	if (arg == 0)
		return 0;
	else
		return -arg;
}


//�任���л�����Ϣ;
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
		//cout << "Vy��任" << endl;
	}

	//��ԭ����ϵ��,Vr>0��˳ʱ����ת����,��������������.
	//ȥw>0ʱΪ��ʱ����ת.��:
	//----------------------------------------------------�����ƺ���������
	

	if (info.vr == 0){
		temp.vr = info.vr;
	}
	else {
		temp.vr = -info.vr;
		//cout << "Vr��任" << endl;

	}

	temp.hp = info.hp;
	temp.remainingAmmo = info.remainingAmmo;
	temp.cooling = info.cooling;
	return temp;
}



//�任�����ӵ���Ϣ://����
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
	
	temp.rotation = AngelTransformation(info.rotation);//����;
	return temp;
}



//�任�����ϰ�����Ϣʹ��Բ�任


//�任���о������Ϣ
RobotAI_ArsenalInformation AresenalInfoTrans(const RobotAI_ArsenalInformation&info){
	RobotAI_ArsenalInformation temp;
	temp.circle = CircleTransformation(info.circle);
	temp.respawning_time = info.respawning_time;
	return temp;
}

//Box��ı任:

Box BoxTrans(const Box & info){
	Box temp;
	temp.x = XAxisTransformation(info.x);
	temp.y = YAxisTransformation(info.y);
	temp.height = info.height;
	temp.width = info.width;
	return temp;
}


//ս����Ϣ�ı任����://�������ϰ���任�Ĵ���
RobotAI_BattlefieldInformation FieldInfoTrans(const RobotAI_BattlefieldInformation& info){
	RobotAI_BattlefieldInformation temp;

	//������Ϣ�任:
	temp.num_robot = info.num_robot;
	for (int i = 0; i < Info_MaxRobots; i++)
		temp.robotInformation[i] = RobotInfoTrans(info.robotInformation[i]);

	//�ӵ���Ϣ�任:
	temp.num_bullet = info.num_bullet;
	for (int i = 0; i < Info_MaxBullets; i++)
		temp.bulletInformation[i] = BulletInfoTrans(info.bulletInformation[i]);

	//�ϰ�����Ϣ�任:

	temp.num_obstacle = info.num_obstacle;
	for (int i = 0; i < Info_MaxObstacles; i++)
		temp.obstacle[i] = CircleTransformation(info.obstacle[i]);
	//cout << "�ϰ���1������:(" << temp.obstacle[0].x << "," << temp.obstacle[0].y << ")" << endl;
	//cout << "�ϰ���2������:(" << temp.obstacle[1].x << "," << temp.obstacle[1].y << ")" << endl;


	//�������Ϣ�任
	temp.num_arsenal = info.num_arsenal;
	for (int i = 0; i < Info_MaxArsenals; i++)
		temp.arsenal[i] = AresenalInfoTrans(info.arsenal[i]);

	//�߽���Ϣ�任:
	temp.boundary = BoxTrans(info.boundary);
	return temp;

}
