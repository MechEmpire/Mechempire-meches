#include "RobotAI.h"

RobotAI::RobotAI()
{

}

RobotAI::~RobotAI()
{

}

//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------

void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	

	UpdateBattlefieldInfo(info);//更新一些地图相关的自定义变量

	/*
	机甲状态(state)在0、1、2三种方式之间切换，0为躲避态，1为攻击态，2为弹药缺失态，每帧最后调用
	StateTransform函数确定下一帧状态
	*/
	
	switch (state)
	{
	case 0:
		//躲在障碍物后面伺机逼近
		HideAndApproach(order);
		break;

	case 1:
		//杀出去
		Crusher_Attack(order);
		Attack_ElectricSaw(order);
		break;

	case 2:
		//补子弹
		Reload(order);
		break;

	default:
		state = 0;
		HideAndApproach(order);
		break;
	}

	StateTransform();

}


void RobotAI::ChooseArmor(weapontypename& weapon, enginetypename& engine, bool a)
{
	//挑选装备函数
	//功能：在战斗开始时为你的机甲选择合适的武器炮塔和引擎载具
	//参数：weapon	...	代表你选择的武器，在函数体中给它赋值
	//		engine	...	代表你选择的引擎，在函数体中给它赋值
	//tip:	括号里的参数是枚举类型 weapontypename 或 enginetypename
	//		开发文档中有详细说明，你也可以在RobotAIstruct.h中直接找到它们的代码
	//tip:	最后一个bool是没用的。。那是一个退化的器官

	weapon = WT_ElectricSaw;	//选择武器为电锯
	engine = ET_Robotman;	//选择引擎为毁灭者
}


//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "猥琐_绞肉机";
}


string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "猥琐学长，文成武德，千秋万载，一统江湖";
}


//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return 71;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 20;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -13;
}


//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 104;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 12;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return -48;
}


//-----------------------------------------------------
//3.用不用随你的触发函数
//-----------------------------------------------------

void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info, int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	

	state = 0;
	battle_info = info;

	//记录自己ID，获取敌人ID（两人局）
	this->myID = myID;
	enemyID = 1 - myID;

	//以Vector形式记录两个arsenal(军火库)的位置
	vec_arsenal0.x = battle_info.arsenal[0].circle.x;
	vec_arsenal0.y = battle_info.arsenal[0].circle.y;

	vec_arsenal1.x = battle_info.arsenal[1].circle.x;
	vec_arsenal1.y = battle_info.arsenal[1].circle.y;

}


void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info, int myID)
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


void RobotAI::onHit(int launcherID, bullettypename btn)
{
	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
}


//TODO:这里可以实现你自己的函数


void RobotAI::UpdateBattlefieldInfo(const RobotAI_BattlefieldInformation& info)
{
	battle_info = info;//战场信息更新

	vec_me.x = MY.circle.x;//Vector形式自己的X坐标
	vec_me.y = MY.circle.y;//Vector形式自己的Y坐标

	vec_enemy.x = ENEMY.circle.x;//Vector形式敌人的X坐标
	vec_enemy.y = ENEMY.circle.y;//Vector形式敌人的Y坐标
}


/*
一些基础行为，可根据选择武器不同直接修改这里的实现
	Fire：开火
	AimAtPoint：炮口瞄向某一点
	DirectMoveToPoint:直接移动到某一点
*/

void RobotAI::Fire(RobotAI_Order& order)
{
	order.fire = 1;
}


bool RobotAI::AimAtPoint(RobotAI_Order &order, const double &x, const double &y)
{
	//返回是否瞄准转动结束
	//瞄准点(x,y)

	double rotation = MY.weaponRotation;

	double rr = atan2(y - MY.circle.y, x - MY.circle.x);
	rr = RadianToAngle(rr);

	double c = 0, a = 0;		//clockwise顺时针，anticlockwise逆时针

	if (rr>rotation)
	{
		c = rr - rotation;
		a = rotation + 360 - rr;	//(rotation-(-180))+(180-rr)
	}
	else
	{
		a = rotation - rr;
		c = 360 - rotation + rr;	//(180-rotation)+(rr-(-180));
	}

	order.wturn = 0;
	if (c>a)
	{
		if (abs(a)>get_weapon_rotationSpeed(MY.weaponTypeName))
		{
			order.wturn = -1;
			return false;
		}
	}
	else
	{
		if (abs(c)>get_weapon_rotationSpeed(MY.weaponTypeName))
		{
			order.wturn = 1;
			return false;
		}
	}
	return true;
}


void RobotAI::DirectMoveToPoint(RobotAI_Order &order, const double &x, const double&y)
{
	order.eturn = 0;

	double rr = RadianToAngle(atan2(y - vec_me.y, x - vec_me.x));	//目标正向方向

	double rotation = MY.engineRotation;

	double c = 0, a = 0;		//clockwise, anti-clockwise

	bool forward_or_backward = true;	//前进或者后退

	//TODO:判断方向，选择采取前进还是后退
	if (rr>rotation)
	{
		c = rr - rotation;
		a = rotation + 360 - rr;	//(rotation-(-180))+(180-rr)
	}
	else
	{
		a = rotation - rr;
		c = 360 - rotation + rr;	//(180-rotation)+(rr-(-180));
	}

	order.eturn = 0;
	if (c>a)
	{
		if (abs(a)>90)
		{
			//倒着走
			forward_or_backward = false;
			if (180 - a>get_engine_rotationSpeed(MY.engineTypeName))
			{
				order.eturn = 1;
			}
		}
		else
		{
			forward_or_backward = true;
			if (abs(a)>get_engine_rotationSpeed(MY.engineTypeName))
			{
				order.eturn = -1;
			}
		}
	}
	else
	{
		if (abs(c)>90)
		{
			//倒着走
			forward_or_backward = false;
			if (180 - c>get_engine_rotationSpeed(MY.engineTypeName))
			{
				order.eturn = -1;
			}
		}
		else
		{
			forward_or_backward = true;
			if (abs(c)>get_engine_rotationSpeed(MY.engineTypeName))
			{
				order.eturn = 1;
			}
		}
	}

	if (forward_or_backward) {//毁灭者的特性：当 eturn!=0 时（即旋转时），无法前后移动， 旋转优先级大于移动，故这儿会先转完
		order.run = 1;
	} else {
		order.run = -1;
	}
}


void RobotAI::Attack_ElectricSaw(RobotAI_Order&order)
{
	AimAtPoint(order, ENEMY.circle.x, ENEMY.circle.y);

	//...居然没提供获取weapon半径的接口
	if (Vec2DDistanceSq(vec_me, vec_enemy) <= (95 + MY.circle.r)*(95 + MY.circle.r))	//电锯半径
	{
		Fire(order);
	}
}


void RobotAI::DirectMoveAwayFromPoint(RobotAI_Order &order, const double &x, const double &y)
{
	order.run = 0;	//默认停
	double dx = x - MY.circle.x;
	double dy = y - MY.circle.y;

	if (abs(dx)>abs(dy))
	{
		//左右移动
		if (dx<0)
		{
			order.run = 2;
		}
		else if (dx>0)
		{
			order.run = 1;
		}
	}
	else
	{
		//上下移动
		if (dy<0)
		{
			order.run = 4;
		}
		else if (dy>0)
		{
			order.run = 3;
		}
	}
}


Vector2D RobotAI::GetHidingPosition(const Circle &o, const Circle & hunter)
{
	double distance_from_boundary = MY.circle.r + 3;

	double distance_away = distance_from_boundary + o.r;

	//早晚得用上 Vector2D进来。。

	Vector2D obstacle;
	obstacle.x = o.x;
	obstacle.y = o.y;

	Vector2D vec;
	vec.x = o.x - hunter.x;
	vec.y = o.y - hunter.y;
	vec.Normalize();

	Vector2D target = vec*distance_away + obstacle;

	return target;

}


void RobotAI::Hide(RobotAI_Order & order, const Circle & hunter)
{
	//TODO:当对方是跟踪弹时要使用另一种策略

	double dist_to_closest = 1e20;
	Vector2D best_hiding_position;

	int i;


	for (i = 0; i<battle_info.num_obstacle; i++)
	{
		Vector2D hiding_position = GetHidingPosition(battle_info.obstacle[i], hunter);

		//距离最短视作最好的躲避点
		double dist = Vec2DDistanceSq(hiding_position, vec_me);

		if (dist<dist_to_closest)
		{
			dist_to_closest = dist;
			best_hiding_position = hiding_position;
		}


	}

	if (dist_to_closest == 1e20)
	{
		//没有合适障碍物
		//TODO:refine躲开
		DirectMoveAwayFromPoint(order, hunter.x, hunter.y);
	}


	//TODO:refine 路径规划
	if (!IsArriveAtPoint(best_hiding_position.x, best_hiding_position.y))
	{
		DirectMoveToPoint(order, best_hiding_position.x, best_hiding_position.y);
	}



}


//判断部分

bool RobotAI::IsArriveAtPoint(const double &x, const double &y)
{
	//<的参数需要根据情况调整
	return abs(MY.circle.x - x) + abs(MY.circle.y - y)<min_distance;
}


bool RobotAI::IsEnemyBullet(int bullet_idx)
{
	return enemyID == battle_info.bulletInformation[bullet_idx].launcherID;
	//return (enemyID==0 && battle_info.bulletInformation[bullet_idx].circle.r<-998) || (enemyID==1 && battle_info.bulletInformation[bullet_idx].circle.r>998);
}


bool RobotAI::IsSafeToRush()
{
	int times = 1;
	if (ENEMY.weaponTypeName == WT_Shotgun)
	{
		times = 5;
	}

	if (ENEMY.remainingAmmo*(times*(get_bullet_damage((bullettypename)ENEMY.weaponTypeName)) + battle_info.num_bullet)<MY.hp)
	{
		//剩余子弹和还在飞的子弹全部加起来也打不死我
		return true;
	}
	else //if(ENEMY.hp>MY.hp)
	{
		//死亡冲刺
		Vector2D vec_dist = vec_enemy - vec_me;
		double dist = vec_dist.Length();
		int time_for_death_rush = dist / (get_engine_maxSpeed(MY.engineTypeName));

		//这种算法恐怕不是太保险
		//int time_before_death=MY.hp/(get_weapon_damage(ENEMY.weaponTypeName))*(get_weapon_coolingTime(ENEMY.weaponTypeName));

		//多减一个时间看看
		int time_before_death = MY.hp / (get_bullet_damage((bullettypename)ENEMY.weaponTypeName)*times)*(get_weapon_coolingTime(ENEMY.weaponTypeName)) - (get_weapon_coolingTime(ENEMY.weaponTypeName));

		if (time_for_death_rush<time_before_death)
		{
			//对于像光棱这样慢速的，可以早早冲了
			return true;
		}
	}

	return false;
}


//绞肉机的状态躲避和逼近方法：
void RobotAI::HideAndApproach(RobotAI_Order & order)
{
	//躲在障碍物后面并伺机逼近

	int i;

	if ((ENEMY.weaponTypeName == WT_MissileLauncher || ENEMY.weaponTypeName == WT_PlasmaTorch) && battle_info.num_bullet>0)
	{
		//对手用跟踪弹或等离子得用这种躲避方法
		double nearest_dist_sq = 1e20;
		Vector2D vec_bullet, vec_nearest;
		double current_dist_sq;
		int nearest_bullet_idx = 0;

		for (i = 0; i<battle_info.num_bullet; i++)
		{
			//TODO:子弹的发射者？？这个怎么会没有啊。。。
			if (IsEnemyBullet(i))
			{
				vec_bullet.x = battle_info.bulletInformation[i].circle.x;
				vec_bullet.y = battle_info.bulletInformation[i].circle.y;

				current_dist_sq = Vec2DDistanceSq(vec_me, vec_bullet);
				if (current_dist_sq<nearest_dist_sq)
				{
					nearest_dist_sq = current_dist_sq;
					vec_nearest = vec_bullet;
					nearest_bullet_idx = i;
				}
			}
		}

		Hide(order, battle_info.bulletInformation[nearest_bullet_idx].circle);

	}
	else
	{
		Hide(order, ENEMY.circle);
	}




	//TODO:伺机逼近	
}


void RobotAI::Crusher_Attack(RobotAI_Order & order)
{

	DirectMoveToPoint(order, ENEMY.circle.x, ENEMY.circle.y);

}


void RobotAI::Reload(RobotAI_Order & order)
{
	int select_arsenal_idx = 0;
	if (battle_info.arsenal[0].respawning_time == 0 && battle_info.arsenal[1].respawning_time == 0)
	{
		//两个军火库都能补充弹药

		//补个弹药
		double d0, d1;//距离两个军火库的距离
		//Sq是平方的距离
		d0 = Vec2DDistanceSq(vec_me, vec_arsenal0);
		d1 = Vec2DDistanceSq(vec_me, vec_arsenal1);

		if (d0<d1)
		{
			//简单评估军火库风险
			if (Vec2DDistanceSq(vec_enemy, vec_arsenal0)>danger_distance*danger_distance)
			{
				select_arsenal_idx = 0;
			}
			else
			{
				select_arsenal_idx = 1;
			}
		}
		else
		{
			if (Vec2DDistanceSq(vec_enemy, vec_arsenal1)>danger_distance*danger_distance)
			{
				select_arsenal_idx = 1;
			}
			else
			{
				select_arsenal_idx = 0;
			}
		}
	}
	else
	{
		//不是每个都能补
		if (battle_info.arsenal[0].respawning_time == 0)
		{
			select_arsenal_idx = 0;
		}
		else if (battle_info.arsenal[1].respawning_time == 0)
		{
			select_arsenal_idx = 1;
		}
		else
		{
			//没有当前可以补的
			//先躲着
			Hide(order, ENEMY.circle);
			return;
		}
	}


	//TODO:路径规划
	if (!IsArriveAtPoint(battle_info.arsenal[select_arsenal_idx].circle.x, battle_info.arsenal[select_arsenal_idx].circle.y))
	{
		DirectMoveToPoint(order, battle_info.arsenal[select_arsenal_idx].circle.x, battle_info.arsenal[select_arsenal_idx].circle.y);
	}

}

//Crusher的状态转换函数
void RobotAI::StateTransform()
{
	switch (state)
	{
	case 0:
		//躲藏状态
		if (IsSafeToRush())
		{
			//哈哈，干不掉老子了，冲啊！！！！！
			state = 1;
		}
		else if (Vec2DDistanceSq(vec_me, vec_enemy)<crusher_death_rush*crusher_death_rush)
		{
			//兄弟你靠的太近了,直接冲上去操翻它
			state = 1;
		}
		else
		{
			//接着怂
			state = 0;
		}

		break;

	case 1:
		if (MY.remainingAmmo<1)
		{
			state = 2;	//补充弹药去
		}
		else if (ENEMY.remainingAmmo == get_weapon_ammo(ENEMY.weaponTypeName))
		{
			//对方可能重新补充了弹药
			if (!IsSafeToRush())
			{
				state = 0;
			}
		}
		else if (get_engine_maxSpeed(ENEMY.engineTypeName)>get_engine_maxSpeed(MY.engineTypeName))
		{
			//可能追不上被戏耍
			if (!IsSafeToRush())
			{
				state = 0;
			}
		}
		break;

	case 2:
		//状态转换
		if (MY.remainingAmmo >= 5)
		{
			state = 0;
		}
	}

	if (ENEMY.weaponTypeName == WT_MissileLauncher && Vec2DDistance(vec_me, vec_enemy) < 500)	state = 1;
}