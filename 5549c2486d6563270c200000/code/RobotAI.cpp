#include "RobotAI.h"
#include "math.h"
#include "algorithm"
#include "fstream"
#include <iostream>
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
	double fire_angle = howToRotate(me.circle, armor.circle, me.weaponRotation, armor.vx, armor.vy);//应该旋转的角度
	order.fire = doIFire(me.circle, armor.circle, obs, info.num_obstacle, me.weaponRotation, fire_angle);

	//根据对方的位置调整炮塔角度
	order.wturn = Rotate(fire_angle, me.weaponRotation);

	//杀向敌人！
	order.run = 1;
	order.eturn =  runAndrunAFV(me.circle, armor.circle, me.engineRotation);
	double distance_me_armor = Distance(me.circle.x, me.circle.y, armor.circle.x, armor.circle.y);
	if(armor.weaponTypeName != WT_MissileLauncher && armor.engineTypeName != ET_Shuttle){//打猥琐飞弹和太空要塞只需贴脸！
		if(distance_me_armor <= 250)//太近了就往回走
		order.eturn =  runAndrunAFV(armor.circle, me.circle, me.engineRotation);
	}
	
	

			
	//等等！有子弹再打我？
	//Circle bu[200];
	int bu_num = info.num_bullet;
	for(int i = 0; i < bu_num; i++){
		if(info.bulletInformation[i].launcherID == 1 - myID){			
			if(BulletShotMe(info.bulletInformation[i].circle, me.circle, info.bulletInformation[i].vx, info.bulletInformation[i].vy, armor.weaponTypeName)){
				//赶紧躲开！
				order.eturn = AvoidBulletAFV(info.bulletInformation[i], me);
				break;//先检测一颗子弹吧
			}
		}
	}
	//检查子弹打光没	
	if(info.robotInformation[myID].remainingAmmo <= 1 )
	{
		//没子弹了就去仓库领取
		order.eturn = runAndrunAFV(me.circle, whichArsenal(info.arsenal[0], info.arsenal[1], me.circle),me.engineRotation);
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

	weapon = WT_Cannon;	//啊，我爱旋转机枪
	engine = ET_AFV;	//啊，我爱战锤坦克
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
	return 205;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 255;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -30;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 7;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return -223;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 61;
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
//gogogo，去我想去的地方
int RobotAI::runAndrun(Circle me,Circle armor, Circle obstacle[], int num_obs){
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
//战锤坦克行动！注意返回的是eturn
int RobotAI::runAndrunAFV(Circle me,Circle armor,double engine_rotation)
{
	double an = atan2(armor.y - me.y, armor.x - me.x) * 180 / PI;	
	
	return Rotate(an,engine_rotation);
	
}
//如果有子弹靠近我
bool RobotAI::BulletShotMe(Circle bu, Circle me, double vx, double vy, weapontypename weapontype)
{
	int min_dis = 250;
	if(weapontype == WT_Apollo || weapontype == WT_RPG)
		min_dis = 350;
	if(Distance(bu.x, bu.y, me.x, me.y) < min_dis)
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
//战锤坦克躲子弹
int RobotAI::AvoidBulletAFV(RobotAI_BulletInformation bu, RobotAI_RobotInformation me)
{
	double bu_angle = atan2(bu.vy, bu.vx) * 180 / PI;
	bu_angle += 90;
	if(bu_angle > 180) bu_angle = 360 - bu_angle;
	return Rotate(bu_angle,me.engineRotation);	
}

//确定旋转角方向,旋转机枪Mashinegun射速11,返回应该旋转的角度
double RobotAI::howToRotate(Circle me, Circle armor,double weapon_rotation, double vx, double vy)
{
	double angle = atan2(armor.y - me.y, armor.x - me.x) * 180 / PI;//我和敌人之间的角度
	double v1Angle;//敌人速度方向
	double offset = 0;//这是偏移角
	
	if( lastPlace[1].x != armor.x || lastPlace[1].y != armor.y)//当敌人运动的时候计算偏移角
	{
		double v1 = Distance(vx, vy, 0, 0);//计算armorSpeed速度也可以套用distance函数
		double v2 = 11.0;//加农炮射速11
		double d = Distance(me.x,me.y,armor.x,armor.y);//我和敌人之间的距离
		double r = d * v1 / sqrt( v2 * v2 - v1 * v1); // 辅助圆的半径
		v1Angle = atan2(vy, vx) * 180 / PI;
		double ABC_angle = 180 - v1Angle + angle;//辅助角CBD
		double d_AC = sqrt(d*d + r*r - 2*d*r*cos(ABC_angle / 180 * PI));//三角形的AC边
		offset = acos((d - r*cos(ABC_angle / 180 * PI)) / d_AC) * 180 / PI;
		fout<<d<<"  "<<r<<"   "<<v1Angle<<"   "<<angle<<"   "<<ABC_angle<<"   "<<offset<<endl;
		//offset = acos(1 - v1 * v1 / (2 * v2 * v2)) * 180 / PI ;		
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
	return angle;
}

//何时开炮？有障碍物的时候要节约子弹哦
int RobotAI::doIFire(Circle me, Circle armor,Circle obstacle[],int num_obs,double now_angle, double fire_angle)
{
	if( abs(now_angle - fire_angle) > 5 )//角度太偏就不射
		return 0;
	double A,C,dis;
	Circle obs;
	double distWithAr = Distance(me.x, me.y, armor.x, armor.y), //与敌人的距离
		distWithObs1 = Distance(me.x, me.y, obstacle[0].x, obstacle[0].y),
		distWithObs2 = Distance(me.x, me.y, obstacle[1].x, obstacle[1].y),distWithObs;
	if(distWithAr >= 600)//距离太远就不打！
		return 0;
	if(distWithObs1 < distWithObs2)
	{
		obs = obstacle[0];
		distWithObs = distWithObs1;
	}
	else
	{
		obs = obstacle[1];
		distWithObs = distWithObs2;
	}	
	A = tan(now_angle / 180 * PI);
	C = me.y - A * me.x;
	dis = abs(A * obs.x - obs.y + C) / sqrt( A * A + 1 );//直线到圆心的距离	
	
	if( dis <= obs.r && distWithAr > distWithObs)//判断子弹是否被挡住
	{		
		return 0;
	}
	//fout<<now_angle<<"   "<<fire_angle<<"  "<<A<<"  "<<dis<<"   "<<obs.r<<endl;
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
//旋转函数
int RobotAI::Rotate(double angle_to, double angle_now){
	if(angle_to > 0 && angle_to < 180){
		if(angle_to - 180 < angle_now && angle_now < angle_to)
			return +1;
		else
			return -1;
	}
	else if(angle_to < 0 && angle_to > -180){		
		if(angle_to < angle_now && angle_now < angle_to + 180)
			return -1;
		else
			return 1;
	}else if(angle_to == 0){
		if(angle_now > 0)
			return -1;
		else
			return 1;
	}else{
		if(angle_now >0)
			return 1;
		else
			return -1;
	}
}