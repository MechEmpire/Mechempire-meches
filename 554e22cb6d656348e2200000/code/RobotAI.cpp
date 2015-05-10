#include "RobotAI.h"
#include <iostream>
using namespace std;
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
	double my_x = info.robotInformation[myID].circle.x;
	double my_y = info.robotInformation[myID].circle.y;
	double you_x = info.robotInformation[1-myID].circle.x;
	double you_y = info.robotInformation[1-myID].circle.y;
    double rotation = atan2((you_y-my_y),(you_x-my_x));
    double angle = RadianToAngle(rotation);
	double p_angle =info.robotInformation[myID].weaponRotation;
	//cout<<angle<<" "<<p_angle<<endl;
	SetSeed();
	/*
		枪口指向敌人
	*/
	if(angle<=90&&angle>=-90){
		//对方机甲在我右方
		if(p_angle<=90&&p_angle>=-90){
			//武器指向右方
			if(angle>p_angle)
				order.wturn = 1;
			else
				order.wturn = -1;
		}
		else{
				//武器指向左方
			if(p_angle>90&&angle>p_angle-180)
				order.wturn= -1;
			else if(p_angle>90&&angle<p_angle-180)
				order.wturn = 1;
			else if(p_angle<-90&&angle>p_angle+180)
				order.wturn= -1;
			else if(p_angle<-90&&angle<p_angle+180)
					order.wturn = 1;
		}
	}
	else{
		//对方机甲在我左方
		if(p_angle>90||p_angle<-90){
			//武器指向左方
			if(angle*p_angle<0){
				//不同象限
				if(angle>90)
					order.wturn = -1;
				else
					order.wturn = 1;
			}
			else{
				if(angle>p_angle)
					order.wturn = 1;
				else
					order.wturn = -1;
			}
		}
		else{
			//武器指向右方
			if(angle*p_angle<0){
				if(angle>90){
					if(p_angle>angle-180)
						order.wturn = 1;
					else
						order.wturn = -1;
				}
				else{
					if(p_angle>angle+180)
						order.wturn = 1;
					else
						order.wturn = -1;
				}
			}else{
				if(angle>90){
					order.wturn = 1;
				}
				else{
					order.wturn = -1;
				}
			}
		}
	}
	if(abs(angle-p_angle)<=1){
		/*
			开火条件，能够击中（判断）
		*/
		Beam b;
		b.x = my_x;
		b.y = my_y;
		b.ex = you_x;
		b.ey = you_y;
		if(fire(b,info))
			order.fire = 1;
		order.wturn = 0;
	}
	//走位
	double current_x = info.robotInformation[myID].circle.x;
	double current_y = info.robotInformation[myID].circle.y;
	Point your;
	your.x = info.robotInformation[1-myID].circle.x;
	your.y = info.robotInformation[1-myID].circle.y;
	Point my;
	my.x = current_x;
	my.y = current_y;
	if(info.robotInformation[myID].remainingAmmo!=0){
		Point p;
		p.x = 1366 - your.x;
		p.y = 680 - your.y;
		/*
		if(UPorDown==1){
			//进攻路径规划
			// go (50,630)
			p.x = 300;
			p.y = 130;
		}
		else{
			p.x = 866;
			p.y = 250;
		}
		*/
		GetDistance(p,your);
		order.run=Go(current_x,current_y,p);
	}
	//子弹耗尽
	else{
		Point p1;
		p1.x = 50;
		p1.y = 630;
		Point p2;
		p2.x = 1316;
		p2.y = 50;
		if(GetDistance(p1,your)<GetDistance(p2,your))
			order.run=Go(current_x,current_y,p1);
		else
			order.run=Go(current_x,current_y,p1);
	}
	//逃跑
	if(info.robotInformation[myID].hp<=40&&info.robotInformation[1-myID].hp>=50){
		//escape
		Point p1;
		p1.x = 300;
		p1.y = 430;
		Point p2;
		p2.x = 1066;
		p2.y = 250;
		double temp_rotation = rotation>0?180-rotation:180+rotation;
		double temp_x = 80*sin(temp_rotation);
		double temp_y = 80*cos(temp_rotation);
		if(GetDistance(p1,my)<GetDistance(p1,my)){
			if(p1.x>your.x){
				p1.x+=temp_x;
			}
			else{
				p1.x-=temp_x;
			}

			if(p1.y>your.y){
				p1.y+=temp_y;
			}
			else{
				p1.y-=temp_y;
			}
			order.run=Go(current_x,current_y,p1);
		}
		else{
			if(p2.x>your.x){
				p2.x+=temp_x;
			}
			else{
				p2.x-=temp_x;
			}

			if(p2.y>your.y){
				p2.y+=temp_y;
			}
			else{
				p2.y-=temp_y;
			}
			order.run=Go(current_x,current_y,p2);
		}
	}
	//距离过近
	if(GetDistance(your,my)<60){
		if(abs(info.robotInformation[1-myID].vx)<abs(info.robotInformation[1-myID].vy)){
			if(info.robotInformation[1-myID].circle.y>my.y)
				order.run = 3;
			else
				order.run = 4;
		}
		else{
			if(info.robotInformation[1-myID].circle.x>my.x)
				order.run = 1;
			else
				order.run = 2;
		}
		
	}
	//躲子弹
	double bullet_distance = 1000;
	Beam bb;
	bb.x = -1;
	bb.y = -1;
	if(1){
		for(int i=0;i<info.num_bullet;i++){
			if(1-myID==info.bulletInformation[i].launcherID){
				int k = abs(v_position(info.bulletInformation[i].vx,info.bulletInformation[i].vy)-e_position(my.x,my.y,info.bulletInformation[i].circle.x,info.bulletInformation[i].circle.y));
				if(bullet_distance>GetDistance(my,info.bulletInformation[i].circle)){//k==4&&
					bullet_distance = GetDistance(my,info.bulletInformation[i].circle);
					bb.x = info.bulletInformation[i].circle.x;
					bb.y = info.bulletInformation[i].circle.y;
					bb.ex = info.bulletInformation[i].vx;
					bb.ey = info.bulletInformation[i].vy;
				}
			}
		}
		cout<<"bbbbbb"<<bullet_distance<<","<<bb.x<<endl;
		if(bb.x!=-1&&bullet_distance<80){
			cout<<order.run<<endl;
			double x_gap = abs(bb.x-my.x);
			double y_gap = abs(bb.y-my.y);
			struct Circle c1 = {300, 250, 75};
			struct Circle c2 = {1066,430, 75};
			if (x_gap<y_gap)
			{
				if((my.y-250>0&&my.y-250<80)||((my.y-430>0)&&(my.y-430<80)))
					order.run = 4;
				else if((250-my.y>0&&250-my.y<80)||((430-my.y>0)&&(430-my.y<80)))
					order.run = 3;
				else
					order.run = 4;
			}else{
				if((my.x-300>0&&my.x-300<80)||((my.x-1066>0)&&(my.x-1066<80)))
					order.run = 2;
				else if((300-my.x>0&&300-my.x<80)||((1066-my.x>0)&&(1066-my.x<80)))
					order.run = 1;
				else
					order.run = 3;
			}
			cout<<order.run<<endl;
		}
	}
}
int RobotAI::Go(double current_x,double current_y,Point p){
	int turn = 0;
	if(HorV == 0){
		if((int)p.x-(int)current_x>4)
			turn = 2;
		else if((int)current_x-(int)p.x>4)
			turn = 1;
		else
			turn = 0;
		
	}
	else{
		if((int)p.y-(int)current_y>4)
			turn = 4;
		else if((int)current_y-(int)p.y>4)
			turn = 3;
		else
			turn = 0;
	}
	//HorV = (1+ HorV)%((int)(current_y/current_x)+1);
	HorV = 1- HorV;
	//cout<<current_x<<","<<current_y<<","<<HorV<<endl;
	return turn;
}
int RobotAI::v_position(double x,double y){
	if(x*y==0){
		if(x==0&&y<0)
			return 0;
		else if(x==0&&y>0)
			return 4;
		else if(x>0&&y==0)
			return 2;
		else if(x<0&&y==0)
			return 6;
		else 
			return -1;
	}
	else{
		if(y<0&&x>0)
			return 1;
		else if(y>0&&x>0)
			return 3;
		else if(y>0&&x<0)
			return 5;
		else if(y<0&&x<0)
			return 6;
		else
			return 0;
	}
}
int RobotAI::e_position(double x1,double y1,double x2,double y2){
	if(x2>x1&&y2>y1)
		return 3;
	else if(x2<x1&&y2>y1)
		return 5;
	else if(x2>x1&&y2<y1)
		return 1;
	else if(x2<x1&&y2<y1)
		return 7;
	else if (x2==x1&&y2>y1)
		return 4;
	else if(x2==x1&&y2<y1)
		return 0;
	else if(x2>x1&&y2==y1)
		return 2;
	else if(x2<x1&&y2==y1)
		return 6;
	else
		return 0;
}
void RobotAI::strategy1(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID){
	//返回当前策略
}
double RobotAI::GetDistance(Point x,Point y){
	return sqrt((x.x-y.x)*(x.x-y.x)+(x.y-y.y)*(x.y-y.y));
}
double RobotAI::GetDistance(Point x,Circle y){
	return sqrt((x.x-y.x)*(x.x-y.x)+(x.y-y.y)*(x.y-y.y));
}
bool RobotAI::isBullet(weapontypename a){
	bool isBullet;
	switch(a){
	case WT_Cannon:
		isBullet = true;
		break;
	case WT_Shotgun:
		isBullet = true;
		break;
	case WT_RPG:
		isBullet = true;
		break;
	case WT_Machinegun:
		isBullet = true;
		break;
	case WT_MissileLauncher:
		isBullet = true;
		break;
	case WT_GrenadeThrower:
		isBullet = true;
		break;
	case WT_MineLayer:
		isBullet = true;
		break;
	default:
		isBullet = false;
	}
	//cout<<isBullet<<endl;
	return isBullet;
}
bool RobotAI::fire(Beam b,const RobotAI_BattlefieldInformation& info){
	struct Circle c1 = {300, 250, 75};
	struct Circle c2 = {1066,430, 75};
	double k = (b.ey - b.y)/(b.ex - b.x);
	double up1 = abs(k * (c1.x) - k* b.ex + b.ey - (c1.y));
	double up2 = abs(k * (c2.x) - k* b.ex + b.ey - (c2.y));
	double down = sqrt(k*k + 1);
	if(up1/down<75||up2/down<75)
		return false;
	else
		return true;
}

int RobotAI::escape(Beam b,const RobotAI_BattlefieldInformation& info,int my_id,int currentRun){
	double my_vx = info.robotInformation[my_id].vx;
	double my_vy = info.robotInformation[my_id].vy;
	double my_x = info.robotInformation[my_id].circle.x;
	double my_y = info.robotInformation[my_id].circle.y;
	double distance_x = my_x - b.x;
	double distance_y = my_y - b.y;
	//current position
	if(distance_x/b.ex==distance_y/b.ey)
		return against_position(currentRun); 
}
int RobotAI::against_position(int currentRun){
	if(currentRun==1)
		return 2;
	else if(currentRun==2)
		return 1;
	else if(currentRun==3)
		return 4;
	else
		return 3;
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

	weapon = WT_Prism;	//啊，我爱加农炮
	engine = ET_Spider;	//啊，我爱小蜘蛛
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "null";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "Nameless";
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
	if(info.robotInformation[myID].circle.y==50)
		UPorDown = 1;
	else
		UPorDown = -1;
	HorV = 0;

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