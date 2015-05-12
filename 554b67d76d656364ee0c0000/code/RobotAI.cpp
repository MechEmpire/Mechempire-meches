#include "RobotAI.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <cstring>
#include <iostream>
RobotAI::RobotAI()
{
	
}


RobotAI::~RobotAI()
{

}
struct xxxx{
	xxxx(){
		srand((unsigned)time(0));
	}
}_xxx;

//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------
RobotAI_Order go(double x,double y,const RobotAI_BattlefieldInformation& info, int myID){
	RobotAI_Order order;
	auto me = info.robotInformation[myID];
	double dx = x - me.circle.x;
	double dy = y - me.circle.y;
	double theta = atan2(dy, dx)*180.0 / PI;
	//theta = AngleAdjust(theta);
	double mt =me.engineRotation;
	double dt = theta - mt;
	AngleAdjust(dt);
	//cout << "x=" << x << " y=  " << y << " mx = " << me.circle.x << " my=" << me.circle.y << " theta= " << theta << " mt = " << mt << " dt = " << dt << endl;
	const double eps = 1e-3;
	if (dt > eps){
		order.eturn = 1;
	}
	else if (dt < -eps){
		order.eturn = -1;
	}
	else{
		order.eturn = 0;
		
	}
	order.run = 1;
	//cout << "order.rturn=" << order.eturn << endl;
	return order;

}

double dis(double x1, double y1, double x2, double y2){
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}
double dis(const Circle& a, const Circle& b){
	return dis(a.x, a.y, b.x, b.y);
}
void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)

	auto& me = info.robotInformation[myID];
	auto& arsenal = info.arsenal;
	double dis0 = dis(me.circle.x, me.circle.y, arsenal[0].circle.x, arsenal[0].circle.y);
	double dis1 = dis(me.circle.x, me.circle.y, arsenal[1].circle.x, arsenal[1].circle.y);
	auto& target = info.robotInformation[1 - myID];
	int p, q;
	if (dis0 < dis1)
		p = 0, q = 1;
	else
		p = 1, q = 0;
	if (info.arsenal[p].respawning_time ==0 ){
		order = go(info.arsenal[p].circle.x, info.arsenal[p].circle.y, info, myID);
	}
	else if (info.arsenal[q].respawning_time == 0){
		order = go(info.arsenal[q].circle.x, info.arsenal[q].circle.y, info, myID);
	}else{
		order.run = 1;
		order.eturn = 1;

	}

	for (int i = 0; i < info.num_bullet; i++){
		const auto& bullet = info.bulletInformation[i];
		if (bullet.launcherID != myID){
			if (dis(me.circle.x, me.circle.y,bullet.circle.x,bullet.circle.y) < 60){
				order.run = 0;
			}
			
		}
	}


	static double mlx = me.circle.x, mly = me.circle.y;
	if (mlx == me.circle.x && mly == me.circle.y){
		order.run = -1;
		order.eturn = 1;
	}
	mlx = me.circle.x;
	mly = me.circle.y;

	double emdis = dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y);
	static double elx = target.circle.x, ely = target.circle.y;
	double eex = emdis/10.0 * (target.circle.x - elx) + target.circle.x;
	double eey = emdis/10.0 * (target.circle.y - ely) + target.circle.y;

	elx = target.circle.x;
	ely = target.circle.y;

	//order.wturn = 1;
	
	double the = atan2(eey - me.circle.y, eex - me.circle.x)*180.0 / PI;
	double wp = me.weaponRotation;

	double di = the - wp;
	AngleAdjust(di);
	//cout << the<<" "<<wp<<" "<< dis << endl;
	if (di>3.00){
		//cout << "1\n";
		order.wturn = 1;
	}
	else if (di<-3.0) {
		//cout << "-1\n";
		order.wturn = -1;
	}
	double miss = dis(me.circle.x, me.circle.y, eex, eey)*tan(di * PI / 180.0 / 2);
	//cout << miss << endl;
	if (miss < 10){
		//cout << "fire\n";
		order.fire = 1;
		bool blocked = false;
		const int TN = 10;
		//cout << me.circle.x << " " << me.circle.y << " " << eex << " " << eey << endl;
		for (int i = 0; i < TN; i++){
			double tx = me.circle.x + (eex - me.circle.x)*i / TN;
			double ty = me.circle.y + (eey - me.circle.y)*i / TN;
			for (int j = 0; j < info.num_obstacle; ++j){
				double ddd = dis(tx, ty, info.obstacle[j].x, info.obstacle[j].y);
				//cout <<j<<" "<<tx<<" "<<ty<<" "<< ddd << endl;
				if (ddd< info.obstacle[j].r+3)
					blocked = true;
			}
			if (blocked)
				break;
		}
		if (blocked){
			order.fire = 0;
		}

	}
	else{
		order.fire = 0;
	}

	
	
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

	weapon = WT_GrenadeThrower;
	engine = ET_UFO;
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "蓝色空间";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return " ";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return -50;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return -50;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -50;
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
	return 0;
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