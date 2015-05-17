#include "RobotAI.h"
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
RobotAI_Order go(double x,double y,const RobotAI_BattlefieldInformation& info,int myID,double jiaodu)//到达XY点的行进函数
{
	RobotAI_Order order;
	auto& me=info.robotInformation[myID];
	double myeng=me.engineRotation;
	double gogo=jiaodu-myeng;
	AngleAdjust(gogo);
	order.run=1;
	if(gogo>2.00)
	{
		order.eturn=1;
	}
	else if(gogo<-2.00)
	{
		order.eturn=-1;
	}
	else order.eturn=0;
	order.run=1;
	return order;

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
  auto& target = info.robotInformation[1-myID];
  auto& arsenal0 =info.arsenal[0].circle;
  auto& arsenal1 =info.arsenal[1].circle; 
  auto&	obstacle0=info.obstacle[0];//障碍物0
  auto& obstacle1=info.obstacle[1];//障碍物1
  static double mcx = me.circle.x;
  static double mcy = me.circle.y;
  static double tcx = target.circle.x;
  static double tcy = target.circle.y;
  double dis0 = distance(me.circle.x, me.circle.y,arsenal0.x,arsenal0.y);//我与军火库0的距离
  double dis1 = distance(me.circle.x, me.circle.y,arsenal1.x,arsenal1.y);//我与军火库1的距离
  double distance0=distance(me.circle.x,me.circle.y,obstacle0.x,obstacle0.y);//我与障碍物0的距离
  double distance1=distance(me.circle.x,me.circle.y,obstacle1.x,obstacle1.y);//我与障碍物1的距离
  mcx = me.circle.x;//我的X坐标
  mcy = me.circle.y;//我的Y坐标
  tcx = target.circle.x;//敌人的X坐标
  tcy = target.circle.y;//敌人的Y坐标
  double emdistance = distance(me.circle.x, me.circle.y, target.circle.x, target.circle.y);//我与敌人的距离
  double jdu= atan2(tcy-me.circle.y,tcx- me.circle.x)*180.0/PI;//与敌方的角度
  double jud0=atan2(obstacle0.y-me.circle.y,obstacle0.x-me.circle.x)*180.0/PI;//与障碍物0中心的角度
  double jud1=atan2(obstacle1.y-me.circle.y,obstacle1.x-me.circle.x)*180.0/PI;//与障碍物1中心的角度
  double jiaodu0=atan2(arsenal0.y-me.circle.y,arsenal0.x-me.circle.x)*180.0/PI;//与军火库0中心的角度
  double jiaodu1=atan2(arsenal1.y-me.circle.y,arsenal1.x-me.circle.x)*180.0/PI;//与军火库1中心的角度
  double jj0=atan2(0-me.circle.y,0-me.circle.x)*180/PI;//我与上面出生点的角度
  double jj1=atan2(630-me.circle.y,1316-me.circle.x)*180/PI;//我与下面出生点的角度
  double Lanucher=info.bulletInformation[1-myID].launcherID; 
  double yqin=me.engineRotation;//引擎角度 单位为度
  double going=jdu-yqin;   //引擎角度判别
  static int count=rand();
  static int flag=0;
  static int time0=rand();
  static int time1=rand();
  static int MT=rand();
  if(flag==0)//判断出生点
  {
	 if(distance0<distance1)//出生点在上方;
	  count=0;
	 else      //出生点在下方
	  count=1;
	 flag=1;    //出生点判断完毕
  }
  if(count==0)
  {
	order=go(arsenal0.x,arsenal0.y,info,myID,jiaodu0);//出生点在上方去军火库0
	if(me.circle.x==50&&me.circle.y==630)//用于判断是否去过军火库0
		 time0=0;
  }
  else if(count==1)//出生点在下方去军火库1
  {
	  order=go(arsenal1.x,arsenal1.y,info,myID,jiaodu1);
	  if(me.circle.x==1316&&me.circle.y==50)//用于判断是否去过军火库1
		 time1=1;
  }  
  if(time1==1)
  {
    order = go(info.arsenal[0].circle.x, info.arsenal[0].circle.y, info, myID,jiaodu0);//去过军火库1后前往军火库0
	if(me.circle.x==50&&me.circle.y==630)
			MT=0;
  }
  if(time0==0)
  {
    order = go(info.arsenal[1].circle.x, info.arsenal[1].circle.y, info, myID,jiaodu1);//去过军火库0后前往军火库1
	if(me.circle.x==1316&&me.circle.y==50)
			MT=1;
  }
  if(MT==0)//围绕军火库0旋转
  {
	  order.run=1;
	  order=go(obstacle0.x,obstacle0.y,info,myID,jud0);
	  if(distance0<130&&info.robotInformation[1-myID].remainingAmmo>2)
	  {
		  double T=jud0-yqin+90;
		  if(T>2.00)
		  {
			  order.run=1;
			  order.eturn=1;
		  }
		  else if(T<-2.00)
		  {
			  order.run=1;
			  order.eturn=-1;
		  }
	  }
  }
  if(MT==1)//围绕军火库1旋转
  {
	  order.run=1;
	  order=go(obstacle1.x,obstacle1.y,info,myID,jud1);
	  if(distance1<130&&info.robotInformation[1-myID].remainingAmmo>2)
	  {
		    if(distance1<130&&info.robotInformation[1-myID].remainingAmmo>2)
		{
				  double F=jud1-yqin+90;
				  if(F>2.00)
				 {
				  order.run=1;
				  order.eturn=1;
				  }
				  else if(F<-2.00)
				  {
					  order.run=1;
					  order.eturn=-1;
				  }
		}
			 
	  }
  }
 //开火
	  if(emdistance<180)
	  order.fire=1;

  //武器瞄准
	 double mw=me.weaponRotation;//武器角度 单位为度 
	 double wq=jdu-mw;//武器角度判别
	 AngleAdjust(wq);
	 if(wq>2.00){
	  order.wturn = 1;
	 }
	 else if (wq<-2.00) 
	 {
	   order.wturn = -1;
	 } 
  //冲向敌人
  
 
  AngleAdjust(going);
  if(emdistance<400&&info.robotInformation[1-myID].remainingAmmo<=2)
  {
	  if(going>2.00){
		  order.run=1;
		  order.eturn=1;
	  }
	  else if(going<-2.00){
		  order.run=1;
		  order.eturn=-1; 
	  }
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

	weapon =WT_ElectricSaw  ;	//啊，我爱大电锯
	engine = ET_GhostTank ;	//啊，我爱幽灵坦克
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "寒冰诱惑";
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
	return -18;
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
	return -255;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return -18;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 255;
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