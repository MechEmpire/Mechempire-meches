#include "RobotAI.h"
double dis(double, double, double, double);
RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}

//工具函数

//算夹角
double Angle(double x1, double y1, double x2, double y2){
	double angle = acos((x1*x2 + y1*y2) / sqrt(x1*x1 + y1*y1) / sqrt(x2*x2 + y2*y2));
	return angle;
}

//i为外围倍增系数
bool Between_Circles(const Circle &c, double x, double y, double i)
{
	double dx = x - c.x, dy = y - c.y;
	double dis1 = sqrt(dx*dx + dy*dy);
	return (dis1 <= c.r*i&&dis1>c.r);
		
}

//p为c1倍增系数，q为c2倍增系数
bool HitTestCircles_fake(const Circle &c1, const Circle &c2, double p,double q)
{
	double xx = c1.x - c2.x, yy = c1.y - c2.y;
	double rr = c1.r *p+ q*c2.r;

	

	double dis2 = xx*xx + yy*yy;

	return (rr*rr >= dis2);
}

//r为倍增系数
bool HitTestBeamCircle_fake(const Beam &b, const Circle &c,double r)
{


	
    double br = AngleToRadian(b.rotation);
	double k = tan(br);
	double g = k*c.x - c.y - k*b.x + b.y;	//直线一般式的值
	double d2 = g*g / (k*k + 1);			//圆心到直线距离平方
	if ((c.r+r)*(c.r+r) >= d2)
	{
		//直线与圆已经相交，下面判断方向
		double dy = c.y - b.y;
		double ss = sin(br);

		if ((ss>0 && dy>0) || (ss<0 && dy<0))
		{
			return true;
		}
	}
	return false;
}

//算距离
double dis(double x1, double y1, double x2, double y2) {
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

//两射线相交否
bool HitTestBeam_Beam(const Beam &a, const Beam &b)
{
	double br = AngleToRadian(a.rotation);
	double k1 = tan(br);

	double br2 = AngleToRadian(b.rotation);
	double k2 = tan(br2);

	double x = (k1*a.x - k2*b.x + b.y - a.y) / (k1 - k2);
	double y = k1*(x - a.x) + a.y;

	double angle_bias1 = atan2(y - b.y, x - b.x) * 180 / PI;
	if (angle_bias1 < 0)
		angle_bias1 += 360;//转成0——360度

	double angle_bias2 = atan2(y - a.y, x - a.x) * 180 / PI;
	if (angle_bias2 < 0)
		angle_bias2 += 360;//转成0——360度
	
	if (angle_bias1 == b.rotation&&angle_bias2 == a.rotation)
		return true;
	else
		return false;

}




//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------


//瞄准有待提高，有时间大改   应根据对手速度和我的炮弹速度算角度，明年再写了
double weapon_rotation(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID) {
	
	auto& me = info.robotInformation[myID];
	auto& target = info.robotInformation[1 - myID];
	double dx = target.circle.x - me.circle.x;
	double dy = target.circle.y - me.circle.y;
	double dt = atan2(dy, dx)*180.0 / PI - me.weaponRotation;

	double dx2 = cos(AngleToRadian(me.weaponRotation));
	double dy2 = sin(AngleToRadian(me.weaponRotation));

	double dt2 = atan2(dy2, dx2)*180.0 / PI - target.engineRotation;//新的
	AngleAdjust(dt);
	AngleAdjust(dt2);
	
	

	double dx3 = target.circle.x - me.circle.x;
	double dy3 = target.circle.y - me.circle.y;
	double dt3 = atan2(dy3, dx3)*180.0 / PI;
	if (dt3 < 0)
		dt3 += 360;
	
	
	double speed_vertical = sqrt(target.vx*target.vx + target.vy*target.vy)*sin(AngleToRadian(abs(dt2)));
	
	double distance = cos(AngleToRadian(abs(me.weaponRotation - dt3)))*dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y);
	
	const double eps = speed_vertical*distance/200;
	//const double eps = 0;
	if (dt> eps){
		order.wturn = 1;//顺时针
	}
	else if (dt < -eps){
		order.wturn = -1;//逆时针
	}
	else 
	{
		if (target.circle.y >= 600)
		{
			if (target.engineRotation >= 0 && target.engineRotation <= 85)
				order.wturn = -1;//逆
			else if (target.engineRotation >= 95 && target.engineRotation <= 180)
				order.wturn = 1;//顺
			else
			{
				if (dt2 <= -2 && dt2 >= -178)
					order.wturn = 1;//有争议。。。
				if (dt2 >= 2 && dt2 <= 178)
					order.wturn = -1;
			}
		}
		else if (target.circle.y <= 80)//target.circle.x>=1216||target.circle.x<=50)
		{
			if (target.engineRotation <= 0 && target.engineRotation >= -85)
				order.wturn = 1;//顺
			else if (target.engineRotation <= -95 && target.engineRotation <= -180)
				order.wturn = -1;//逆
			else
			{
				if (dt2 <= -2 && dt2 >= -178)
					order.wturn = 1;//有争议。。。
				if (dt2 >= 2 && dt2 <= 178)
					order.wturn = -1;
			}
		}
		else if (target.circle.x >= 1176)//target.circle.x>=1216||target.circle.x<=50)
		{
			if (target.engineRotation >5 && target.engineRotation <= 90)
				order.wturn = 1;//顺
			else if (target.engineRotation <-5 && target.engineRotation >= -90)
				order.wturn = -1;//逆
			else
			{
				if (dt2 <= -2 && dt2 >= -178)
					order.wturn = 1;//有争议。。。
				if (dt2 >= 2 && dt2 <= 178)
					order.wturn = -1;
			}
		}
		else if (target.circle.x <=80)//target.circle.x>=1216||target.circle.x<=50)
		{
			if (target.engineRotation <=-90 && target.engineRotation > -175)
				order.wturn = 1;//顺
			else if (target.engineRotation >=90 && target.engineRotation <175)
				order.wturn = -1;//逆
			else
			{
				if (dt2 <= -2 && dt2 >= -178)
					order.wturn = 1;//有争议。。。
				if (dt2 >= 2 && dt2 <= 178)
					order.wturn = -1;
			}
		}
		else
		{
			if (dt2 <= -2 && dt2 >= -178)
				order.wturn = 1;//有争议。。。
			if (dt2 >= 2 && dt2 <= 178)
				order.wturn = -1;
		}
	}
	return Angle(dx, dy, dx2, dy2);
	
}

// x 为目的地横坐标
// y 为目的地竖坐标
void engine_drive(double x,double y,RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID) {
	auto& me = info.robotInformation[myID];
	
	double dx = x - me.circle.x;
	double dy = y - me.circle.y;
	double dt = atan2(dy, dx)*180.0 / PI - me.engineRotation;
	AngleAdjust(dt);
	const double eps = 1e-3;
	if (dt> eps){
		order.eturn = 1;//顺时针
	}
	else if (dt < -eps){
		order.eturn = -1;//逆时针
	}
	else {
		order.eturn = 0;
	}
	order.run = 1;
}

void obstacle_hit(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	auto& me = info.robotInformation[myID];
	for (int i = 0; i <= 1; i++)
	{

		if (bool obstacle_judge = HitTestCircles_fake(info.robotInformation[myID].circle, info.obstacle[i],1,1.15))
		{
			/*
			if (Angle(me.circle.x, me.circle.y, info.obstacle[i].x - me.circle.x, info.obstacle[i].y - me.circle.y) <= 90.0)
			{
				order.eturn = 1;
			
			}*/
			double dx = info.obstacle[i].x - me.circle.x;
			double dy = info.obstacle[i].y - me.circle.y;
			double dt = atan2(dy, dx)*180.0 / PI - me.engineRotation;
			AngleAdjust(dt);
			if (dt>=0&&dt<90){
				order.eturn = -1;//逆时针
			}
			else if (dt<0&&dt>-90){
				order.eturn = 1;//顺时针
			}
		}
	}
	
}

void escape_bullet(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	auto& me = info.robotInformation[myID];

	//找最近的子弹
	double min_dis = 1000;//初始化一个很大的值
	int num_min = -1;
	for (int i = 0; i <= info.num_bullet; i++)
	{
		auto& target = info.bulletInformation[i];
		if (target.launcherID == myID)
			continue;
		double temp = dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y);
		if (min_dis >= temp)
		{
			min_dis = temp;
			num_min = i;
		}
	}

	if (num_min != -1)
	{

		auto& target = info.bulletInformation[num_min];
		//躲避各种子弹

		double r_expansion = 0;//半径倍增系数

		switch (target.type)
		{
		case BT_Cannonball:
			r_expansion = 6;
			break;
		case BT_ShotgunBall:
			r_expansion = 6;
			break;

		case WT_PlasmaTorch:
			r_expansion = 4;
			break;
		case WT_Prism:
			r_expansion = 30;
			break;
		case WT_RPG:
			r_expansion = 30;
			break;
		}

		if (r_expansion)
		{


			//如果更大半径内碰得到，处于两圆中间地带
			if (Between_Circles(me.circle, target.circle.x, target.circle.y, r_expansion))
			{
				//order.fire = 1;
				//判断子弹是否能击中自己得重写
				Beam bullet{ target.circle.x, target.circle.y, target.rotation };
				//Beam me_engine{ me.circle.x, me.circle.y, me.engineRotation };
				if (r_expansion&&HitTestBeamCircle_fake(bullet, me.circle,2.5))
				{
					//order.fire = 1;

					double dx2 = target.circle.x - me.circle.x;
					double dy2 = target.circle.y - me.circle.y;
					double dt2 = atan2(dy2, dx2)*180.0 / PI - me.engineRotation;
					(dt2);
					
					double dx = cos(AngleToRadian(me.engineRotation));
					double dy = sin(AngleToRadian(me.engineRotation));
					double dt = atan2(dy, dx)*180.0 / PI - target.rotation;
					AngleAdjust(dt);

					Beam engine_beam{ me.circle.x, me.circle.y, me.engineRotation };
					Beam bullet_beam{ target.circle.x, target.circle.y, target.rotation };


					if (dt >= 1 && dt < 90)
					{
						if (HitTestBeam_Beam(engine_beam, bullet_beam))
							order.eturn = -1;
						else
							order.eturn = 1;
					}
					else if (dt <= -1 && dt >-90)
					{
						if (HitTestBeam_Beam(engine_beam, bullet_beam))
							order.eturn = 1;
						else
							order.eturn = -1;
					}
					else if (dt > 90.0&&dt <= 179)
					{
						if (HitTestBeam_Beam(engine_beam, bullet_beam))
							order.eturn = 1;
						else
							order.eturn = -1;

					}
					else if (dt < -90 && dt >= -179)
					{
						if (HitTestBeam_Beam(engine_beam, bullet_beam))
							order.eturn = -1;
						else
							order.eturn = 1;
					}
					
					else if ((dt <1 && dt >= 0) || (dt <0 && dt >= -1))
						order.eturn = -1;
					else if ((dt < -179 && dt >= -180) || (dt >179 && dt <= 180))
					{
						order.eturn = 1;
						
					}
					/*
					if (dt >=5&&dt <90)
						order.eturn = -1;
					else if (dt <=-5 && dt >-90)
						order.eturn = 1;
					else if ((dt <5 && dt >= 0) || (dt <0 && dt >=-5))
						order.eturn = 1;
					else if (dt > 90.0&&dt <= 175)
					{
						order.eturn = 1;

					}
					else if (dt < -90 && dt >= -175)
					{
						order.eturn = -1;
					}
					else if ((dt < -175 && dt >= -180) || (dt >175 && dt <= 180))
					{
						order.eturn = 1;
						//order.run = 0;
					}//逆时针
					*/
				}
			

			}
		}
	}

}

void escape_bullet_Beam(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	auto& me = info.robotInformation[myID];
	
	
	//找最近的子弹
	double min_dis = 1000;//初始化一个很大的值
	int num_min = -1;
	for (int i = 0; i <= info.num_bullet; i++)
	{
		auto& target = info.bulletInformation[i];
		if (target.launcherID == myID)
			continue;
		double temp = dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y);
		if (min_dis >= temp)
		{
			min_dis = temp;
			num_min = i;
		}
	}
	if (num_min != -1)
	{

		auto& target = info.bulletInformation[num_min];
		Beam bullet{ target.circle.x, target.circle.y, target.rotation };
		if (Between_Circles(me.circle, target.circle.x, target.circle.y, 35)&& HitTestBeamCircle(bullet, me.circle))
		{
			double dx = cos(AngleToRadian(me.engineRotation));
			double dy = sin(AngleToRadian(me.engineRotation));
			double dt = atan2(dy, dx)*180.0 / PI - target.rotation;
			//order.eturn = -1;//顺时针

			if (dt >= 90.0&&dt <= 175)
			{
				order.eturn = 1;

			}
			else if (dt <= -90 && dt >= -175)
			{
				order.eturn = -1;
			}
			else if ((dt < -175 && dt >= -180) || (dt >175 && dt <= 180))
			{
				order.eturn = -1;
				//order.run = 0;
			}//逆时针
		}
	}
}

void run_away(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID,int p, double run_away_distance)
{
	auto& arsenal = info.arsenal;
	auto& me = info.robotInformation[myID];
	auto& target = info.robotInformation[1 - myID];

	if (dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y) <= run_away_distance)
	{
		order.run = 1;

		double dx = cos(AngleToRadian(me.engineRotation));
		double dy = sin(AngleToRadian(me.engineRotation));
		double dt = atan2(dy, dx)*180.0 / PI - target.engineRotation;
		AngleAdjust(dt);
		if (dt <= 0 && dt > -180)
			order.eturn = 1;
		else if (dt > 0 && dt <180)
			order.eturn = -1;

	}
	else //if (me.remainingAmmo * 7*0.5 >= target.hp)//子弹够
		engine_drive(target.circle.x, target.circle.y, order, info, myID);
	//else
		//engine_drive(arsenal[p].circle.x, arsenal[p].circle.y, order, info, myID);


}

void boundry_hit(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID,double range)

{
	

	
	auto& me = info.robotInformation[myID];
	auto& target = info.robotInformation[1-myID];
	if (me.circle.y -range- me.circle.r == 0)// 上 墙
	{
		if (me.engineRotation >270)
			order.eturn = 1;
		else if (me.engineRotation >180 && me.engineRotation<270)
			order.eturn = -1;
		else if (me.engineRotation == 270)//等于 情况目前简化
		{
			if (me.circle.x<=target.circle.x)
				order.eturn = -1;
			else
				order.eturn = 1;
		}
			
		
	}
		
	if (me.circle.y + me.circle.r+range == info.boundary.height)//下
	{
		if (me.engineRotation >0 && me.engineRotation<90)
			order.eturn = -1;
		else if (me.engineRotation >90 && me.engineRotation <180)
			order.eturn = 1;
		else if (me.engineRotation == 90)//等于 情况目前简化
		{
			if (me.circle.x <= target.circle.x)
				order.eturn = 1;
			else
				order.eturn = -1;
		}

	}

	if (me.circle.x + me.circle.r+range == info.boundary.width) //右
	{
		if (me.engineRotation >0 && me.engineRotation < 90)
			order.eturn = 1;
		else if (me.engineRotation >270)
			order.eturn = -1;
		else if (me.engineRotation == 0)//等于 情况目前简化
		{
			if (me.circle.y <= target.circle.y)
				order.eturn = -1;
			else
				order.eturn = 1;
		}

	}

	if (me.circle.x - me.circle.r-range== 0)//左
	{
		if (me.engineRotation >90 && me.engineRotation < 180)
			order.eturn = -1;
		else if (me.engineRotation >180 && me.engineRotation < 270)
			order.eturn = 1;
		else if (me.engineRotation == 180)//等于 情况目前简化
		{
			if (me.circle.y <= target.circle.y)
				order.eturn = 1;
			else
				order.eturn = -1;
		}

	}

}

void fire_or_not(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, double firerange, int q, double fire_angle)
{
	auto& me = info.robotInformation[myID];
	auto& target = info.robotInformation[1 - myID];
	
	int fire_angle_flag = 1;

	for (int i = 0; i <= 1; i++)
	{
		auto& obstacle = info.obstacle[i];
		double dx = obstacle.x - me.circle.x;
		double dy = obstacle.y - me.circle.y;
		double dt = atan2(dy, dx)*180.0 / PI;
		if (dt < 0)
			dt += 360;

		double dx2 = target.circle.x - me.circle.x;
		double dy2 = target.circle.y - me.circle.y;
		double dt2 = atan2(dy2, dx2)*180.0 / PI;
		if (dt2 < 0)
			dt2 += 360;
		
		double angle_charge = abs(asin(obstacle.r / dis(me.circle.x, me.circle.y, obstacle.x, obstacle.y))*180.0 / PI);
		double dt_min = dt - angle_charge-5;
		double dt_max = dt + angle_charge+5;

		

		if (dt2 >= dt_min&&dt2 <= dt_max&&me.weaponRotation <= dt_max&&me.weaponRotation >= dt_min && (dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y)*dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y) > dis(me.circle.x, me.circle.y, obstacle.x, obstacle.y)*dis(me.circle.x, me.circle.y, obstacle.x, obstacle.y) - obstacle.r*obstacle.r))
		{
			fire_angle_flag = 0;
			
			break;
		}
	}



	Beam my_bullet{ me.circle.x, me.circle.y, me.weaponRotation };
	
//(!HitTestBeamCircle(my_bullet, info.obstacle[q]))
	
	if ((!HitTestBeamCircle(my_bullet, info.obstacle[q])) && fire_angle_flag != 0 && HitTestCircles_fake(info.robotInformation[myID].circle, info.robotInformation[1 - myID].circle, firerange, 1))
	{
		//if (abs(fire_angle) <= 14)//要写一个算角度的。。大招，墙角要优化
		 order.fire = 1;
	}
}

Point solve_equation(Circle obstacle,Circle target)
{
	Point destination;
	double k = (obstacle.y - target.y) / (obstacle.x - target.x);
	double x1 = 80 / sqrt(k*k + 1) + obstacle.x;
	double x2 = -80 / sqrt(k*k + 1) + obstacle.x;
	if (target.x > obstacle.x)
		destination.x = x2;
	else
		destination.x = x1;
	destination.y = k*(destination.x - obstacle.x)+obstacle.y;
	return destination;
}

void escape_machinegun(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	auto& me = info.robotInformation[myID];

	//找最近的子弹
	double min_dis = 1000;//初始化一个很大的值
	int num_min = -1;
	for (int i = 0; i <= info.num_bullet; i++)
	{
		auto& target = info.bulletInformation[i];
		if (target.launcherID == myID)
			continue;
		double temp = dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y);
		if (min_dis >=temp)
		{
			min_dis = temp;
			num_min = i;
		}
	}

	if (num_min != -1)
	{

		auto& target = info.bulletInformation[num_min];
		//如果更大半径内碰得到，处于两圆中间地带
		if (Between_Circles(me.circle, target.circle.x, target.circle.y, 20))
		{
			//order.fire = 1;
			//判断子弹是否能击中自己得重写
			//Beam bullet{ target.circle.x, target.circle.y, target.rotation };
			//Beam me_engine{ me.circle.x, me.circle.y, me.engineRotation };

			
			double dx = cos(AngleToRadian(me.engineRotation));
			double dy = sin(AngleToRadian(me.engineRotation));
			double dt = atan2(dy, dx)*180.0 / PI - target.rotation;
			order.eturn = 1;//顺时针
			/*
			AngleAdjust(dt);
			if (dt >= 90.0&&dt <= 175)
			{
			order.eturn = 1;

			}
			else if (dt <= -90 && dt >= -175)
			{
			order.eturn = -1;
			}
			else if ((dt < -175 && dt >= -180) || (dt >175 && dt <= 180))
			{
			order.eturn = -1;
			//order.run = 0;
			}//逆时针
			*/
		}
	}
}

void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{

	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	auto& me = info.robotInformation[myID];
	auto& target = info.robotInformation[1 - myID];
	
	//距离军火库距离
	auto& arsenal = info.arsenal;
	double dis0_to_arsenal= dis(me.circle.x, me.circle.y, arsenal[0].circle.x, arsenal[0].circle.y);
	double dis1_to_arsenal = dis(me.circle.x, me.circle.y, arsenal[1].circle.x, arsenal[1].circle.y);
	int p;//哪个军火库更近
	if (dis0_to_arsenal < dis1_to_arsenal)
		p = 0;
	else
		p = 1;
	
	//距离障碍物距离
	auto& obstacle = info.obstacle;
	double dis0_to_obstacle = dis(me.circle.x, me.circle.y, obstacle[0].x, obstacle[0].y);
	double dis1_to_obstacle = dis(me.circle.x, me.circle.y, obstacle[1].x, obstacle[1].y);
	int q;//哪个障碍物更近
	if (dis0_to_obstacle < dis1_to_obstacle)
		q = 0;
	else
		q = 1;




	double flag = 0;//默认不吃弹药状态
	double firerange = 12;//默认开火距离
	int warn = 0;//默认不waste状态
	int lack_bullet = 9;//默认弹药不足状态

	double fire_angle=weapon_rotation(order, info, myID);//任何情况下炮口都得对着敌人
    
	
	Point destination = solve_equation(obstacle[q], target.circle);
	engine_drive(destination.x, destination.y, order, info, myID);
	
	

	//目前写的是大炮坦克专杀 
	if (target.weaponTypeName == WT_Cannon )
	{
		firerange = 15;
		if (dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y) <= 600 && me.remainingAmmo * 4 >= target.hp&&target.remainingAmmo * 25<me.hp)
			engine_drive(target.circle.x, target.circle.y, order, info, myID);
		else
		{
			Point destination = solve_equation(obstacle[q], target.circle);
			engine_drive(destination.x, destination.y, order, info, myID);
			if (abs(me.circle.x -destination.x)<=1&&abs(me.circle.y == destination.y)<=1)
				order.run = 0;
		}

	}
	
	else if (target.weaponTypeName == WT_ElectricSaw)
	{
		lack_bullet = 10;//改变弹药不足状态阈值
		firerange = 12;
		
		run_away(order, info, myID, p, 550);
	}
	else if (target.weaponTypeName == WT_RPG)
	{
		firerange = 9;
		
		if (target.remainingAmmo==0&&me.remainingAmmo*5>target.hp)
			engine_drive(target.circle.x, target.circle.y, order, info, myID);
		
		else if (dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y) <= 315 && me.remainingAmmo * 5 > target.hp)
			order.run = 0;
		
		else if (dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y) <= 320 && me.remainingAmmo * 4 >= target.hp&&target.remainingAmmo * 35<me.hp)
			engine_drive(target.circle.x, target.circle.y, order, info, myID);
		else
			run_away(order, info, myID, p, 50);
	}
	else if (target.weaponTypeName == WT_Prism)
	{
		firerange = 10;
		run_away(order, info, myID, p, 50);
	}
	else if (target.weaponTypeName == WT_Machinegun)
	{
		firerange = 15;
		
		if (dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y) <= 500 && me.remainingAmmo * 4 >= target.hp&&target.remainingAmmo * 25<me.hp)
			engine_drive(target.circle.x, target.circle.y, order, info, myID);
		else
		{
			Point destination = solve_equation(obstacle[q], target.circle);
			engine_drive(destination.x, destination.y, order, info, myID);
			if (me.circle.x == destination.x&&me.circle.y == destination.y)
				order.run = 0;
		}
	}
	else if (target.weaponTypeName == WT_Shotgun)
	{
		firerange = 15;
		run_away(order, info, myID, p, 500);
	}
	else //目前是非大炮，电锯,火箭筒,光棱,  都往上冲
	{
		engine_drive(target.circle.x, target.circle.y, order, info, myID);
		if (dis(me.circle.x, me.circle.y, target.circle.x, target.circle.y) <= 100)
		{
			order.run = 1;


			double dx = cos(AngleToRadian(me.engineRotation));
			double dy = sin(AngleToRadian(me.engineRotation));
			double dt = atan2(dy, dx)*180.0 / PI - target.engineRotation;
			AngleAdjust(dt);
			if (dt <= 0 && dt > -180)
				order.eturn = 1;
			else if (dt > 0 && dt <180)
				order.eturn = -1;
		}
	}
    
	
	
	//没弹药
	if (me.remainingAmmo <= lack_bullet)
	{
		if (info.arsenal[p].respawning_time == 0)
		{
			double flag = 1;//去吃弹药状态
			engine_drive(arsenal[p].circle.x, arsenal[p].circle.y, order, info, myID);
		}
		else if (info.arsenal[1 - p].respawning_time == 0)
		{
			double flag = 1;//去吃弹药状态
			engine_drive(arsenal[1 - p].circle.x, arsenal[1 - p].circle.y, order, info, myID);
		}
		else if (target.remainingAmmo*25<me.hp)//要分析各种情况！！！！ 等明天补一下
		{
			if (info.arsenal[1 - p].respawning_time>info.arsenal[p].respawning_time)
				engine_drive(arsenal[ p].circle.x, arsenal[p].circle.y, order, info, myID);
			else
				engine_drive(arsenal[1-p].circle.x, arsenal[1-p].circle.y, order, info, myID);
		}
		else
		{
			Point destination = solve_equation(obstacle[q], target.circle);
			engine_drive(destination.x, destination.y, order, info, myID);
			if (me.circle.x == destination.x&&me.circle.y == destination.y)
				order.run = 0;
		}
	}
	
	
	if (target.remainingAmmo < 1 && (dis(me.circle.x, me.circle.y, arsenal[p].circle.x, arsenal[p].circle.y) < dis(target.circle.x, target.circle.y, arsenal[p].circle.x, arsenal[p].circle.y)))
	{
		if (info.arsenal[1 - p].respawning_time == 0 && info.arsenal[1 - p].respawning_time != 0)
			engine_drive(arsenal[p].circle.x, arsenal[p].circle.y, order, info, myID);
	}




	//躲避子弹模式切换
	//if (target.weaponTypeName != WT_RPG&&target.weaponTypeName != WT_Prism&&target.weaponTypeName != WT_Machinegun)
	if (target.weaponTypeName != WT_Machinegun)
		escape_bullet(order, info, myID);
	else 
	//if (target.weaponTypeName == WT_Machinegun)
		escape_machinegun(order, info, myID);
	//else	
		//escape_bullet_Beam(order, info, myID);
	
	
	
	if (me.remainingAmmo <= lack_bullet&&target.remainingAmmo<=2)
	{
		if (info.arsenal[p].respawning_time == 0)
		{
			double flag = 1;//去吃弹药状态
			engine_drive(arsenal[p].circle.x, arsenal[p].circle.y, order, info, myID);
		}
		else if (info.arsenal[1 - p].respawning_time == 0)
		{
			double flag = 1;//去吃弹药状态
			engine_drive(arsenal[1 - p].circle.x, arsenal[1 - p].circle.y, order, info, myID);
		}
	}

	obstacle_hit(order, info, myID);
	
	
	//吃弹药库模式切换
	if (flag == 1 && ((me.circle.x <= 60 && me.circle.y >= 610) || (me.circle.x >= 1290 && me.circle.y <= 60)))
	{
		boundry_hit(order, info, myID, 5);
		if (me.remainingAmmo>=15)
		firerange = 25;
	}
	else
	   boundry_hit(order, info, myID,70);

	if (me.remainingAmmo >= 27)
		firerange = 20;
	

	if (me.remainingAmmo <= 20)
		firerange = 12;
	if (me.remainingAmmo <=10)
		firerange = 10;
	if (me.remainingAmmo <= 3)
		firerange = 7;

	


	fire_or_not(order, info, myID, firerange, q, fire_angle);
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

	weapon = WT_Machinegun;	//啊，我爱加农炮
	engine = ET_AFV;	//四轴飞行器
	
}










//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "林";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "云";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return 0;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return -100;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return 0;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 255;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 0;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 0;
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