#include "RobotAI.h"
#include<iostream>
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

	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[1 - myID];
	/*静态成员列表*/
	static int frame = 0;
	static int hp_tag = tag.hp;
	/*静态成员列表*/


	//开火操作
	//cout << "*******************当前帧数" << frame << endl;
    if (frame != 0)
	{
			onFire1(order, info, myID);
	}
	onTag1(order, info, myID);

	//躲避操纵
	//isDodeg——判断是否执行躲避函数，如果执行则为true，将跳过后面的移动函数优先躲避
	//bullet ——最近的子弹
	bool isDodge = false;
	int b_index_nearest = GetNearetBul(order, info, myID);
	if (b_index_nearest != -1)//如果有敌方子弹则进行躲避判断
	{
		auto bullet = info.bulletInformation[b_index_nearest];
        isDodge = onDodge(order, info, myID, bullet);

	}
	
	
	//正常移动操作
	//isDodge = false;
	if (!isDodge)
	{
		onMove1(order, info, myID, tag.circle);
	}

	//静态变量更新
	hp_tag = tag.hp;
	frame++;
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
	srand(time(0));
	MyCar = rand() % 2+ 1;
	MyCar = AFV_Esaw;
	if (MyCar == AFV_Esaw)//幽灵电锯
	{
		weapon = WT_ElectricSaw;	
		engine = ET_AFV;	
	}
	if (MyCar == AFV_Prism)//幽灵光棱
	{
		weapon = WT_Prism;	
		engine = ET_AFV;
	}
	if (MyCar == AFV_WT_Machinegun)
	{
		weapon = WT_Machinegun;
		engine = ET_AFV;
	}
	
	
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "中二锯子";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "爆炸何";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return color;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return color;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return color;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return color;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return color;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return color;
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
	//system("Pause");
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

//判断和敌方之间是否有障碍物,ArID为障碍物ID
bool RobotAI::HaveBarrier(const RobotAI_BattlefieldInformation& info,const int myID,const int ArID)
{
	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[1 - myID];
	Circle obs_circle = info.obstacle[ArID];
	double pdis = pointdis(me.circle, tag.circle, obs_circle);
	if (pdis > info.obstacle[ArID].r)		
		return false;
	else
	{
		if (((me.circle.y - obs_circle.y)*(tag.circle.y - obs_circle.y)) &&
			((me.circle.x - obs_circle.x)*(tag.circle.x - obs_circle.x)))
			return false;
		else
			return true;	
	}

}

int RobotAI::GetNearetBul(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID)
{
	auto me = info.robotInformation[myID];
	//cout << "地图上子弹数量" << info.num_bullet + 1 << endl;
	int b_index_near = -1;
	int b_tag = 0;//敌方子弹数
	for (int b_index = 0; b_index <= info.num_bullet; b_index++)
	{
		//cout << "判断第" << b_index << "子弹" << endl;
		auto bullet = info.bulletInformation[b_index];
		if (bullet.launcherID == myID)
		{
			continue;
		}
		b_tag++;
		if (b_tag == 1)b_index_near = b_index;
		//cout << "第" << b_index << "子弹是敌方子弹" << endl;
		if (dis(bullet.circle, me.circle) <= dis(info.bulletInformation[b_index_near].circle, me.circle))
			b_index_near = b_index;		
	}
	//cout << "最近！" << b_index_near << endl;
	return b_index_near;//如果没有找到最近的敌方子弹就返回-1
}



void RobotAI::onTag1(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info, const int myID)//瞄准函数（不带预判）
{
	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[1 - myID];
	double dx = tag.circle.x - me.circle.x;
	double dy = tag.circle.y - me.circle.y;
	double Rota = atan2(dy, dx) * 180 / PI;
	double wpRota = info.robotInformation[myID].weaponRotation;
	double offset = wpRota - Rota;
	if (abvalue(offset) >= 180)
	{
		if (offset > 0)
			order.wturn = 1;
		else
			order.wturn = -1;
	}
	else{
		if (offset > 0)
			order.wturn = -1;
		else
			order.wturn = 1;
	}
	
}

double RobotAI::onTagRota(const RobotAI_BattlefieldInformation& info, const int myID)
{
	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[1 - myID];
	double dx = tag.circle.x - me.circle.x;
	double dy = tag.circle.y - me.circle.y;
	double Rota = atan2(dy, dx) * 180 / PI;
	double wpRota = info.robotInformation[myID].weaponRotation;
	double offset = wpRota - Rota;
	return offset;
}

void RobotAI::onMove1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID, const Circle circle_tag)
{
	onMove1(order, info, myID, circle_tag.x, circle_tag.y);
}

void RobotAI::onMove1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID,const double x_tag,const double y_tag)
{
	auto me = info.robotInformation[myID];
	double dx = x_tag - me.circle.x;
	double dy = y_tag - me.circle.y;
	double Rota = atan2(dy, dx) * 180 / PI;
	onMove1(order, info, myID, Rota);
}

void RobotAI::onMove1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID, const double rota_tag)
{
	auto me = info.robotInformation[myID];
	double Rota = rota_tag;
	double Rota_eng = me.engineRotation;
	double offset = Rota_eng - Rota;
	switch (MyCar)
	{
	case AFV_Esaw:
	case AFV_WT_Machinegun:
	case AFV_Prism:
		if (abvalue(offset) >= 180)
		{
			if (offset > 0)
				order.eturn = 1;
			else
				order.eturn = -1;
		}
		else{
			if (offset > 0)
				order.eturn = -1;
			else
				order.eturn = 1;
		}
		order.run = 1;
		break;
	default:
		break;
	}
}

void RobotAI::onFire1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID)
{
	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[1 - myID];
	switch (MyCar)
	{
	case AFV_Esaw:
		if (tag.weaponTypeName == WT_ElectricSaw)
		{
			if (
				!HaveBarrier(info, myID, 0) &&
				!HaveBarrier(info, myID, 1) &&
				(onTagRota(info, myID)<3) &&
				(onTagRota(info, myID)>-3) &&
				dis(me.circle, tag.circle)<120 + tag.circle.r
				)
				order.fire = 1;
			break;
		}
		if (
			!HaveBarrier(info, myID, 0) &&
			!HaveBarrier(info, myID, 1) &&
			dis(me.circle, tag.circle)<95 + tag.circle.r
			)
			order.fire = 1;				
		break;
	case AFV_Prism:
	case AFV_WT_Machinegun:
		if (
			!HaveBarrier(info, myID, 0) &&
			!HaveBarrier(info, myID, 1) &&
			(onTagRota(info, myID)<3)&&
			(onTagRota(info, myID)>-3)
			)
			order.fire = 1;
		break;
	default:
		break;
	}
}


bool  RobotAI::onDodge(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID,const  RobotAI_BulletInformation& bullet)
//躲闪函数，如果发出了躲避命令，则返回true;
{		
	auto me = info.robotInformation[myID];
	auto tag = info.robotInformation[myID];
	bool inHitRange = dis(bullet.circle, me.circle) <= 0.6 * me.circle.r *getBulletSpeed(bullet.type);//判断在监控范围内再躲
	double Rota_bulv = atan2(bullet.vy, bullet.vx) * 180 / PI;//子弹速度方向
	double Rota_bulv_ag = Rota_bulv - 180;//子弹速度反方向
	AngleAdjust(Rota_bulv_ag);
	auto Type_Bul = bullet.type;
	switch (MyCar)
	{
	case AFV_Esaw:
		if (order.fire == 1)
			return false;
		switch (Type_Bul)
		{
		case BT_ShotgunBall:
		case BT_MachinegunBall:
			order.eturn = 1;
			order.run = 1;
			return true;
			break;
		case BT_Cannonball:
		case BT_ApolloBall:
			if (tag.remainingAmmo <= 5)
			{
				return false;
			}
			if (!inHitRange)return false;
			order.eturn = 1;
			order.run = 1;
			return true;
			break;
		default:
			if (tag.remainingAmmo != 0)
			{
				return false;
			}
			if (!inHitRange)return false;
			order.eturn = 1;
			order.run = 1;
			return true;
			break;
			/*if (!inHitRange)
			{
				return false;
			}
			if (me.engineRotation >= Rota_bulv_ag)
				order.eturn = -1;
			else
				order.eturn = 1;
			order.run = 1;
			return true;
			break;*/
		}
		break;
		/*…………………………………………………………*/
	case AFV_Prism:
	case AFV_WT_Machinegun:
		break;
		return false;
	}
}

bool RobotAI::WillHit(const RobotAI_BulletInformation& bullet, const RobotAI_RobotInformation& rotinfo)
//判断子弹能否击中rotinfo
{
	//double pointdis_me_bul = pointdis(
	//	bullet.circle.x,
	//	bullet.circle.y,
	//	bullet.circle.x + bullet.vx,
	//	bullet.circle.y + bullet.vy,
	//	rotinfo.circle.x,
	//	rotinfo.circle.y
	//	);
	//double Rota = atan2(rotinfo.circle.y - bullet.circle.y, 
	//	rotinfo.circle.x- bullet.circle.x) * 180 / PI;//子弹为原点，子弹和机甲中心所构成的角
	//double Rota_bul = atan2(bullet.vy-rotinfo.vx, bullet.vx-rotinfo.vy) * 180 / PI;//子弹相对机甲射击方向角
	//double dis_me_bul = sqrt(dis(bullet.circle, rotinfo.circle));
	//double Rota_hit = atan2(
	//	rotinfo.circle.r,
	//	sqrt(dis_me_bul*dis_me_bul - rotinfo.circle.r*rotinfo.circle.r)
	//	);//能击中的角度范围
	//if (
	//	pointdis_me_bul <= rotinfo.circle.r &&
	//	(Rota_bul<Rota + abvalue(Rota_hit) || Rota_bul>Rota - abvalue(Rota_hit))
	//	)
	//	return true;
	//else
	//	return false;
	double Rota_bul = atan2(bullet.vy, bullet.vx ) * 180 / PI;//子弹相对机甲射击方向角
	AngleAdjust(Rota_bul);
	Beam bulbeam;
	bulbeam.x = bullet.circle.x;
	bulbeam.y = bullet.circle.y;
	bulbeam.rotation = Rota_bul;
	return HitTestBeamCircle(bulbeam, rotinfo.circle);
}

double RobotAI::abvalue(double x)
{
	if (x >= 0)
		return x;
	else
		return -x;
}

double RobotAI::pointdis(double x1, double y1, double x2, double y2, double x3, double y3)
//返回（x3,y3)到(x1,y1),(x2,y2)连线的距离
{
	double A = (y2 - y1) / (x2 - x1);
	double B = -1.0;
	double C = y1 - (y2 - y1)*x1 / (x2 - x1);
	double son = A*x3 + B*y3 + C;
	double Abvalue = son / sqrt(A*A + B*B);
	if (Abvalue > 0)
		return Abvalue;
	else
		return -Abvalue;
}
double pointdis(double k, double x1, double y1, double x3, double y3)
{
	double A = k;
	double B = -1.0;
	double C = y1 - k*x1;
	double son = A*x3 + B*y3 + C;
	double Abvalue = son / sqrt(A*A + B*B);
	if (Abvalue > 0)
		return Abvalue;
	else
		return -Abvalue;
}

double RobotAI::pointdis(const Circle a1, const Circle a2, const Circle a3)
{
	return pointdis(a1.x, a1.y, a2.x, a2.y, a3.x, a3.y);
}


double RobotAI::dis(double x1, double y1, double x2, double y2) {
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

double RobotAI::dis(const Circle& a, const Circle& b){
	return dis(a.x, a.y, b.x, b.y);
}




