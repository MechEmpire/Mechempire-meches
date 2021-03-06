#pragma once

#include "RobotAI_Interface.h"



class RobotAI:public RobotAI_Interface
{
protected:
	//TODO:可以在这里添加你自己的成员变量


public:

	//************************************************
	//这一段函数声明别乱动啊，否则机甲抛锚了别怪我

	RobotAI();
	virtual ~RobotAI();



	//-----------------------------------------------------
	//1.必须完成的战斗核心
	//-----------------------------------------------------

	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	virtual void Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID);



	//挑选装备函数
	//功能：在战斗开始时为你的机甲选择合适的武器炮塔和引擎载具
	//参数：weapon	...	代表你选择的武器，在函数体中给它赋值
	//		engine	...	代表你选择的引擎，在函数体中给它赋值
	//tip:	括号里的参数是枚举类型 weapontypename 或 enginetypename
	//		开发文档中有详细说明，你也可以在RobotAIstruct.h中直接找到它们的代码
	//tip:	最后一个bool是没用的。。那是一个退化的器官
	virtual void ChooseArmor(weapontypename& weapon,enginetypename& engine,bool);


	//-----------------------------------------------------






	//-----------------------------------------------------
	//2.个性信息
	//-----------------------------------------------------

	//返回你的机甲的名字
	virtual string GetName();

	//返回机甲制作人或团队的名字
	virtual string GetAuthor();



	//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
	//你可以在flash客户端的参数预览中预览颜色搭配的效果
	virtual int GetWeaponRed();
	virtual int GetWeaponGreen();
	virtual int GetWeaponBlue();

	//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
	//你可以在flash客户端的参数预览中预览颜色搭配的效果
	virtual int GetEngineRed();
	virtual int GetEngineGreen();
	virtual int GetEngineBlue();

	//-----------------------------------------------------




	//-----------------------------------------------------
	//3.用不用随你的触发函数
	//-----------------------------------------------------


	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	virtual void onBattleStart(const RobotAI_BattlefieldInformation& info,int myID);



	//一场战斗结束时被调用，可能可以用来析构你动态分配的内存空间（如果你用了的话）
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	virtual void onBattleEnd(const RobotAI_BattlefieldInformation& info,int myID);


	//有机甲开火时被调用
	//参数：fireID	... 开火的机甲下标
	virtual void onSomeoneFire(int);


	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
	virtual void onHit(int,bullettypename);

	//-------------------------------------------------------------



	//***************************************************************



	//TODO:可以在这里添加你自己的函数声明,并在RobotAI.cpp中编写相应的函数定义
	//针对蜘蛛坦克V3，躲避子弹用
	virtual int AvoidBullet(Circle bu, double vx, double vy, Circle myself);
	//针对战锤坦克，躲避子弹用
	virtual int AvoidBulletAFV(RobotAI_BulletInformation bu, RobotAI_RobotInformation me);
	//战锤坦克躲加农炮和阿波罗电磁炮
	virtual int AvoidCannonAFV(RobotAI_BulletInformation bu, RobotAI_RobotInformation me);

	//如果有子弹威胁到我
	virtual bool BulletShotMe(RobotAI_BulletInformation bu, RobotAI_RobotInformation me);

	//选择一个可以去的军火库
	virtual Circle whichArsenal(RobotAI_ArsenalInformation ar1, RobotAI_ArsenalInformation ar2, Circle me);

	//向敌人进攻！小蜘蛛
	virtual int runAndrun(Circle me,Circle armor,Circle obstacle[], int num_obs);
	//向敌人进攻！幽浮
	virtual int runAndrunAFV(Circle me,Circle armor,double engine_rotation);
	//遇到障碍物
	virtual int StrikeObstacle(Circle me, Circle obstacle[],const int num_obs);
	virtual int avoidObstacleAFV(RobotAI_RobotInformation me, Circle obstacle);
	//遇到墙
	virtual int avoidWall(RobotAI_RobotInformation me);
	//确定旋转角方向
	virtual double howToRotate(Circle me, Circle armor, double weapon_rotation, double vx, double vy);

	//旋转函数
	virtual int Rotate(double angle_to, double angle_now);
	//何时开炮？有障碍物的时候要节约子弹哦
	virtual int doIFire(Circle me, RobotAI_RobotInformation armor,Circle obstacle[],int num_obs,double now_angle, double fire_angle);

	//距离函数Distance
	virtual double Distance(double x1, double y1, double x2, double y2);


};