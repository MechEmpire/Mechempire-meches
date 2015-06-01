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



	


	
	
	//开火判定


	double k=(selfE.y-armyE.y)/(selfE.x-armyE.x);
	double b=(selfE.y-selfE.x*k);
	double lu=fabs(k*barrierLU.x+b-barrierLU.y)/sqrt(k*k+1);
	double rd=fabs(k*barrierRD.x+b-barrierRD.y)/sqrt(k*k+1);
	double case2plu=(barrierLU.x-selfE.x)*(barrierLU.x-armyE.x);
	double case2prd=(barrierRD.x-selfE.x)*(barrierRD.x-armyE.x);

	if (distance<=850+armyE.r&&(fabs(sumangle)<=5||fabs(chaangle)<=5))
	{
		if ((case2plu<0&&lu<=barrierLU.r)||(case2prd<0&&rd<=barrierRD.r))
		{
			order.fire=0;
			order.run=0;
		}
		else
		{
			order.fire=1;
			order.run=0;
		}

		
	}




		int bul_id;
	double disbe=10000;
	double tempdis;
	bool hide=false;
	
	for (int i = 0; i < info.num_bullet; i++)
	{
		const auto& bul= info.bulletInformation[i];
		if (bul.launcherID==army)
		{
			hide=true;
			tempdis=dis(selfE.x, bul.circle.x,selfE.y,bul.circle.y) ;
			if (tempdis<disbe)
			{
				disbe=tempdis;
				bul_id=i;
			}
		}
		
	}

	const auto& bullet= info.bulletInformation[bul_id];
		//不是自己的子弹//子弹很近

	if (hide&&disbe< 200&&info.robotInformation[myID].remainingAmmo>0)
		{		
			double k_bul=180*bullet.rotation/PI;
			double b_bul=(bullet.circle.y-bullet.circle.x*k_bul);
			double dis_bulAndE=disLinePoint(k_bul,b_bul,selfE);
			double angleOFbe=sinangle(selfE,bullet.circle);
				if(dis_bulAndE<selfE.r)
				RunOffPoint(selfE,bullet.circle,angleOFbe,angleE,order);
				else
				order.run=0;
		}
	 //子弹远或者是自己子弹
	 else
	 {


	if (info.robotInformation[myID].remainingAmmo>0)//有弹药
	
	{	
				if (selfE.y>=armyE.y&&angleE>=0)
	{

//		cout<<"   我方在下  UFO朝下";
		
			if (sumangle2<=180&&sumangle2>=0)
			{
				order.eturn=-1;
//				cout<<"逆时针转"<<endl;
				
			}	
			if (sumangle2>=180&&sumangle2<=360)
			{
				order.eturn=1;
//				cout<<"顺时针转"<<endl;
			}			
	}
	if (selfE.y>=armyE.y&&angleE<=0)
	{
	

//		cout<<"   我方在下  UFO朝上 ";
			if (sumangle2>=0)
			{
			order.eturn=-1;
//			cout<<"逆时针转"<<endl;
			}
			if (sumangle2<=-1)
			{
				order.eturn=1;
//				cout<<"顺时针转"<<endl;
			}
			
	}


	if (selfE.y<=armyE.y&&angleE>=0)
	{

//		cout<<"   我方在上  UFO朝下 ";
			if (chaangle2>=0)
		{
			order.eturn=1;
//			cout<<"顺时针转"<<endl;
		}
		if (chaangle2<=-1)
		{
			order.eturn=-1;
//			cout<<"逆时针转"<<endl;
		}

		
	}


	if (selfE.y<=armyE.y&&angleE<=0)
	{
//		cout<<"   我方在上  UFO朝上 ";


			if (chaangle2<180&&chaangle2>=0)
		{
			order.eturn=1;
//			cout<<"顺时针转"<<endl;
		}
		if (chaangle2>=180&&chaangle2<=359)
		{
			order.eturn=-1;
//			cout<<"逆时针转"<<endl;
		}

		
	}
	
		order.run=1;

	}
	


	else    //没有弹药
	{
		double disfire1=dis(info.arsenal[0].circle.x,selfE.x,info.arsenal[0].circle.y,selfE.y);
		double disfire2=dis(info.arsenal[1].circle.x,selfE.x,info.arsenal[0].circle.y,selfE.y);

		if(info.arsenal [0].respawning_time<10&&info.arsenal [1].respawning_time<10)
		{
			if(disfire1<disfire2)
				TurnToPoint(selfE,info.arsenal[0].circle,angleoffire1 ,angleE,order);
			else
				TurnToPoint(selfE,info.arsenal[1].circle,angleoffire2 ,angleE,order);

		}
		else
		{
				if(info.arsenal [0].respawning_time<10)
			TurnToPoint(selfE,info.arsenal[0].circle,angleoffire1 ,angleE,order);
				else if(info.arsenal [1].respawning_time<10)
			TurnToPoint(selfE,info.arsenal[1].circle,angleoffire2 ,angleE,order);
		}
		order.run=1;
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

	weapon = WT_Machinegun;	
	engine = ET_UFO;	
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "机枪3";
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
				order.eturn=-1;
//				cout<<"逆时针转"<<endl;
				
			}	
			if (sumangle>=180&&sumangle<=360)
			{
				order.eturn=1;
//				cout<<"顺时针转"<<endl;
			}			
	}
	if (self.y>=other.y&&angle<=0)
	{
	

//		cout<<"   我方在下  武器朝上 ";
			if (sumangle>=0)
			{
			order.eturn=-1;
//			cout<<"逆时针转"<<endl;
			}
			if (sumangle<=-1)
			{
				order.eturn=1;
//				cout<<"顺时针转"<<endl;
			}
			
	}


	if (self.y<=other.y&&angle>=0)
	{

//		cout<<"   我方在上  武器朝下 ";
			if (chaangle>=0)
		{
			order.eturn=1;
//			cout<<"顺时针转"<<endl;
		}
		if (chaangle<=-1)
		{
			order.eturn=-1;
//			cout<<"逆时针转"<<endl;
		}

		
	}


	if (self.y<=other.y&&angle<=0)
	{
//		cout<<"   我方在上  武器朝上 ";


			if (chaangle<180&&chaangle>=0)
		{
			order.eturn=1;
//			cout<<"顺时针转"<<endl;
		}
		if (chaangle>=180&&chaangle<=359)
		{
			order.eturn=-1;
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


	void  RobotAI::RunOffPoint(const Circle &self,const Circle &other,double angleofTwoEngine,double angle,RobotAI_Order& order)
	{
		double sumangle=angleofTwoEngine+angle;
		double chaangle=angleofTwoEngine-angle;
		
	if (self.y>=other.y&&angle>=0)
	{

//		cout<<"   我方在下  武器朝下";
		
			if (sumangle<=180&&sumangle>=0)
			{
				order.eturn=1;

				
			}	
			if (sumangle>=180&&sumangle<=360)
			{
				order.eturn=-1;

			}			
	}
	if (self.y>=other.y&&angle<=0)
	{
	

//		cout<<"   我方在下  武器朝上 ";
			if (sumangle>=0)
			{
			order.eturn=1;
//			cout<<"逆时针转"<<endl;
			}
			if (sumangle<=-1)
			{
				order.eturn=-1;
//				cout<<"顺时针转"<<endl;
			}
			
	}


	if (self.y<=other.y&&angle>=0)
	{

//		cout<<"   我方在上  武器朝下 ";
			if (chaangle>=0)
		{
			order.eturn=-1;
//			cout<<"顺时针转"<<endl;
		}
		if (chaangle<=-1)
		{
			order.eturn=1;
//			cout<<"逆时针转"<<endl;
		}

		
	}


	if (self.y<=other.y&&angle<=0)
	{
//		cout<<"   我方在上  武器朝上 ";


			if (chaangle<180&&chaangle>=0)
		{
			order.eturn=-1;
//			cout<<"顺时针转"<<endl;
		}
		if (chaangle>=180&&chaangle<=359)
		{
			order.eturn=1;
//			cout<<"逆时针转"<<endl;
		}

		
	}


	}