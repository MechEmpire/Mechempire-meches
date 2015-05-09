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

//----------------------------行进控制函数初步-----------------

//----------------------------尝试各个引擎算法初步-------------

//----------------------------弱AI函数-------------------------
void ET_Spider_Circle(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, Inface&vset)
{
	//order.run = 1;
	//cout << "向x负向移动" << endl;
	//order.run = 2;
	//cout << "向x正向移动" << endl;
	//order.run = 3;
	//cout << "向y正向移动" << endl;
	//order.run = 4;
	//cout << "向y负向移动" << endl;
	
}



void ET_Robotman_Circle(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, Inface&vset){
	//order.run = 1;
	//cout << "向当前前方行动" << endl;
	//order.eturn = 1;
	//cout << "引擎顺时针旋转" << endl;

}


void ET_GhostTank_Circle(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, Inface&vset){
	order.run = 1;//向当前方向加速运动
	order.eturn = 1;
	cout << "当前移动速度是:" << endl;
	cout << "vx=" << info.robotInformation[vset.myID].vx << endl;
	cout << "vy=" << info.robotInformation[vset.myID].vy << endl;
	cout << "合速度:" << sqrt(info.robotInformation[vset.myID].vx*info.robotInformation[vset.myID].vx + info.robotInformation[vset.myID].vy*info.robotInformation[vset.myID].vy) << endl;
	
	cout << "vr=" << info.robotInformation[vset.myID].vr << endl;
	cout << "理论角速度:" << 3.5*sqrt(info.robotInformation[vset.myID].vx*info.robotInformation[vset.myID].vx + info.robotInformation[vset.myID].vy*info.robotInformation[vset.myID].vy) << endl;
}

void ET_Shuttle_Guard(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, Inface&vset){
	//引擎旋转顺时针旋转:
	if (n <= 45){
		order.eturn = 1;
	}
	else
		order.run = 1;
}