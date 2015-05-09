#pragma once

#include <string>
#include <vector>
#include "Vector2D.h"
//#include "../sys/RobotAI_Interface.h"
#include "RobotAI_Interface.h"

#define MY battle_info.robotInformation[myID]
#define ENEMY battle_info.robotInformation[enemyID]



struct BulletDisSort
{
	int idx;
	double dissq;

	BulletDisSort(){idx=0;dissq=1e20;}
};



class RobotAI:public RobotAI_Interface
{
protected:
	//TODO:可以在这里添加你自己的成员变量
	vector<BulletDisSort> bullet_sort;
	double crusher_death_rush;	//绞肉机死亡冲刺距离
	int tmp_state;	//临时用来有限状态机


	Vector2D vec_me;
	Vector2D vec_enemy;


	double min_distance;		//距离误差限
	double min_rotation;		//角度误差限

	double danger_distance;		//危险距离
	//const double min_distance=4;		//距离误差限
	//const double min_rotation=1;		//角度误差限
	
	
	int myID;
	int enemyID;	//敌人ID（两人局）

	RobotAI_BattlefieldInformation battle_info;	//战场信息，方便访问，每帧更新

	//arsenal
	Vector2D vec_arsenal0,vec_arsenal1;

	//temp
	bool moving;

	int to_point;
	vector<Vector2D> wandering_point;	//巡逻点

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

	

	//----------底层基本动作---------------------------

	void UpdateBattlefieldInfo(const RobotAI_BattlefieldInformation& info);
	void UpdateBulletInfoVector();
	int NearestBulletIdx(int);

	//***直接操控***
	void Fire(RobotAI_Order&);	//开火
	bool AimAtPoint(RobotAI_Order&,const double &x,const double &y);	//瞄准点（x,y)


	void DirectMoveToPoint(RobotAI_Order&order,const double &x,const double &y);	//直接移动到点(x,y)...
	void DirectMoveAwayFromPoint(RobotAI_Order&order,const double &x,const double &y);	//直接离开点(x,y)



	void DirectMoveToPoint_Spider(RobotAI_Order&,const double &x,const double&y);	//直接移动到点(x,y)...蜘蛛机甲子方法
	void DirectMoveToPoint_Robotman(RobotAI_Order&,const double &x,const double&y);	//直接移动到点(x,y)...毁灭者
	void DirectMoveToPoint_AFV(RobotAI_Order&,const double &x,const double&y);	//直接移动到点(x,y)...战锤坦克
	void DirectMoveToPoint_UFO(RobotAI_Order&,const double &x,const double&y);	//直接移动到点(x,y)...幽浮
	

	void DirectMoveInDirection(RobotAI_Order& order, const double & r);		//沿特定方向（角度）移动
	
	void DirectMoveInDirection_Spider(RobotAI_Order& order, const double & r);
	void DirectMoveInDirection_Robotman(RobotAI_Order& order, const double & r);
	void DirectMoveInDirection_AFV(RobotAI_Order& order, const double & r);
	void DirectMoveInDirection_UFO(RobotAI_Order& order, const double & r);



	void Attack_ElectricSaw(RobotAI_Order&order);


	//载具朝向目标点
	//为坦克服务，搞一个角度参数?
	int AdjustDirection(const double &x,const double &y);



	void DirectMoveAwayFromPoint_Spider(RobotAI_Order&order,const double &x,const double &y);	//直接离开点(x,y)...蜘蛛机甲
	

	//沿法线方向逃离射线
	void EscapeFromBeam(RobotAI_Order&order,const Beam &b);

	//************


	//稍微高层一点的


	//看自己有没有在子弹的飞行路径上
	//return -1--是
	//return 0,1,2... 先打到第i个障碍上
	//return other ... 不在子弹射线路径上
	int HitTestBulletBeam(const RobotAI_BulletInformation& bi);


	void PursuitEnemy(RobotAI_Order & order);

	//带预判的瞄准瞄准
	void AimWithAnticipation(RobotAI_Order & order,const double & x,const double & y);


	//跟踪弹躲障碍后边时的瞄准
	void Aim_Missile(RobotAI_Order & order);


	//在指定障碍后躲避hunter
	Vector2D GetHidingPosition(const Circle & o,const Circle & hunter);




	//大策略

	void Hide(RobotAI_Order & order,const Circle & hunter);


	//寻求去军火库补子弹
	void Reload(RobotAI_Order & order);	//去补子弹

	//用障碍物躲子弹
	void HideAndApproach(RobotAI_Order & order);	

	//使用离对方近的障碍物躲避逼近
	void SafeApproach(RobotAI_Order& order);

	//沿点路径行进
	void PathFollow(RobotAI_Order& order);




	//***判断*****************************************
	bool IsArriveAtPoint(const double &x, const double &y);	//判断是否到达点(x,y);

	//是不是敌人的子弹
	bool IsEnemyBullet(int bullet_idx);

	//敌人现在的剩余子弹能不能干掉我
	bool IsSafeToRush();


	//对普通直线子弹武器，是否开火路径畅通，是否已瞄准
	bool IsOkToFire();


	//判断是否被敌人瞄准了
	bool IsBeAimed();

	//************





	void AddWanderPoint(Vector2D v);



	//***
	//导航图
	void ConstructNavGraph();	//根据battle_info创建导航图

	bool ConstructNavGraph_ValidPosition(const Vector2D &);

	//返回距离最近的导航点
	int NavGraph_NearestNode(const Vector2D &);








	//猥琐系的状态execute方法
	void Scared_HideAndFlee(RobotAI_Order & order);
	void Scared_Fire(RobotAI_Order & order);


	//绞肉机的状态execute方法
	
	void Crusher_Attack(RobotAI_Order & order);
	
	//绞肉机的状态转换判别
	void Crusher_StateTransform();



};


