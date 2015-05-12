#include "RobotAI.h"
#include <math.h>
#include <cmath>
#define Con 80
//#include "GlobalFunction.cpp"

RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}


//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------

#if 0
void SetSeed__()
{
	srand((unsigned)time(0) + 7);
}

int cover(int num, int zhishu);
double random0_1();
int random(int minv, int maxv);



void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)

	//order.fire = 1;
	//order.wturn = 1;

	static long int time = 0;
	time++;

	int x0 = info.robotInformation[myID].circle.x;
	int y0 = info.robotInformation[myID].circle.y;
	int r0 = info.robotInformation[myID].circle.r;
	int x1 = info.robotInformation[1 - myID].circle.x;
	int y1 = info.robotInformation[1 - myID].circle.y;
	int r1 = info.robotInformation[1 - myID].circle.r;
	if (myID == 0) { ; }
	else { x0 = 1066 - x0; x1 = 1066 - x1; y0 = 680 - y0; y1 = 680 - y1; }
	char locate0,locate1;
	int wheel=1;
	if (sqrt(cover((x1 - 1366), 2) + cover((y1 - 680), 2))<=600 && (y1 >= 100)) { locate1 = 'E'; }
	else if (sqrt(cover((x1 - 1366), 2) + cover((y1 - 680), 2)) > 600 && (y1 < 100) && (x1>766)) { locate1 = 'D'; }
	else if (sqrt(cover((x1), 2) + cover((y1), 2)) <= 600 && (y1 <= 580)) { locate1 = 'A'; }
	else if (sqrt(cover((x1), 2) + cover((y1), 2)) > 600 && (y1 > 580)&&(x1<600)) { locate1 = 'B'; }
	else { locate1 = 'C'; }
	//enermy location
	if (sqrt(cover((x0 - 1366), 2) + cover((y0 - 680), 2)) <= 600 && (y0 >= 100)) { locate0 = 'E'; }
	else if (sqrt(cover((x0 - 1366), 2) + cover((y0 - 680), 2)) > 600 && (y0 < 100) && (x0>766)) { locate0 = 'D'; }
	else if (sqrt(cover((x0), 2) + cover((y0), 2)) <= 600 && (y0 <= 580)) { locate0 = 'A'; }
	else if (sqrt(cover((x0), 2) + cover((y0), 2)) > 600 && (y0 > 580)&&(x0<600)) { locate0 = 'B'; }
	else { locate0 = 'C'; }
	//self location
	double angle_l0l1 = (y1 - y0) / (x1 - x0);
	double angle_l0b1 = (250 - y0) / (300 - x0);
	double angle_l0b2 = (430 - y0) / (1066 - x0);
	double angle_l1b1 = (250 - y1) / (300 - x1);
	double angle_l1b2 = (430 - y1) / (1066 - x1);
	double r_l0l1 = sqrt(cover((y1 - y0), 2) + cover(x1 - x0, 2));  //helen3
	double r_l0b1 = sqrt(cover((y0 - 250), 2) + cover(x0 - 300, 2));  //helen1
	double r_l0b2 = sqrt(cover((y0 - 430), 2) + cover(x0 - 1066, 2));  
	double r_l1b1 = sqrt(cover((y1 - 250), 2) + cover(x1 - 300, 2));  //helen2
	double r_l1b2 = sqrt(cover((y1 - 430), 2) + cover(x1 - 1066, 2));
	double p_middle = (r_l0l1 + r_l0b1 + r_l1b1) / 3;

	double k0 = atan2((y1 - y0) , (x1 - x0))*180/PI;
	if (wheel == 1)
	{ 
		if (r_l0b1 > (r0 + 75))
		{
			long int time4random = time;
			if (int((time4random % Con)) % 4 == 0) 
			{
				int is2B = random(1, 4);
				order.run = is2B;
			}
			else
			{
				if ((angle_l0b1 >= 1) && (x0 < 300) && (y0 < 250)) { order.run = 4; };
				if ((angle_l0b1 < 1) && (x0 < 300) && (y0 < 250)) { order.run = 2; };
				if ((angle_l0b1 >= -1) && (x0 >= 300) && (y0 < 250)) { order.run = 4; };
				if ((angle_l0b1 < -1) && (x0 >= 300) && (y0 < 250)) { order.run = 1; };
				if ((angle_l0b1 >= -1) && (x0 < 300) && (y0 >= 250)) { order.run = 3; };
				if ((angle_l0b1 < -1) && (x0 < 300) && (y0 >= 250)) { order.run = 2; };
				if ((angle_l0b1 >= 1) && (x0 >= 300) && (y0 >= 250)) { order.run = 3; };
				if ((angle_l0b1 < 1) && (x0 >= 300) && (y0 >= 250)) { order.run = 1; };
			}
		}
		if ((info.robotInformation[myID].remainingAmmo == 0)&&(info.num_arsenal != 2)/*&&(info.robotInformation[myID].cooling>=10)*/) { wheel = 3; }
//		if ((r_l0b1-(r0 + 75))<=5) { wheel = 2; }                       //1=starting hide   2=hide from enermy opposite burden1  3=bullets!
	} 
/*
	if (wheel == 2)
	{
		if ((angle_l1b1 < angle_l0b1)&&(angle_l1b1>=1)) { order.run = 3; }
		if ((angle_l1b1 < angle_l0b1)&&(angle_l1b1<1)) { order.run = 3; }
		if ((angle_l1b1 > angle_l0b1)&&(angle_l1b1)>=1) { order.run = 2; }
		if ((angle_l1b1 > angle_l0b1)&&(angle_l1b1<1)) { order.run = 2; }
		wheel = 1;
	}
	*/


//go and get ammo
	if (wheel == 3)
	{

	/*	int YYNisSB= random0_1();
		if (YYNisSB > (1 - YYNisSB)) { order.run = 4; }
		else { order.run = 1; }  */
		if (time % 2 == 0) { order.run = 4; }
		else { order.run = 1; };
		if ((info.robotInformation[myID].remainingAmmo != 0)||(info.num_arsenal!=2)) { wheel = 1; }
	}
	//Helen Fomula initiating
	double s4helen = sqrt(p_middle*(p_middle-r_l1b1)*(p_middle-r_l0b1)*(p_middle-r_l0l1));
	double dist4helen = 2 * s4helen / r_l0l1;
//shooting
    static int Firenot = 1;
    if (abs(info.robotInformation[myID].weaponRotation - k0) > 0.01)
	{
		if ((info.robotInformation[myID].weaponRotation - k0) > 0)
		{
			order.wturn = -1;
		}
		else
		{
			order.wturn = 1;
		}

	}
	if (dist4helen <= 75) { Firenot = 0; }
	else { Firenot = 1; };
	if(Firenot==1)order.fire = 1;
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

	weapon = WT_PlasmaTorch;	
	engine = ET_Spider;	
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "'or '1'='1 alpha";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "Henry Jee";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return -255;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return -50;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return 255;
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
}


void RobotAI::onHit(int launcherID,bullettypename btn)
{
	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
}



//TODO:这里可以实现你自己的函数


int cover(int num, int zhishu)
{
	int ans = 1;
	for (int i = 1; i <= zhishu; i++)
		ans *= num;
	return(ans);
}

enum wheel
{hide,hid};

double random0_1() {
	//产生0-1的随机数的函数
	int temp;
	double r;
	temp = rand() % 1000;
	r = (double)temp / 1000;
	return r;
}

int random(int minv, int maxv)
{
	int gap = maxv - minv;
	if (maxv <= minv)
	{
		return 0;
	}
	return (minv + rand() % (gap + 1));

}

#else
int cover(int num, int zhishu);
int move(int xx, int yy, int xx_, int yy_); int check0(int a);
void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)

	//order.fire = 1;
	//order.wturn = 1;
	
	static long int time = 0;
	time++;

	int x0 = info.robotInformation[myID].circle.x;
	int y0 = info.robotInformation[myID].circle.y;
	int r0 = info.robotInformation[myID].circle.r;
	int x1 = info.robotInformation[1 - myID].circle.x;
	int y1 = info.robotInformation[1 - myID].circle.y;
	int r1 = info.robotInformation[1 - myID].circle.r;
	char locate0, locate1;
	int wheel = 1;
	int bl_1x = 300, bl_1y = 250, bl_2x = 1066, bl_2y = 430;
	if (sqrt(cover((x1 - 1366), 2) + cover((y1 - 680), 2))<=600 && (y1 >= 100)) { locate1 = 'E'; }
	else if (sqrt(cover((x1 - 1366), 2) + cover((y1 - 680), 2)) > 600 && (y1 < 100) && (x1>766)) { locate1 = 'D'; }
	else if (sqrt(cover((x1), 2) + cover((y1), 2)) <= 300 && (y1 <= 580)) { locate1 = 'A'; }
	else if (sqrt(cover((x1), 2) + cover((y1), 2)) > 300 && (y1 > 580)&&(x1<300)) { locate1 = 'B'; }
	else { locate1 = 'C'; }
	//enermy location
	if (sqrt(cover((x0 - 1366), 2) + cover((y0 - 680), 2)) <= 600 && (y0 >= 100)) { locate0 = 'E'; }
	else if (sqrt(cover((x0 - 1366), 2) + cover((y0 - 680), 2)) > 600 && (y0 < 100) && (x0>766)) { locate0 = 'D'; }
	else if (sqrt(cover((x0), 2) + cover((y0), 2)) <= 300 && (y0 <= 580)) { locate0 = 'A'; }
	else if (sqrt(cover((x0), 2) + cover((y0), 2)) > 300 && (y0 > 580)&&(x0<300)) { locate0 = 'B'; }
	else { locate0 = 'C'; }
	//self location   
	double angle_l0l1 = (y1 - y0) / (x1 - x0);
	double angle_l0b1 = (250 - y0) / (300 - x0);
	double angle_l0b2 = (430 - y0) / (1066 - x0);
	double angle_l1b1 = (250 - y1) / (300 - x1);
	double angle_l1b2 = (430 - y1) / (1066 - x1);
//	double angle_fucking = (y1-250);
	double r_l0l1 = sqrt(cover((y1 - y0), 2) + cover(x1 - x0, 2));
	double r_l0b1 = sqrt(cover((y0 - 250), 2) + cover(x0 - 300, 2));
	double r_l0b2 = sqrt(cover((y0 - 430), 2) + cover(x0 - 1066, 2));
	double r_l1b1 = sqrt(cover((y1 - 250), 2) + cover(x1 - 300, 2));
	double r_l1b2 = sqrt(cover((y1 - 430), 2) + cover(x1 - 1066, 2));
	double k0 = atan2((y1 - y0), (x1 - x0)) * 180 / PI;
	static int switch4timec1 = 150;
    if (myID == 0) { ; }
	else { x0 = 1066 - x0; x1 = 1066 - x1; y0 = 680 - y0; y1 = 680 - y1; }
	/*if (locate1 == 'A')
	{
			if ((angle_l0l1 >= 1) && (x0 < x1) && (y0 < y1)) { order.run = 4; };
			if ((angle_l0l1 < 1) && (x0 < x1) && (y0 < y1)) { order.run = 2; };
			if ((angle_l0l1 >= -1) && (x0 >= x1) && (y0 < y1)) { order.run = 4; };
			if ((angle_l0l1 < -1) && (x0 >= x1) && (y0 < y1)) { order.run = 1; };
			if ((angle_l0l1 >= -1) && (x0 < x1) && (y0 >= y1)) { order.run = 3; };
			if ((angle_l0l1 < -1) && (x0 < x1) && (y0 >= y1)) { order.run = 2; };
			if ((angle_l0l1 >= 1) && (x0 >= x1) && (y0 >= y1)) { order.run = 3; };
			if ((angle_l0l1 < 1) && (x0 >= x1) && (y0 >= y1)) { order.run = 1; };
	}
	else
	{
		//100;
		if (switch4timec1<100)
		{
			if ((angle_l0b1 > 1) && (y0 <= 250)) { order.run = 3; }
			if ((angle_l0b1 <= 1) && (x0 <= 300)) { order.run = 1; }
			if ((angle_l0b1 > 1) && (y0>250)) { order.run = 4; }
			if ((angle_l0b1 <= 1) && (x0>300)) { order.run = 2; }

		}  
		else  
		{
			if ((angle_l0b1 >= 1) && (x0 < 300) && (y0 < 250)) { order.run = 4; };
			if ((angle_l0b1 < 1) && (x0 < 300) && (y0 < 250)) { order.run = 2; };
			if ((angle_l0b1 >= -1) && (x0 >= 300) && (y0 < 250)) { order.run = 4; };
			if ((angle_l0b1 < -1) && (x0 >= 300) && (y0 < 250)) { order.run = 1; };
			if ((angle_l0b1 >= -1) && (x0 < 300) && (y0 >= 250)) { order.run = 3; };
			if ((angle_l0b1 < -1) && (x0 < 300) && (y0 >= 250)) { order.run = 2; };
			if ((angle_l0b1 >= 1) && (x0 >= 300) && (y0 >= 250)) { order.run = 3; };
			if ((angle_l0b1 < 1) && (x0 >= 300) && (y0 >= 250)) { order.run = 1; };
			switch4timec1 = 100;
		}
		if ((r_l0b1 - 75 - r0) <= 10) { switch4timec1 = 0; switch4timec1++; }
		else { switch4timec1 = 100; }
	}
	*/

	//model:order.run=move(x0,y0,destination_x,destination_y)
//	order.run = move(x0, y0, x1, y1);
	static int x0t = 0, y0t = 0;
	if ((x1<=600)||((x1<=900)&&(y1<300))/*locate1=='A'*/) {	order.run = move(x0, y0, x1, y1);}
	else 
	{
		double middle1_ = (y1 - 250)*(y1 - 250) + (x1 - 300)*(x1 - 300);
		double middle2_ = (75 / check0(middle1_))*(x1 - 300);
		double middle3_ = (75 / check0(middle1_))*(y1 - 250);
		x0t = 300 - middle2_-75;
		y0t = 250 - middle3_-75;
		//order.run = move(x0, y0, x0t, y0t);
		if (switch4timec1 < 100)
		{
			if (x1*(abs(y1 - 250)) / check0((abs(x1 - 300))) >= 0) { y1 = x1*abs((y1 - 250)) / check0(abs((x1 - 300))); x0t = 0; }
			else { x0t = y1*abs((x1 - 300)) / abs((y1 - 250)); y0t = 0; }
			order.run = move(x0, y0, x0t, y0t);  

		}
		else
		{
			order.run = move(x0, y0, x0t, y0t);
		}
		if ((r_l0b1 - 75 - r0) <= 60) { switch4timec1--; }
		else { switch4timec1 = 150; }

	}

	if (abs(info.robotInformation[myID].weaponRotation - k0) > 0.01)
	{
			if ((info.robotInformation[myID].weaponRotation - k0) > 0)
			{
				order.wturn = -1;
			}
			else
			{
				order.wturn = 1;
			}

		if (r_l0l1 <= (r0 + r1 + 10)) 
		{
			order.fire = 1;
		}


		
	}

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

	weapon = WT_ElectricSaw;
	engine = ET_Spider;
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "Cree9ySaw";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "YYNisSB";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return -255;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return -50;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return 0;
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


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info, int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标

}

void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info, int myID)
{
	//一场战斗结束时被调用，可能可以用来析构你动态分配的内存空间（如果你用了的话）
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
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



//TODO:这里可以实现你自己的函数


int cover(int num, int zhishu)
{
	int ans = 1;
	for (int i = 1; i <= zhishu; i++)
		ans *= num;
	return(ans);
}


int move(int xx, int yy,int xx_,int yy_)
{  
	double angle_l0t1_ = (yy_ - yy) / (xx_ - xx);
	if ((angle_l0t1_ >= 1) && (xx < xx_) && (yy < yy_)) { return 4; };
	if ((angle_l0t1_ < 1) && (xx < xx_) && (yy < yy_)) { return 2; };
	if ((angle_l0t1_ >= -1) && (xx >= xx_) && (yy < yy_)) {return 4; };
	if ((angle_l0t1_ < -1) && (xx >= xx_) && (yy < yy_)) { return 1; };
	if ((angle_l0t1_ >= -1) && (xx < xx_) && (yy >= yy_)) { return 3; };
	if ((angle_l0t1_ < -1) && (xx < xx_) && (yy >= yy_)) { return 2; };
	if ((angle_l0t1_ >= 1) && (xx >= xx_) && (yy >= yy_)) { return 3; };
	if ((angle_l0t1_ < 1) && (xx >= xx_) && (yy >= yy_)) { return 1; };
}

int check0(int a)
{
	if (a == 0) { return 1; }
	else { return a; }
}

#endif