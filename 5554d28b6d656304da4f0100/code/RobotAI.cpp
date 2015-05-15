#include "RobotAI.h"
#include<iostream>
#include<cmath>
RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

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

	armyE.x=info.robotInformation[army].circle.x;
	armyE.y=info.robotInformation[army].circle.y;
	
	selfE.x=info.robotInformation[myID].circle.x;
	selfE.y=info.robotInformation[myID].circle.y;

	army_point.x=info.robotInformation[army].circle.x;
	army_point.y=info.robotInformation[army].circle.y;
	
	self_point.x=info.robotInformation[myID].circle.x;
	self_point.y=info.robotInformation[myID].circle.y;


	
	//距离计算
	double distance=dis(selfE.x,armyE.x,selfE.y,armyE.y);
	
//	cout<<distance<<"  ";

//	cout<<info.robotInformation[myID].engineRotation<<endl;

	//障碍检测
	hitbyBarriersLU=HitTestCircles(selfE,barrierLU);
	hitbyBarriersRD=HitTestCircles(selfE,barrierRD);
	
	
      //角度计算
	angleofTwoEngine=sinangle(selfE,armyE);
	angleBetweenMeLU=sinangle(selfE,barrierLU);
	angleBetweenMeRD=sinangle(selfE,barrierRD);
	double angleoffire1=sinangle (selfE,info.arsenal[0].circle);
	double angleoffire2=sinangle (selfE,info.arsenal[1].circle );
		
//	cout<<"连线角度 "<<angleofTwoEngine<<"  ";
	
	angleW=info.robotInformation[myID].weaponRotation;
//	cout<<"武器角度 "<<angleW<<endl;

	angleE=info.robotInformation[myID].engineRotation;
	//	cout<<"引擎角度 "<<angleW<<endl;


	double sumangle=angleofTwoEngine+angleW;
	double chaangle=angleofTwoEngine-angleW;
//	cout<<"我方坐标"<<"("<<selfE.x<<","<<selfE.y<<")";
//	cout<<"敌方坐标"<<"("<<armyE.x<<","<<armyE.y<<")";


	//武器瞄准
	

	double sumangle2=angleofTwoEngine+angleE;
	double chaangle2=angleofTwoEngine-angleE;

	if (selfE.y>=armyE.y&&angleW>=0)
	{

//		cout<<"   我方在下  武器朝下";
		
			if (sumangle<=180&&sumangle>=0)
			{
				order.wturn=-1;
//				cout<<"逆时针转"<<endl;
				
			}	
			if (sumangle>=180&&sumangle<=360)
			{
				order.wturn=1;
//				cout<<"顺时针转"<<endl;
			}			
	}
	if (selfE.y>=armyE.y&&angleW<=0)
	{
	

//		cout<<"   我方在下  武器朝上 ";
			if (sumangle>=0)
			{
			order.wturn=-1;
//			cout<<"逆时针转"<<endl;
			}
			if (sumangle<=-1)
			{
				order.wturn=1;
//				cout<<"顺时针转"<<endl;
			}
			
	}


	if (selfE.y<=armyE.y&&angleW>=0)
	{

//		cout<<"   我方在上  武器朝下 ";
			if (chaangle>=0)
		{
			order.wturn=1;
//			cout<<"顺时针转"<<endl;
		}
		if (chaangle<=-1)
		{
			order.wturn=-1;
//			cout<<"逆时针转"<<endl;
		}

		
	}


	if (selfE.y<=armyE.y&&angleW<=0)
	{
//		cout<<"   我方在上  武器朝上 ";


			if (chaangle<180&&chaangle>=0)
		{
			order.wturn=1;
//			cout<<"顺时针转"<<endl;
		}
		if (chaangle>=180&&chaangle<=359)
		{
			order.wturn=-1;
//			cout<<"逆时针转"<<endl;
		}

		
	}



	//run

	if(selfE.x<barrierLU.x)
	{
		if (angleE<92&&angleE>88)
		{
			order.eturn=0;
			order.run=1;
		}
		else{
		order.run=0;
		if (angleE<90)
		{
			order.eturn=1;


		}
		if (angleE>90)
		{
				order.eturn=-1;
		}
		}

	}
	if (selfE.x>barrierRD.x)
	{
		if (angleE>-92&&angleE<-88)
		{
			order.eturn=0;
			order.run=1;
		}
		else
		{
			order.run=0;
		if (angleE<-90)
		{
			order.eturn=1;


		}
		if (angleE>-90)
		{
				order.eturn=-1;

		}
		}
		
	}
	
	
	//开火判定


	double k=(selfE.y-armyE.y)/(selfE.x-armyE.x);
	double b=(selfE.y-selfE.x*k);
	double lu=disLinePoint(k,b,barrierLU);
	double rd=disLinePoint(k,b,barrierRD);
	
	double effectiveshotRadian=asin(armyE.r/distance);
	double effectiveshotangle=RadianToAngle(effectiveshotRadian);
	
	
	


	if (info.robotInformation[myID].remainingAmmo>1)//有弹药
	{	
		if ((fabs(sumangle)<=effectiveshotangle||fabs(chaangle)<=effectiveshotangle))
		{
			if ((isTwoSide(self_point,army_point,barrierLU)&&lu<=barrierLU.r)||(isTwoSide(self_point,army_point,barrierRD)&&rd<=barrierRD.r))
			{
			order.fire=0;
			}
			else
			{
			
			order.fire=1;

			}

		
		}

	}
	else
	{
		
	
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

	//weapon = WT_Tesla;	
	//weapon=WT_PlasmaTorch;
	//weapon=WT_Cannon;
	weapon=WT_Prism;
	//weapon=WT_Apollo;
	//weapon=WT_Shotgun;
	//engine = ET_UFO;	
	engine=ET_Shuttle;
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "疯狂的试验品";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "bardon";
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
	army=1-myID;
	cout<<"I "<<myID<<endl;
	cout<<"army "<<army<<endl;
	armyE.r=info.robotInformation[army].circle.r;
	selfE.r=info.robotInformation[myID].circle.r;
	cout<<"my radus="<<selfE.r<<endl;
	cout<<"army's radus="<<armyE.r<<endl;

	//初始化障碍物信息
	barrierLU.x=300;
	barrierLU.y=250;
	barrierLU.r=75;

	barrierRD.x=1066;
	barrierRD.y=430;
	barrierRD.r=75;

	count=0;

}

void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗结束时被调用，可能可以用来析构你动态分配的内存空间（如果你用了的话）
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//system("pause");
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
bool RobotAI::HitTestCircles(Circle &c1, const Circle &c2)
{
	double xx=c1.x-c2.x,	yy=c1.y-c2.y;
	double rr=c1.r+c2.r;

	if(abs(xx)>(rr) || abs(yy)>(rr))
	{
		return false;
	}

	double dis2=xx*xx+yy*yy;
	
	return ((rr*rr)>=dis2);
}



	double RobotAI::dis(double  x1,double x2,double y1,double y2)
	{
		double xx=x1-x2;
		double yy=y1-y2;
		return sqrt(xx*xx+yy*yy);
	}


	double RobotAI::sinangle(Circle &self,const Circle& other)
	{

		Point c;
		c.x=self.x+10;
		c.y=self.y;
		double lena=sqrt(pow(armyE.x-c.x,2)+pow(armyE.y-c.y,2));
	
		double lenb=10;
		double lenc=dis(other.x,self.x,other.y,self.y);

		double YY=pow((other.y-self.y),2);

		double S2=sqrt(YY)*10;
		double sina=S2/lenb/lenc;

	double angle=RadianToAngle(asin(sina));

	if (self.x>other.x)
		{
			angle=180-angle;//angleofTwoEngine当机甲处于右边时该怎么算？180-angle还是360-angle?
		}

	return angle;
	}


	void  RobotAI::TurnToPoint(const Circle &self,const Circle &other,double angleofTwoEngine,double angle,RobotAI_Order& order)
	{
		double sumangle=angleofTwoEngine+angle;
		double chaangle=angleofTwoEngine-angle;
		
	if (self.y>=other.y&&angle>=0)
	{

//		cout<<"   我方在下  武器朝下";
		
			if (sumangle<=180&&sumangle>=0)
			{
				order.wturn=-1;
//				cout<<"逆时针转"<<endl;
				
			}	
			if (sumangle>=180&&sumangle<=360)
			{
				order.wturn=1;
//				cout<<"顺时针转"<<endl;
			}			
	}
	if (self.y>=other.y&&angle<=0)
	{
	

//		cout<<"   我方在下  武器朝上 ";
			if (sumangle>=0)
			{
			order.wturn=-1;
//			cout<<"逆时针转"<<endl;
			}
			if (sumangle<=-1)
			{
				order.wturn=1;
//				cout<<"顺时针转"<<endl;
			}
			
	}


	if (self.y<=other.y&&angle>=0)
	{

//		cout<<"   我方在上  武器朝下 ";
			if (chaangle>=0)
		{
			order.wturn=1;
//			cout<<"顺时针转"<<endl;
		}
		if (chaangle<=-1)
		{
			order.wturn=-1;
//			cout<<"逆时针转"<<endl;
		}

		
	}


	if (self.y<=other.y&&angle<=0)
	{
//		cout<<"   我方在上  武器朝上 ";


			if (chaangle<180&&chaangle>=0)
		{
			order.wturn=1;
//			cout<<"顺时针转"<<endl;
		}
		if (chaangle>=180&&chaangle<=359)
		{
			order.wturn=-1;
//			cout<<"逆时针转"<<endl;
		}

		
	}


	}


	double RobotAI::disLinePoint(double &k,double& b,const Circle& heart)
	{
		return fabs(k*heart.x+b-heart.y)/sqrt(k*k+1);
	}

	bool RobotAI::isTwoSide(Point &left,Point &right,const Circle &middle)

	{
		int row=(middle.x-left.x)*(middle.x-right.x);
		int col=(middle.y-left.y)*(middle.y-right.y);
		if (row<=0&&col<=0)
		{
			return true;
		}
		else 
			return false;
	}