#include "RobotAI.h"
#include "math.h"
#include "algorithm"
#include "fstream"
using namespace std;
RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}
#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b
ofstream fout("h:\\test.txt");
Circle lastPlace[2];//上一帧的位置，0是我，1是敌人

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

	RobotAI_RobotInformation me = info.robotInformation[myID], armor = info.robotInformation[1 - myID];
	//根据是否有障碍物决定是否开火
	Circle obs[5];
	for(int i = 0; i < info.num_obstacle; i++)
		obs[i] = info.obstacle[i];
	order.fire = doIFire(me.circle, armor.circle, obs, info.num_obstacle);

	//根据对方的位置调整炮塔角度
	order.wturn = howToRotate(me.circle, armor.circle, info.robotInformation[myID].weaponRotation, armor.vx, armor.vy);

	//杀向敌人！
	order.run = runAndrun(me.circle, armor.circle, obs, info.num_obstacle);

	//检查子弹打光没	
	
	if(info.robotInformation[myID].remainingAmmo == 0 )
	{
		//没子弹了就去仓库领取
		order.run = runAndrun(me.circle, whichArsenal(info.arsenal[0], info.arsenal[1], me.circle), obs, info.num_obstacle);
	}		
	//等等！有子弹再打我？
	//Circle bu[200];
	int bu_num = info.num_bullet;
	for(int i = 0; i < bu_num; i++){
		if(info.bulletInformation[i].launcherID == 1 - myID){			
			if(BulletShotMe(info.bulletInformation[i].circle, me.circle, info.bulletInformation[i].vx, info.bulletInformation[i].vy)){
				//赶紧躲开！
				order.run = AvoidBullet(info.bulletInformation[i].circle, info.bulletInformation[i].vx, info.bulletInformation[i].vy, info.robotInformation[myID].circle);
				break;//先检测一颗子弹吧
			}
		}
	}
	
	lastPlace[0] = info.robotInformation[myID].circle;
	lastPlace[1] = info.robotInformation[1-myID].circle;
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

	weapon = WT_Machinegun;	//啊，我爱旋转机枪
	engine = ET_Spider;	//啊，我爱小蜘蛛
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "啦啦啦小凯";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "凯子歌打炮";
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
	lastPlace[0] = info.robotInformation[myID].circle;
	lastPlace[1] = info.robotInformation[1-myID].circle;
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
//gogogo，去我想去的地方
int RobotAI::runAndrun(Circle me,Circle armor, Circle obstacle[], int num_obs){
	//fout<<me.x<<"  "<<me.y<<"       "<<lastPlace[0].x<<"   "<<lastPlace[0].y<<endl;
	double dx = abs(me.x - armor.x);
	double dy = abs(me.y - armor.y);
	double dist, angle;
	for(int i = 0; i < num_obs; i++)
	{
		dist = Distance(me.x, me.y, obstacle[i].x, obstacle[i].y);
		if(dist == 120)
		{
			angle = atan2(me.y - obstacle[i].y, me.x - obstacle[i].y) * 180 / PI;
			if(angle>=0 && angle<90)
				return 4;
			else if(angle<0 && angle>=-90)
				return 3;
			else if(angle<-90 && angle>=-180)
				return 1;
			else
				return 1;
		}
	}
	

	if(dx < dy){
		if(me.y < armor.y)
			return 4;
		else
			return 3;
	}else{
		if(me.x < armor.x)
			return 2;
		else
			return 1;
	}
}
//如果有子弹靠近我
bool RobotAI::BulletShotMe(Circle bu, Circle me, double vx, double vy)
{
	if(Distance(bu.x, bu.y, me.x, me.y) < 100)
	{
		/*
		//打向我的角度
		double bulletAngle = atan2(vy,vx) * 180 / PI;
		//我与子弹的角度
		double angle = atan2(me.y - bu.y, me.x - bu.x) * 180 / PI;
		int poor = bulletAngle - angle;
		if(poor > 180){
			poor = 360 - poor;
		}else if(poor < -180)
		{
			poor += 360;
		}
		fout<<bulletAngle<<"   "<<angle<<"   "<<poor<<endl;
		//24.3°是0.45的反正切值，即有可能打到我的角度差值
		if(abs(poor) < 30)
		*/
		return true;		
	}
	return false;
}
//小蜘蛛躲子弹
int RobotAI::AvoidBullet(Circle bu, double vx, double vy, Circle myself)
{
	//针对蜘蛛坦克V3，躲避子弹用
	if(vy > vx)
	{
		if(bu.x < myself.x)
			return 2;
		else
			return 1;
	}else
	{
		if(bu.y < myself.y)
			return 4;
		else
			return 3;
	}
	
}

//确定旋转角方向,旋转机枪Mashinegun射速11
int RobotAI::howToRotate(Circle me, Circle armor,double weapon_rotation, double vx, double vy)
{
	double angle = atan2(armor.y - me.y, armor.x - me.x) * 180 / PI;//我和敌人之间的角度
	double v1Angle;//敌人速度方向
	double offset = 0;//这是偏移角
	
	if( lastPlace[1].x != armor.x || lastPlace[1].y != armor.y)//当敌人运动的时候计算偏移角
	{
		double v1 = Distance(vx, vy, 0, 0);//计算armorSpeed速度也可以套用distance函数
		double v2 = 11.0;//旋转机枪Mashinegun射速11
		double d = Distance(me.x,me.y,armor.x,armor.y);//我和敌人之间的距离
		v1Angle = atan2(vy, vx) * 180 / PI;
		/*
		double B = (max(angle, v1Angle)) - (min(angle, v1Angle)) - 180;//三角形的角B
		//解如下方程（v1^2-v2^2)*t^2 - 2*d*v1*cosB*t + d^2 = 0, 设a=v1^2-v2^2, b=2*d*v1*cosB, c=d^2
		double a = pow(v1,2) - pow(v2,2), b = 2*d*v1*cos(B * PI / 180), c = pow(d,2);//a,b,c是方程的系数
		double dlt = sqrt(b*b-4*a*c);
		double t = (-b-dlt)/(2*a);//这是方程的解t，另外一个解是负数舍弃
		offset = acos((d-v1*cos(B * PI / 180)*t)/(v2*t)) * 180 / PI;
		*/
		//解一个等腰三角形
		double t1 = d / v2;
		double d1 = v1 * t1;
		//offset = acos((2*d*d-d1*d1)/(2*d*d)) * 180 / PI;
		//加上偏移角
		if(angle > 0 && angle < 180){
			if(angle - 180 < v1Angle && v1Angle < angle)
				angle -= offset;
			else{
				angle += offset;
				angle = (angle > 180)?(360-angle):angle;
			}
		}else if(angle < 0 && angle > -180){
			if(angle < v1Angle && v1Angle < angle + 180)
				angle += offset;
			else{
				angle -= offset;
				angle = (angle < -180) ? (360+angle) : angle;
			}
		}else if(angle == 0){
			if(v1Angle > 0)
				angle += offset;
			else
				angle -= offset;
		}else{
			if(v1Angle < 0)
				angle += offset;
			else
				angle = 180 - offset;
		}
	}

	//判断旋转角度
	if(angle > 0 && angle < 180){
		if(angle - 180 < weapon_rotation && weapon_rotation < angle)
			return 1;
		else
			return -1;
	}
	else if(angle < 0 && angle > -180){		
		if(angle < weapon_rotation && weapon_rotation < angle + 180)
			return -1;
		else
			return 1;
	}else if(angle == 0){
		if(weapon_rotation > 0)
			return -1;
		else
			return 1;
	}else{
		if(weapon_rotation >0)
			return 1;
		else
			return -1;
	}
}

//何时开炮？有障碍物的时候要节约子弹哦
int RobotAI::doIFire(Circle me, Circle armor,Circle obstacle[],int num_obs)
{
	double A,C,dis;
	double distWithAr = Distance(me.x, me.y, armor.x, armor.y), distWithObs;
	for(int i = 0; i < num_obs; i++){		
		distWithObs = Distance(me.x, me.y, obstacle[i].x, obstacle[i].y);
		if(distWithAr < distWithObs)//障碍物在另外一边则继续开炮
			return 1;
		A = (armor.y - me.y) / (armor.x - me.x);
		C = me.y - A * me.x;
		dis = abs(A * obstacle[i].x - obstacle[i].y + C) / sqrt( A * A + 1 );
		if( dis <= obstacle[i].r )
		{
			return 0;
		}
	}
	return 1;
}
//选则一个可以去的军火库
Circle RobotAI::whichArsenal(RobotAI_ArsenalInformation ar1, RobotAI_ArsenalInformation ar2, Circle me)
{
	if(ar1.respawning_time == 0 && ar2.respawning_time == 0)
	{
		double dis_ars1 = Distance(ar1.circle.x, ar1.circle.y, me.x, me.y);
		double dis_ars2 = Distance(ar2.circle.x, ar2.circle.y, me.x, me.y);
		if(dis_ars1 < dis_ars2){
			return ar1.circle;
		}else{
			return ar2.circle;
		}
	}else if(ar1.respawning_time == 0)	
		return ar1.circle;
	else if(ar2.respawning_time == 0)
		return ar2.circle;
	else
		return Circle();
}
//距离函数
double RobotAI::Distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}