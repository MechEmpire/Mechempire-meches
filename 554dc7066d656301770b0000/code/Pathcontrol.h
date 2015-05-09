#pragma once
#include "RobotAI.h"
#include "Vector.h"
#include "Coordinatetrans.h"
#include "Firecontrol.h"
#include "Frame.h"
#include <iostream>
#include "Interface.h"
using namespace std;

extern int n;

//----------------------------�н����ƺ�������-----------------

//----------------------------���Ը��������㷨����-------------

//----------------------------��AI����-------------------------
void ET_Spider_Circle(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, Inface&vset)
{
	//order.run = 1;
	//cout << "��x�����ƶ�" << endl;
	//order.run = 2;
	//cout << "��x�����ƶ�" << endl;
	//order.run = 3;
	//cout << "��y�����ƶ�" << endl;
	//order.run = 4;
	//cout << "��y�����ƶ�" << endl;
	
}



void ET_Robotman_Circle(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, Inface&vset){
	//order.run = 1;
	//cout << "��ǰǰ���ж�" << endl;
	//order.eturn = 1;
	//cout << "����˳ʱ����ת" << endl;

}


void ET_GhostTank_Circle(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, Inface&vset){
	order.run = 1;//��ǰ��������˶�
	order.eturn = 1;
	cout << "��ǰ�ƶ��ٶ���:" << endl;
	cout << "vx=" << info.robotInformation[vset.myID].vx << endl;
	cout << "vy=" << info.robotInformation[vset.myID].vy << endl;
	cout << "���ٶ�:" << sqrt(info.robotInformation[vset.myID].vx*info.robotInformation[vset.myID].vx + info.robotInformation[vset.myID].vy*info.robotInformation[vset.myID].vy) << endl;
	
	cout << "vr=" << info.robotInformation[vset.myID].vr << endl;
	cout << "���۽��ٶ�:" << 3.5*sqrt(info.robotInformation[vset.myID].vx*info.robotInformation[vset.myID].vx + info.robotInformation[vset.myID].vy*info.robotInformation[vset.myID].vy) << endl;
}

void ET_Shuttle_Guard(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, Inface&vset){
	//������ת˳ʱ����ת:
	if (n <= 45){
		order.eturn = 1;
	}
	else
		order.run = 1;
}