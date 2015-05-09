#pragma once
#include "RobotAI.h"
#include <iostream>
using namespace std;

extern int n;


//帧数控制函数:记录当前帧数,并且输出


void Frame(){
	n++;
	cout << "当前帧:" << n << "----------------------------------------------------------------"<<endl;
	return;
}


//在武器角度中缺少-180?






