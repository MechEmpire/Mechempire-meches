﻿#include "RobotAI.h"
  static int x=0;
  static int countt=0;
RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////

double distance(double x1, double y1, double x2, double y2) //计算与敌人的距离
{
  return sqrt((x2-x1)*(x2-x1)+(y2- y1)*(y2-y1));
}

//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------


void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 auto& me = info.robotInformation[myID];
  auto& target = info.robotInformation[1 - myID];
  static double mcx = me.circle.x;
  static double mcy = me.circle.y;
  static double tcx = target.circle.x;
  static double tcy = target.circle.y;
  double dis0 = distance(me.circle.x, me.circle.y, 300, 250);
  double dis1 = distance(me.circle.x, me.circle.y, 1066, 430);
  double jdu1= atan2( - me.circle.y, tcx- me.circle.x)*180.0 / PI;
  
   if(distance(me.circle.x,me.circle.y,target.circle.x,target.circle.y)<210)
  {
    order.fire = 1;
	order.fire = 1;
	order.fire = 1;
	order.fire = 1;
  }

 if(x==0)
 {
  if(dis0<dis1)//冲向上边的障碍物
  {
	  order.run=1;
	  countt++;
	  jdu1=atan2(175-me.circle.y, 50-me.circle.x)*180.0 / PI;
	 order.run=1;
  double going1=jdu1-me.engineRotation;   //引擎角度判别
	 order.run=1;
  AngleAdjust(going1);
	  if(going1>2.00){
		  order.run=1;
		  order.eturn=1;
	  }
	  else if(going1<-2.00){

		  order.run=1;
		  order.eturn=-1;
	  }
	  if(-20<me.circle.y-170&&me.circle.y-170<20)
		  x++;
  }
  if(countt==0) //冲向下边的障碍物
  {
	double jdu2=atan2(430-me.circle.y, 1316- me.circle.x)*180.0 / PI;
	order.run=1;
	double going2=jdu2-me.engineRotation;   //引擎角度判别
	 order.run=1;
  AngleAdjust(going2);
	  if(going2>2.00){
		  order.run=1;
		  order.eturn=1;
	  }
	  else if(going2<-2.00){

		  order.run=1;
		  order.eturn=-1;
	  }
	  if(-20<me.circle.y-430&&me.circle.y-430<20)
		  x++;
  }
 }

  if(distance(me.circle.x,me.circle.y,target.circle.x,target.circle.y)<210)
  {
    order.fire = 1;
	order.fire = 1;
	order.fire = 1;
	order.fire = 1;
	order.fire = 1;
  }

  order.run=1;
  mcx = me.circle.x;
  mcy = me.circle.y;
  tcx = target.circle.x;
  tcy = target.circle.y;
  double emdistance = distance(me.circle.x, me.circle.y, target.circle.x, target.circle.y);
  double jdu= atan2(tcy - me.circle.y, tcx- me.circle.x)*180.0 / PI;

  //distance(me.circle.x,me.circle.y,target.circle.x,target.circle.y)<300)//狂追猛操
  if(x!=0)
  {
	 order.run=1;
  double yqin=me.engineRotation;//引擎角度 单位为度
	 order.run=1;
  double going=jdu-yqin;   //引擎角度判别
	 order.run=1;
  AngleAdjust(going);
	  if(going>2.00){
		  order.run=1;
		  order.eturn=1;
	  }
	  else if(going<-2.00){

		  order.run=1;
		  order.eturn=-1;
	  }
  }
  double wp=me.weaponRotation;//武器角度 单位为度
  double wq=jdu-wp;//武器角度判别
  AngleAdjust(wq);
  if(wq>3.00){
    order.wturn = 1;
  }
  else if (wq<-3.00) {
    order.wturn = -1;
  } 
  if(distance(me.circle.x,me.circle.y,target.circle.x,target.circle.y)<200)
  {
    order.fire = 1;
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

	weapon = WT_ElectricSaw   ;	//啊，我爱大电锯
	engine = ET_GhostTank ;	//啊，我爱幽灵坦克
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "无耻小螃蟹";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "small bear";
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
	return 10;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -128;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 20;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return -255;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return -158;
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