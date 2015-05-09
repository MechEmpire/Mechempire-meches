#include <iostream>
#include <algorithm>
#include "RobotAI.h"

RobotAI::RobotAI()
{
	crusher_death_rush=250;	//绞肉机死亡冲刺距离


	tmp_state=0;	//临时用来有限状态机(0起始状态)

	to_point = 0;

	min_distance=5;
	min_rotation=6;
	danger_distance=100;


	moving=true;//temp
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
	

	UpdateBattlefieldInfo(info);

	//AimAtPoint(order,ENEMY.circle.x,ENEMY.circle.y);
	//order.fire = 1;
	//order.run = 2;
















	
	
	//-----------------------------------------------------------------------------------------------
	//猥琐系(飞弹)状态进化

	switch(tmp_state)
	{
	case 0:
		//起始态
		//躲避
		HideAndApproach(order);
		Aim_Missile(order);
		Fire(order);
		break;
	case 1:
		//补子弹
		Reload(order);
		AimAtPoint(order,ENEMY.circle.x,ENEMY.circle.y);
		Fire(order);
		break;
	default:
		break;
	}
	
	if(MY.remainingAmmo<=1 && ENEMY.weaponTypeName!=WT_ElectricSaw)
	{
		//敌人是电锯没子弹了也得在障碍物绕
		tmp_state=1;
	}
	else
	{
		tmp_state=0;
	}
	








	/*
	
	//---------------------------------------------------------------------------------------------
	//绞肉机 Crusher
	//玩玩。。。
	//导航图啊啥时能正常？
	
	Attack_ElectricSaw(order);

	switch(tmp_state)
	{
	case 0:
		//躲在障碍物后面伺机逼近
		HideAndApproach(order);
		
		break;

	case 1:
		//杀出去
		Crusher_Attack(order);
		break;

	case 2:
		//补子弹
		Reload(order);
		
		break;

	default:
		tmp_state=0;
		HideAndApproach(order);
		break;
	}

	Crusher_StateTransform();

	*/
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

	weapon=WT_MissileLauncher;	
	//weapon = WT_Apollo;
	engine=ET_Robotman;
	//engine = ET_Spider;
	//weapon = WT_Apollo;
	//engine = ET_Shuttle;
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	//return "太空要塞";
	//return "Fuck you";
	return "猥琐的狄学长";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "狄学长";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return 30;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 10;
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
	return 30;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 0;
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
	//cout<<"型号："<<GetName()<<"\n";

	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	tmp_state=0;


	to_point=0;
	battle_info=info;

	//记录自己ID，获取敌人ID（两人局）
	this->myID=myID;

	enemyID=1;
	if(myID==1)
	{
		enemyID=0;
	}
	//////////////////////////////////



	//arsenal
	vec_arsenal0.x=battle_info.arsenal[0].circle.x;
	vec_arsenal0.y=battle_info.arsenal[0].circle.y;

	vec_arsenal1.x=battle_info.arsenal[1].circle.x;
	vec_arsenal1.y=battle_info.arsenal[1].circle.y;

	///////////
	wandering_point.clear();
	AddWanderPoint(vec_arsenal0);
	AddWanderPoint(vec_arsenal1);

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

void RobotAI::AddWanderPoint(Vector2D v)
{
	
	wandering_point.push_back(v);

}


void RobotAI::UpdateBattlefieldInfo(const RobotAI_BattlefieldInformation& info)
{
	battle_info=info;

	vec_me.x=MY.circle.x;
	vec_me.y=MY.circle.y;

	vec_enemy.x=ENEMY.circle.x;
	vec_enemy.y=ENEMY.circle.y;
}

bool sort_greater_bullet_distancesq_idx(const BulletDisSort& a,const BulletDisSort& b)
{
	return a.dissq>b.dissq;
}

int RobotAI::NearestBulletIdx(int idx)
{
	//idx 最近的第几个敌方子弹
	if(idx>=battle_info.num_bullet)
	{
		idx=0;
	}
	return bullet_sort.at(idx).idx;
}

void RobotAI::UpdateBulletInfoVector()
{
	//更新子弹按距离排序下标的vec
	//只在bullet_num更新才重新更新,以节省时间?
	//或定时长更新


	int i;
	//int j=0;
	for(i=0;i<battle_info.num_bullet;i++)
	{
		if(IsEnemyBullet(i))
		{
			//bullet_distancesq_sort_idx.at(i)=i;
			bullet_sort.at(i).idx=i;
			Vector2D dis(vec_me.x-battle_info.bulletInformation[i].circle.x
				,vec_me.y-battle_info.bulletInformation[i].circle.y);
			bullet_sort.at(i).dissq=dis.LengthSq();

			//j++;
		}
		else
		{
			//是我的子弹
			bullet_sort.at(i).idx=0;
			bullet_sort.at(i).dissq=1e20;
		}
	}
	sort(bullet_sort.begin(),bullet_sort.begin()+battle_info.num_bullet,sort_greater_bullet_distancesq_idx);



}

//通用基础方法

void RobotAI::Fire(RobotAI_Order& order)
{
	if(MY.weaponTypeName == WT_MissileLauncher)
	{
		//currently two man battle
		order.fire = enemyID + 1;
	}
	else
	{
		order.fire=1;
	}
}

bool RobotAI::AimAtPoint(RobotAI_Order &order,const double &x,const double &y)
{
	//返回是否瞄准转动结束
	//瞄准点(x,y)
	//double rotation=AnglePlus(MY.engineRotation,MY.weaponRotation);
	//2014_3_1
	//武器角度独立
	double rotation=MY.weaponRotation;

	double rr=atan2(y-MY.circle.y,x-MY.circle.x);
	rr=RadianToAngle(rr);

	double c=0,a=0;		//clockwise顺时针，anticlockwise逆时针

	if(rr>rotation)
	{
		c=rr-rotation;
		a=rotation+360-rr;	//(rotation-(-180))+(180-rr)
	}
	else
	{
		a=rotation-rr;
		c=360-rotation+rr;	//(180-rotation)+(rr-(-180));
	}

	order.wturn=0;
	if(c>a)
	{
		if(abs(a)>get_weapon_rotationSpeed(MY.weaponTypeName))
		{
			order.wturn=-1;
			return false;
		}
	}
	else
	{
		if(abs(c)>get_weapon_rotationSpeed(MY.weaponTypeName))
		{
			order.wturn=1;
			return false;
		}
	}
	return true;
	//这么搞还滞后一帧的瞄准啊。。。
}








void RobotAI::AimWithAnticipation(RobotAI_Order & order,const double & x,const double & y)
{
	//普通武器的瞄准策略(带预判)
	//子弹的静态速度又给漏了。。。
	//TODO
	//temp
	//首先预判瞄准点
	//其实只能对付敌人
	Vector2D vec_target(ENEMY.circle.x,ENEMY.circle.y);

	Vector2D to_evader=vec_target-vec_me;

	Vector2D vec_v_enemy(ENEMY.vx,ENEMY.vy);

	
	double lookahead_time=to_evader.Length()/(get_bullet_speed((bullettypename)MY.weaponTypeName)+vec_v_enemy.Length());

	Vector2D destination=vec_target+vec_v_enemy*lookahead_time;

	AimAtPoint(order,destination.x,destination.y);
	
}

void RobotAI::Aim_Missile(RobotAI_Order & order)
{
	Vector2D vec_aim=vec_enemy-vec_me;
	double rr=AnglePlus(90,RadianToAngle(atan2(vec_aim.y,vec_aim.x)));

	double c=0,a=0;		//clockwise顺时针，anticlockwise逆时针

	double rotation=MY.weaponRotation;

	if(rr>rotation)
	{
		c=rr-rotation;
		a=rotation+360-rr;	//(rotation-(-180))+(180-rr)
	}
	else
	{
		a=rotation-rr;
		c=360-rotation+rr;	//(180-rotation)+(rr-(-180));
	}

	order.wturn=0;
	if(c>a)
	{
		if(abs(a)>get_weapon_rotationSpeed(MY.weaponTypeName))
		{
			order.wturn=-1;
		}
	}
	else
	{
		if(abs(c)>get_weapon_rotationSpeed(MY.weaponTypeName))
		{
			order.wturn=1;
		}
	}
}






void RobotAI::DirectMoveToPoint(RobotAI_Order &order,const double &x,const double&y)
{
	switch(battle_info.robotInformation[myID].engineTypeName)
	{
	case ET_Spider:
	case ET_Quad:
		DirectMoveToPoint_Spider(order,x,y);
		break;
	case ET_Robotman:
	case ET_Shuttle:
		DirectMoveToPoint_Robotman(order,x,y);break;
	case ET_AFV:
	case ET_GhostTank:
		DirectMoveToPoint_AFV(order,x,y);break;
	case ET_UFO:
	case ET_XCraft:
		DirectMoveToPoint_UFO(order,x,y);break;
	}
}




//Spider
//order.run=1 向左行进
//order.run=2 向右行进
//order.run=3 向上行进
//order.run=4 向下行进
//order.run=otherwise 停止
//order.etrun 不起作用
void RobotAI::DirectMoveToPoint_Spider(RobotAI_Order &order,const double &x,const double&y)
{
	order.run=0;	//默认停
	double dx=x-MY.circle.x;
	double dy=y-MY.circle.y;

	if(abs(dx)>abs(dy))
	{
		//左右移动
		if(dx<0)
		{
			order.run=1;
		}
		else if(dx>0)
		{
			order.run=2;
		}
	}
	else
	{
		//上下移动
		if(dy<0)
		{
			order.run=3;
		}
		else if(dy>0)
		{
			order.run=4;
		}
	}
}



int RobotAI::AdjustDirection(const double &x,const double &y)
{
	//使得载具朝向指向点（x,y）
	//return 需要顺时针旋转 1
	//			逆时针旋转	-1
	//		在范围之内（暂时不转） 0
	double rotation=MY.engineRotation;

	double rr=atan2(y-MY.circle.y,x-MY.circle.x);
	rr=RadianToAngle(rr);

	double c=0,a=0;		//clockwise顺时针，anticlockwise逆时针

	if(rr>rotation)
	{
		c=rr-rotation;
		a=rotation+360-rr;	//(rotation-(-180))+(180-rr)
	}
	else
	{
		a=rotation-rr;
		c=360-rotation+rr;	//(180-rotation)+(rr-(-180));
	}

	if(c>min_rotation && a>min_rotation)
	{
		if(c>a)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}




//Robotman
//order.run>0 全速前进
//order.run<0 全速后退
//order.run=0 立即停止
//order.eturn<0 逆时针转动
//order.eturn>0 顺时针转动
//order.eturn=0 停止转动（只有此时才能径向移动）
void RobotAI::DirectMoveToPoint_Robotman(RobotAI_Order &order,const double &x,const double&y)
{
	order.eturn=0;

	double rr=RadianToAngle(atan2(y-vec_me.y,x-vec_me.x));	//目标正向方向

	double rotation=MY.engineRotation;

	double c=0,a=0;		//clockwise, anti-clockwise

	bool forward_or_backward=true;

	//TODO:判断方向，选择采取前进还是后退
	if(rr>rotation)
	{
		c=rr-rotation;
		a=rotation+360-rr;	//(rotation-(-180))+(180-rr)
	}
	else
	{
		a=rotation-rr;
		c=360-rotation+rr;	//(180-rotation)+(rr-(-180));
	}


	//这里a,c都肯定为正，其实
	order.eturn=0;
	if(c>a)
	{
		if(abs(a)>90)
		{
			//倒着走
			forward_or_backward=false;
			if(180-a>get_engine_rotationSpeed(MY.engineTypeName))
			{
				order.eturn=1;
			}
		}
		else
		{
			forward_or_backward=true;
			if(abs(a)>get_engine_rotationSpeed(MY.engineTypeName))
			{
				order.eturn=-1;
			}
		}
	}
	else
	{
		if(abs(c)>90)
		{
			//倒着走
			forward_or_backward=false;
			if(180-c>get_engine_rotationSpeed(MY.engineTypeName))
			{
				order.eturn=-1;
			}
		}
		else
		{
			forward_or_backward=true;
			if(abs(c)>get_engine_rotationSpeed(MY.engineTypeName))
			{
				order.eturn=1;
			}
		}
	}







	//order.eturn=AdjustDirection(x,y);

	if(order.eturn==0)
	{
		if(forward_or_backward)
		{
			order.run=1;
		}
		else
		{
			order.run=-1;
		}
		
	}

}

//装甲车（AFV）
	//最正常的游戏中的vehicle, vr正比于当前速度
	//acceleration为加速比例，(0,1)
	//m>0加速,<=0刹车
//最正常的游戏中的vehicle, vr正比于当前速度，转向可以与加速同时操作
	//mnplt>0顺时针,<0逆时针,0不转
void RobotAI::DirectMoveToPoint_AFV(RobotAI_Order &order,const double &x,const double&y)
{
	//TODO
	//int flag_turn=AdjustDirection(x,y);
	const double min_accelerate_angle=50;

	double rotation=MY.engineRotation;

	double rr=atan2(y-MY.circle.y,x-MY.circle.x);
	rr=RadianToAngle(rr);

	double c=0,a=0;		//clockwise顺时针，anticlockwise逆时针


	order.run=1;
	
	if(rr>rotation)
	{
		c=rr-rotation;
		a=rotation+360-rr;	//(rotation-(-180))+(180-rr)
	}
	else
	{
		a=rotation-rr;
		c=360-rotation+rr;	//(180-rotation)+(rr-(-180));
	}

	if(c>min_rotation && a>min_rotation)
	{
		if(c>a)
		{
			order.eturn=-1;
			if(a>min_accelerate_angle && MY.vx+MY.vy>0.1)
			{
				order.run=0;
			}
		}
		else
		{
			order.eturn=1;
			if(c>min_accelerate_angle && MY.vx+MY.vy>0.1)
			{
				order.run=0;
			}
		}
	}
	else
	{
		order.eturn=0;
	}
	

	//TODO
	//当目标离机甲较远时，同时行进和转向
	//当目标离机甲较近时，可能要刹车+转向

	//这里的一个半径需要微调



	//temp
	//order.eturn=flag_turn;
	//order.run=1;
}


//幽浮(UFO)
	//飘移悬浮式，加速度与速度完全分开
	//==0不加速，其他沿rotation反向喷气加速
//>0顺时针,<0逆时针
void RobotAI::DirectMoveToPoint_UFO(RobotAI_Order &order,const double &x,const double&y)
{
	//TODO
	

}


void RobotAI::DirectMoveInDirection(RobotAI_Order& order, const double & r)
{
	switch(MY.engineTypeName)
	{
	case ET_Spider:
	case ET_Quad:
		DirectMoveInDirection_Spider(order,r);break;
	case ET_Robotman:
	case ET_Shuttle:
		DirectMoveInDirection_Robotman(order,r);break;
	case ET_AFV:
	case ET_GhostTank:
		DirectMoveInDirection_AFV(order,r);break;
	case ET_UFO:
	case ET_XCraft:
		DirectMoveInDirection_UFO(order,r);break;
	}
}

void RobotAI::DirectMoveInDirection_Spider(RobotAI_Order& order, const double & r)
{
	double rr=AngleToRadian(r);
	double xx=50*cos(rr)+MY.circle.x;
	double yy=50*sin(rr)+MY.circle.y;

	DirectMoveToPoint_Spider(order,xx,yy);
}

void RobotAI::DirectMoveInDirection_Robotman(RobotAI_Order& order, const double & r)
{
	double rr=AngleToRadian(r);
	double xx=50*cos(rr)+MY.circle.x;
	double yy=50*sin(rr)+MY.circle.y;

	DirectMoveToPoint_Robotman(order,xx,yy);
}

void RobotAI::DirectMoveInDirection_AFV(RobotAI_Order& order, const double & r)
{

}

void RobotAI::DirectMoveInDirection_UFO(RobotAI_Order& order, const double & r)
{

}






void RobotAI::Attack_ElectricSaw(RobotAI_Order&order)
{
	AimAtPoint(order,ENEMY.circle.x,ENEMY.circle.y);
	
	//...居然没提供获取weapon半径的接口
	if(Vec2DDistanceSq(vec_me,vec_enemy)<=(95+MY.circle.r)*(95+MY.circle.r))	//电锯半径
	{
		Fire(order);
	}
}











/*
void EscapeFromBeam(RobotAI_Order&order,const Beam & b)
{
	double vertical_rotation=AnglePlus(b.rotation,90);
	//TODO
}
*/




void RobotAI::DirectMoveAwayFromPoint(RobotAI_Order &order,const double &x,const double &y)
{
	switch(MY.engineTypeName)
	{
	case ET_Spider:DirectMoveAwayFromPoint_Spider(order,x,y);break;
	//case ET_Robotman:DirectMoveToPoint_Robotman(order,x,y);break;
	//case ET_AFV:DirectMoveToPoint_AFV(order,x,y);break;
	//case ET_UFO:DirectMoveToPoint_UFO(order,x,y);break;
	}
}

void RobotAI::DirectMoveAwayFromPoint_Spider(RobotAI_Order &order,const double &x,const double &y)
{
	order.run=0;	//默认停
	double dx=x-MY.circle.x;
	double dy=y-MY.circle.y;

	if(abs(dx)>abs(dy))
	{
		//左右移动
		if(dx<0)
		{
			order.run=2;
		}
		else if(dx>0)
		{
			order.run=1;
		}
	}
	else
	{
		//上下移动
		if(dy<0)
		{
			order.run=4;
		}
		else if(dy>0)
		{
			order.run=3;
		}
	}
}


void RobotAI::EscapeFromBeam(RobotAI_Order&order,const Beam &b)
{
	
	double rr=AngleToRadian(b.rotation);


	//TODO:哪边可以畅通行进
	double rr_v=AngleToRadian(AnglePlus(90,b.rotation));

	double rrvr=RadianToAngle(rr_v);
	AngleAdjust(rrvr);

	/*
	//临时
	if(rrvr<0 && MY.circle.y<200)
	{
		rrvr=AnglePlus(rrvr,180);
	}
	else if(rrvr>0 && MY.circle.y>400)
	{
		rrvr=AnglePlus(rrvr,180);
	}
	*/

	DirectMoveInDirection(order,rrvr);
	//TODO:更贴近那一边

	//TODO：除了蜘蛛，其他以当前方向为躲避方向的主要考虑因素

	/*
	Vector2D bm;
	bm.x=cos(rr);
	bm.y=sin(rr);

	Vector2D bm_v;
	bm_v.x=cos(rr_v);
	bm_v.y=sin(rr_v);
	*/
}




int RobotAI::HitTestBulletBeam(const RobotAI_BulletInformation& bi)
{
	Beam t;
	t.x=bi.circle.x;
	t.y=bi.circle.y;
	t.rotation=RadianToAngle(atan2(bi.vy,bi.vx));
	//return HitTestBeamCircle(t,MY.circle);

	Point p;
	Point hit_point_nearest;
	hit_point_nearest.x=9999;
	hit_point_nearest.y=9999;

	int robot_or_obstacle=-99;//i   obstacle,-1自己
	int i;

	if(HitTestBeamCircle(t,MY.circle))
	{
		p=GetHitPoint(t,MY.circle);
		robot_or_obstacle=-1;
	}


	for(i=0;i<battle_info.num_obstacle;i++)
	{
		if(HitTestBeamCircle(t,battle_info.obstacle[i]))
		{
			p=GetHitPoint(t,battle_info.obstacle[i]);


			if(abs(p.x-t.x)<abs(hit_point_nearest.x-t.x))
			{
				hit_point_nearest=p;
				//标下标
				robot_or_obstacle=-i;//obstacle
			}
		}
	}

	//这里想到，是不是有个基本动作时 escape_from_beam
	//沿对方瞄准射线的径向走动

	return robot_or_obstacle;

}



void RobotAI::PursuitEnemy(RobotAI_Order & order)
{
	//预判追逐
	
	Vector2D to_evader=vec_enemy-vec_me;

	Vector2D vec_v_enemy;
	vec_v_enemy.x=ENEMY.vx;
	vec_v_enemy.y=ENEMY.vy;

	//?判断是否迎面而来？


	double lookahead_time=to_evader.Length()/(get_engine_maxSpeed(MY.engineTypeName)+vec_v_enemy.Length());

	Vector2D destination=vec_enemy+vec_v_enemy*lookahead_time;

	DirectMoveToPoint(order,destination.x,destination.y);
}













Vector2D RobotAI::GetHidingPosition(const Circle &o,const Circle & hunter)
{
	double distance_from_boundary=MY.circle.r+3;
	
	double distance_away=distance_from_boundary+o.r;

	//早晚得用上 Vector2D进来。。
	
	Vector2D obstacle;
	obstacle.x=o.x;
	obstacle.y=o.y;

	Vector2D vec;
	vec.x=o.x-hunter.x;
	vec.y=o.y-hunter.y;
	vec.Normalize();
	
	Vector2D target=vec*distance_away+obstacle;

	return target;

}



void RobotAI::Hide(RobotAI_Order & order,const Circle & hunter)
{
	//TODO:当对方是跟踪弹时要使用另一种策略

	double dist_to_closest=1e20;
	Vector2D best_hiding_position;

	int i;


	for(i=0;i<battle_info.num_obstacle;i++)
	{
		Vector2D hiding_position=GetHidingPosition(battle_info.obstacle[i],hunter);

		//距离最短视作最好的躲避点
		double dist=Vec2DDistanceSq(hiding_position,vec_me);

		if(dist<dist_to_closest)
		{
			dist_to_closest=dist;
			best_hiding_position=hiding_position;
		}


	}

	if(dist_to_closest==1e20)
	{
		//没有合适障碍物
		//TODO:refine躲开
		DirectMoveAwayFromPoint(order,hunter.x,hunter.y);
	}


	//TODO:refine 路径规划
	if(!IsArriveAtPoint(best_hiding_position.x,best_hiding_position.y))
	{
		DirectMoveToPoint(order,best_hiding_position.x,best_hiding_position.y);
	}



}


//这种打法经测试不靠谱。。。
void RobotAI::SafeApproach(RobotAI_Order& order)
{
	//使用离对方近的障碍物躲避逼近
	double dist_to_closest=1e20;
	Vector2D best_hiding_position;

	int i;
	int o_idx=0;

	for(i=0;i<battle_info.num_obstacle;i++)
	{
		Vector2D vec_o;
		vec_o.x=battle_info.obstacle[i].x;
		vec_o.y=battle_info.obstacle[i].y;
		//Vector2D hiding_position=GetHidingPosition(battle_info.obstacle[i],hunter);

		//使用离对方最近的障碍物
		double dist=Vec2DDistanceSq(vec_o,vec_enemy);

		if(dist<dist_to_closest)
		{
			dist_to_closest=dist;
			o_idx=i;
		}

	}
	if(dist_to_closest==1e20)
	{
		//没有合适障碍物
		//TODO:refine躲开
		DirectMoveAwayFromPoint(order,vec_enemy.x,vec_enemy.y);
	}


	//障碍物已经选好
	//先保持三点一线
	Vector2D vec_safe;
	vec_safe.x=vec_enemy.x-battle_info.obstacle[o_idx].x;
	vec_safe.y=vec_enemy.y-battle_info.obstacle[o_idx].y;

	//安全扇形区域一半的sin值
	double ss=battle_info.obstacle[o_idx].r/vec_safe.Length();


	
	vec_safe.Normalize();

	Vector2D vec_cur=vec_enemy-vec_me;	//当前我与敌方机甲的向量
	Vector2D vec_cur_native=vec_cur;
	vec_cur.Normalize();

	double cc=vec_safe.Dot(vec_cur);	//标准化点积结果（cos）
	if(  pow(cc,2)<1-ss*ss    )	
	{
		//安全扇形区域内
		Vector2D hiding_poisition=GetHidingPosition(battle_info.obstacle[o_idx],ENEMY.circle);
		if(!IsArriveAtPoint(hiding_poisition.x,hiding_poisition.y))
		{
			DirectMoveToPoint(order,hiding_poisition.x,hiding_poisition.y);
		}
		return;
	}
	else
	{
		//先走到安全区域中
		double r_c=atan2(vec_cur.y,vec_cur.x);
		double r_t=atan2(vec_safe.y,vec_safe.x);

		vec_safe*=vec_cur_native.Length()*sqrt(1-ss*ss);

		DirectMoveToPoint(order,vec_safe.x+vec_enemy.x,vec_safe.y+vec_enemy.y);
	}




	//TODO:refine 路径规划
	if(!IsArriveAtPoint(best_hiding_position.x,best_hiding_position.y))
	{
		DirectMoveToPoint(order,best_hiding_position.x,best_hiding_position.y);
	}

}


void RobotAI::PathFollow(RobotAI_Order& order)
{
	//巡逻到点idx=to_point
	if(IsArriveAtPoint(wandering_point.at(to_point).x,wandering_point.at(to_point).y))
	{
		to_point++;
		to_point=to_point%wandering_point.size();
		DirectMoveToPoint(order,wandering_point.at(to_point).x,wandering_point.at(to_point).y);
	}
	else
	{
		DirectMoveToPoint(order,wandering_point.at(to_point).x,wandering_point.at(to_point).y);
	}
}





//判断部分

bool RobotAI::IsArriveAtPoint(const double &x, const double &y)
{
	//<的参数需要根据情况调整
	return abs(MY.circle.x-x)+abs(MY.circle.y-y)<min_distance;
}

bool RobotAI::IsEnemyBullet(int bullet_idx)
{
	return enemyID == battle_info.bulletInformation[bullet_idx].launcherID;
	//return (enemyID==0 && battle_info.bulletInformation[bullet_idx].circle.r<-998) || (enemyID==1 && battle_info.bulletInformation[bullet_idx].circle.r>998);
}

bool RobotAI::IsSafeToRush()
{
	int times=1;
	if(ENEMY.weaponTypeName==WT_Shotgun)
	{
		times=5;
	}

	if(ENEMY.remainingAmmo*(times*(get_bullet_damage((bullettypename)ENEMY.weaponTypeName))+battle_info.num_bullet)<MY.hp)
	{
		//剩余子弹和还在飞的子弹全部加起来也打不死我
		return true;
	}
	else //if(ENEMY.hp>MY.hp)
	{
		//死亡冲刺
		Vector2D vec_dist=vec_enemy-vec_me;
		double dist=vec_dist.Length();
		int time_for_death_rush=dist/(get_engine_maxSpeed(MY.engineTypeName));
		
		//这种算法恐怕不是太保险
		//int time_before_death=MY.hp/(get_weapon_damage(ENEMY.weaponTypeName))*(get_weapon_coolingTime(ENEMY.weaponTypeName));
		
		//多减一个时间看看
		int time_before_death=MY.hp/(get_bullet_damage((bullettypename)ENEMY.weaponTypeName)*times)*(get_weapon_coolingTime(ENEMY.weaponTypeName))-(get_weapon_coolingTime(ENEMY.weaponTypeName));
		
		if(time_for_death_rush<time_before_death)
		{
			//对于像光棱这样慢速的，可以早早冲了
			return true;
		}
	}

	return false;
}



bool RobotAI::IsOkToFire()
{
	int i;

	Vector2D vec_aim=vec_enemy-vec_me;

	//电锯这里必须时时瞄准啊，没判断瞄准
	if(MY.weaponTypeName==WT_ElectricSaw)
	{
		if(vec_aim.LengthSq()<=pow(95+ENEMY.circle.r,2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	
	Beam b;
	b.x=vec_me.x;
	b.y=vec_me.y;
	b.rotation=RadianToAngle(atan2(vec_aim.y,vec_aim.x));

	Point nearest_hit_point;
	



	if(abs(MY.weaponRotation-b.rotation)<min_rotation)
	{
		if(HitTestBeamCircle(b,ENEMY.circle))
		{
			nearest_hit_point=GetHitPoint(b,ENEMY.circle);
			for(i=0;i<battle_info.num_arsenal;i++)
			{
				if(HitTestBeamCircle(b,battle_info.obstacle[i]))
				{
					if(  abs(GetHitPoint(b,battle_info.obstacle[i]).x-MY.circle.x)<abs(ENEMY.circle.x-MY.circle.x)    )
					{
						return false;
					}
				}
			}
			return true;
		}
	}
	return false;
}


bool RobotAI::IsBeAimed()
{
	//和IsOkToFire正好反着来

	int i;

	Vector2D vec_aim=vec_me-vec_enemy;

	//电锯这里必须时时瞄准啊，没判断瞄准
	if(MY.weaponTypeName==WT_ElectricSaw)
	{
		if(vec_aim.LengthSq()<=pow(95+MY.circle.r,2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	
	Beam b;
	b.x=vec_enemy.x;
	b.y=vec_enemy.y;
	b.rotation=RadianToAngle(atan2(vec_aim.y,vec_aim.x));

	Point nearest_hit_point;
	



	if(abs(ENEMY.weaponRotation-b.rotation)<min_rotation)
	{
		if(HitTestBeamCircle(b,MY.circle))
		{
			nearest_hit_point=GetHitPoint(b,MY.circle);
			for(i=0;i<battle_info.num_arsenal;i++)
			{
				if(HitTestBeamCircle(b,battle_info.obstacle[i]))
				{
					if(  abs(GetHitPoint(b,battle_info.obstacle[i]).x-ENEMY.circle.x)<abs(MY.circle.x-ENEMY.circle.x)    )
					{
						return false;
					}
				}
			}
			return true;
		}
	}
	return false;

}


//////////////////////////////////






/*

//Nav graph, 跑了崩溃暂时消失

void RobotAI::ConstructNavGraph()
{
	//难道手动构建粗粒导航图吗


	//3_11,现在这个一跑就死了


	//以battle_info创建导航图
	//用8洪水填充算法来做(要栈吧)
	//(似没有必要且低效(边也不好处理)，一行一行扫就行了，只需要避开球形Obstacle)


	Vector2D current;

	const double root2=sqrt(2);

	current.x=0+MY.circle.r;
	current.y=0+MY.circle.r;

	int row=0,col=0;
	//int row_num,col_num;

	//到这是好的
	//nav_graph.Save("【调试】空图");

	//尝试：先生成全部点的图，再把在障碍中的点删除
	while(current.x<battle_info.boundary.width-MY.circle.r*1.5)
	{
		//一行刷点
		while(current.y<battle_info.boundary.height-MY.circle.r*1.5)
		{
			//一列刷点
			NavGraphNode<int> newly_add(nav_graph.GetNextFreeNodeIndex(),current);
			nav_graph.AddNode(newly_add);

			//和左和上的点生成边（0的边界处理）
			
			if(col>0)
			{
				//左边的点
				nav_graph.AddEdge(NavGraphEdge(newly_add.Index(),newly_add.Index()-row_num,gap));
			}

			if(row>0)
			{
				//上面的点
				nav_graph.AddEdge(NavGraphEdge(newly_add.Index(),newly_add.Index()-1,gap));
			}

			if(row>0 && col>0)
			{
				//左上的点
				nav_graph.AddEdge(NavGraphEdge(newly_add.Index(),newly_add.Index()-row_num-1,gap*root2));
			}

			if(row>0 && col<col_num-1)
			{
				//左下的点
				nav_graph.AddEdge(NavGraphEdge(newly_add.Index(),newly_add.Index()-row_num+1,gap*root2));
			}

			current.y+=gap;
			row++;
		}
		row_num=row;
		row=0;
		current.x+=gap;
		col++;
	}

	col_num=col;

	//这里还有一个格子分割图
	//row_num,col_num
	//  /gap  =row_id+row_num*col_id

	////////////////////////

	nav_graph.Save("【调试】最初网格");





	//这是还要重新遍历一遍吗。。算法真差
	int num_nodes=nav_graph.NumNodes();
	int i;
	for(i=0;i<num_nodes;i++)
	{
		if(!ConstructNavGraph_ValidPosition(nav_graph.GetNode(i).Pos()))
		{
			//删掉这个点
			nav_graph.RemoveNode(i);
		}
	}
	nav_graph.Save("【调试】删点网格");

	//TODO:标记军火库
	Vector2D point_arsenal;
	int nearest_point_id;
	for(i=0;i<battle_info.num_arsenal;i++)
	{
		point_arsenal.x=battle_info.arsenal[i].circle.x;
		point_arsenal.y=battle_info.arsenal[i].circle.y;
		nearest_point_id=NavGraph_NearestNode(point_arsenal);
		nav_graph.GetNode(nearest_point_id).SetExtraInfo(NODE_FLAG_ARSENAL);


		//TRACE("军火库"+IntToString(i)+"  ("+IntToString((int)point_arsenal.x)+","+IntToString((int)point_arsenal.y)+")\r");
	}

	nav_graph.Save("【调试】标军火库网格");
}


bool RobotAI::ConstructNavGraph_ValidPosition(const Vector2D & point)
{
	int i;
	Vector2D ob;
	for(i=0;i<battle_info.num_obstacle;i++)
	{
		if(point.x>battle_info.obstacle[i].x-battle_info.obstacle[i].r
			&& point.x<battle_info.obstacle[i].x+battle_info.obstacle[i].r)
		{
			if(point.y>battle_info.obstacle[i].y-battle_info.obstacle[i].r
			&& point.y<battle_info.obstacle[i].y+battle_info.obstacle[i].r)
			{
				//within the square area of the obstacle
				ob.x=battle_info.obstacle[i].x;
				ob.y=battle_info.obstacle[i].y;
				double dis=Vec2DDistanceSq(point,ob);
				
				if(dis<battle_info.obstacle[i].r*battle_info.obstacle[i].r)
				{
					//not valid
					return false;
				}

			}
		}
	}
	return true;
}


int RobotAI::NavGraph_NearestNode(const Vector2D & point)
{
	int row_id,col_id;

	//这里还有一个格子分割图
	//row_num,col_num
	//  /gap  =row_id+row_num*col_id=point_id

	//2014_3_11:这个算法靠不住啊
	//边界有剩余
	row_id=(int)(point.y-MY.circle.r)/gap;
	col_id=(int)(point.x-MY.circle.r)/gap;

	int box_id=row_id+row_num*col_id;

	//这个格子四周的节点的下标
	//左上：box_id
	//左下：box_id+1
	//右上：box_id+col_num
	//右下：box_id+col_num+1
	int id_array[4]={box_id,box_id+1,box_id+col_num,box_id+col_num+1};

	double min_dis_sq=MaxDouble,temp;
	int nearest_point_id=id_array[0];
	int i;
	for(i=0;i<3;i++)
	{
		temp=Vec2DDistanceSq(point,nav_graph.GetNode(id_array[i]).Pos());
		if(min_dis_sq>temp)
		{
			min_dis_sq=temp;
			nearest_point_id=id_array[i];
		}
	}

	return nearest_point_id;
}

*/







//猥琐系（跟踪弹）的状态执行函数

void RobotAI::Scared_HideAndFlee(RobotAI_Order & order)
{
	//TODO
}

void RobotAI::Scared_Fire(RobotAI_Order & order)
{
	//TODO
}







//绞肉机的状态execute方法：
void RobotAI::HideAndApproach(RobotAI_Order & order)
{
	//躲在障碍物后面并伺机逼近
	
	int i;


	

	if((ENEMY.weaponTypeName==WT_MissileLauncher || ENEMY.weaponTypeName==WT_PlasmaTorch) && battle_info.num_bullet>0)
	{
		//对手用跟踪弹或等离子得用这种躲避方法
		double nearest_dist_sq=1e20;
		Vector2D vec_bullet,vec_nearest;
		double current_dist_sq;
		int nearest_bullet_idx=0;

		for(i=0;i<battle_info.num_bullet;i++)
		{
			//TODO:子弹的发射者？？这个怎么会没有啊。。。
			if(IsEnemyBullet(i))
			{
				vec_bullet.x=battle_info.bulletInformation[i].circle.x;
				vec_bullet.y=battle_info.bulletInformation[i].circle.y;

				current_dist_sq=Vec2DDistanceSq(vec_me,vec_bullet);
				if(current_dist_sq<nearest_dist_sq)
				{
					nearest_dist_sq=current_dist_sq;
					vec_nearest=vec_bullet;
					nearest_bullet_idx=i;
				}
			}
		}

		Hide(order,battle_info.bulletInformation[nearest_bullet_idx].circle);

	}
	else
	{
		Hide(order,ENEMY.circle);
	}

	
	

	//TODO:伺机逼近	
}
















void RobotAI::Crusher_Attack(RobotAI_Order & order)
{
	//冲啊！！！

	//TODO:导航图引导路径

	
	DirectMoveToPoint(order,ENEMY.circle.x,ENEMY.circle.y);
	//PursuitEnemy(order);
	

	cout<<"冲锋中...\n";
	cout<<"enmey ammo:"<<ENEMY.remainingAmmo<<'\n';

	//switch state
	//tmp


	/*
	if(MY.remainingAmmo<1)
	{
		tmp_state=2;	//补充弹药去
	}
	*/
	/*
	else if(ENEMY.remainingAmmo*(get_weapon_damage(ENEMY.weaponTypeName))>=MY.hp)
	{
		tmp_state=0;
	}
	*/

	/*
	else if(ENEMY.remainingAmmo>=5 && Vec2DDistanceSq(vec_me,vec_enemy)>250000)
	{
		tmp_state=0;
	}
	*/
}





void RobotAI::Reload(RobotAI_Order & order)
{
	
	
	/*
	AimAtPoint(order,ENEMY.circle.x,ENEMY.circle.y);
	//...居然没提供获取weapon半径的接口
	if(Vec2DDistanceSq(vec_me,vec_enemy)<=(95+MY.circle.r)*(95+MY.circle.r))	//电锯半径
	{
		Fire(order);
	}
	*/
	int select_arsenal_idx=0;
	if(battle_info.arsenal[0].respawning_time==0 && battle_info.arsenal[1].respawning_time==0)
	{
		//两个军火库都能补充弹药

		//补个弹药
		double d0,d1;//距离两个军火库的距离
		//Sq是平方的距离
		d0=Vec2DDistanceSq(vec_me,vec_arsenal0);
		d1=Vec2DDistanceSq(vec_me,vec_arsenal1);



		
		if(d0<d1)
		{
			//简单评估军火库风险
			if(Vec2DDistanceSq(vec_enemy,vec_arsenal0)>danger_distance*danger_distance)
			{
				select_arsenal_idx=0;
			}
			else
			{
				select_arsenal_idx=1;
			}
		}
		else
		{
			if(Vec2DDistanceSq(vec_enemy,vec_arsenal1)>danger_distance*danger_distance)
			{
				select_arsenal_idx=1;
			}
			else
			{
				select_arsenal_idx=0;
			}
		}

	}
	else
	{
		//不是每个都能补
		if(battle_info.arsenal[0].respawning_time==0)
		{
			select_arsenal_idx=0;
		}
		else if(battle_info.arsenal[1].respawning_time==0)
		{
			select_arsenal_idx=1;
		}
		else
		{
			//没有当前可以补的
			//先躲着
			Hide(order,ENEMY.circle);
			return;
		}
	}


	//TODO:路径规划
	if(!IsArriveAtPoint(battle_info.arsenal[select_arsenal_idx].circle.x,battle_info.arsenal[select_arsenal_idx].circle.y))
	{
		DirectMoveToPoint(order,battle_info.arsenal[select_arsenal_idx].circle.x,battle_info.arsenal[select_arsenal_idx].circle.y);
	}
	
}




//Crusher的状态转换函数
void RobotAI::Crusher_StateTransform()
{
	switch(tmp_state)
	{
	case 0:
		//躲藏状态
		if(IsSafeToRush())
		{
			//哈哈，干不掉老子了，冲啊！！！！！
			tmp_state=1;

		}
		else if(Vec2DDistanceSq(vec_me,vec_enemy)<crusher_death_rush*crusher_death_rush)
		{
			//兄弟你靠的太近了,直接冲上去操翻它
			tmp_state=1;
		}
		else
		{
			//接着怂
			tmp_state=0;
		}

		break;

	case 1:
		if(MY.remainingAmmo<1)
		{
			tmp_state=2;	//补充弹药去
		}
		else if(ENEMY.remainingAmmo==get_weapon_ammo(ENEMY.weaponTypeName))
		{
			//对方可能重新补充了弹药
			if(!IsSafeToRush())
			{
				tmp_state=0;
			}
		}
		else if(get_engine_maxSpeed(ENEMY.engineTypeName)>get_engine_maxSpeed(MY.engineTypeName))
		{
			//可能追不上被戏耍
			if(!IsSafeToRush())
			{
				tmp_state=0;
			}
		}
		break;

	case 2:
		//状态转换
		if(MY.remainingAmmo>=5)
		{
			tmp_state=0;
		}
	}
}