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

	Point c;
	c.x=selfE.x+10;
	c.y=selfE.y;


	
	//距离计算
	double XX=pow((armyE.x-selfE.x),2);
	double YY=pow((armyE.y-selfE.y),2);
	double distanceX=sqrt(XX);
	double distanceY=sqrt(YY);
	double distance=sqrt(XX+YY);
//	cout<<distance<<"  ";

//	cout<<info.robotInformation[myID].engineRotation<<endl;

	//障碍检测
	hitbyBarriersLU=HitTestCircles(selfE,barrierLU);
	hitbyBarriersRD=HitTestCircles(selfE,barrierRD);
	
	//角度计算
	double lena=sqrt(pow(armyE.x-c.x,2)+pow(armyE.y-c.y,2));
	double lenb=10;
	double lenc=distance;
	double S2=distanceY*10;
	double sina=S2/lenb/lenc;

	double angleE=RadianToAngle(asin(sina));
		if (selfE.x>armyE.x)
		{
			angleE=180-angleE;//angleE当机甲处于右边时该怎么算？180-angle还是360-angle?
		}
//	cout<<"连线角度 "<<angleE<<"  ";
	
	double angleW=info.robotInformation[myID].weaponRotation;
//	cout<<"武器角度 "<<angleW<<endl;

//	cout<<"我方坐标"<<"("<<selfE.x<<","<<selfE.y<<")";
//	cout<<"敌方坐标"<<"("<<armyE.x<<","<<armyE.y<<")";
	double sumangle=angleE+angleW;
	double chaangle=angleE-angleW;
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
	//白刃冲刺
	double row=selfE.x-armyE.x;
	double line=selfE.y-armyE.y;
	//cout<<"row dis="<<row<<" ";
	//cout<<"line dis="<<line<<endl;
	if (distance>95+armyE.r)
	{
		//发生了碰撞
		if (count!=0&&(hitbyBarriersLU||hitbyBarriersRD))
		{
			double xx;
			double yy;
			count=15;
				if(hitbyBarriersLU)
		{
		xx=selfE.x-300;
		yy=selfE.y-250;
		if (fabs(xx)<180)
		{
			order.run=4;
			
		}
		if (fabs(yy)<180)
		{
			order.run=2;
			
		}
		
//		cout<<"撞了左上！  ";
	}
	if (hitbyBarriersRD)
	{
		xx=selfE.x-1066;
		yy=selfE.y-430;
		
		if (fabs(xx)<180)
		{
			order.run=3;
		}
		if (fabs(yy)<180)
		{
			order.run=1;
		}
//		cout<<"撞了右下！  ";
	}
	count--;
		}
	
	//未发生碰撞
		else

	{

		if (fabs(row)>90)
		{
			if (row>90)
		{
			order.run=1;
//			cout<<"go left"<<" ";			
		}
			else
		{
			order.run=2;
//			cout<<"go right ";
		}

		}

	if (fabs(line)>90)
		{
			if (line<-90)
		{
			
			order.run=4;
//			cout<<"go down "<<endl;
		}
			else
		{
			
			order.run=3;
//			cout<<"go up  "<<endl;
		}
		}
		
	}
	}
	
	//开火判定

	if (distance<=260+armyE.r&&(fabs(chaangle)<=5||fabs(sumangle)<=5))
	{
		order.fire=1;
		
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

	weapon = WT_Tesla;	
	engine = ET_Spider;	
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "魏爷的闪电小蜘蛛";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "雷系魔法使*薇薇安";
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