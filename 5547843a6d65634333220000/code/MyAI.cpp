#include "MyAI.h"
using namespace std;

CMyAI::CMyAI()
{
}


CMyAI::~CMyAI()
{
}
void CMyAI::fire()
{
	switch (m_myinfo.weaponTypeName)
	{
	case WT_ElectricSaw:
		if (m_bFireLocate)
		{
			if (getDistanceFromEnemy() - 95 >70)
			{
				m_bFireLocate = 0;
			}
		}
		break;
	case WT_Prism:
	case WT_Tesla:
		if (m_bFireLocate)
		{
			detectObstacle(m_info.obstacle[0]);
			detectObstacle(m_info.obstacle[1]);
		}
		break;
	default:
		if (m_bFireLocate)
		{
			detectObstacleToPredict(m_info.obstacle[0]);
			detectObstacleToPredict(m_info.obstacle[1]);
			//detectObstacle(m_info.obstacle[0]);
			//detectObstacle(m_info.obstacle[1]);
		}
		break;
	}
}
void CMyAI::Update()
{
	locateWeapon();
	fire();
	move();
	m_order.fire = m_bFireLocate;
	m_order.run = m_run;
	m_order.eturn = m_trun;
}
Beam CMyAI::getMyWeaponBeam()
{
	Beam b;
	b.x = m_myinfo.circle.x;
	b.y = m_myinfo.circle.y;
	b.rotation = m_myinfo.weaponRotation;
	return b;
}

double CMyAI::getDistanceFromEnemy()
{
	return sqrt((m_enemy_info.circle.x - m_myinfo.circle.x)*(m_enemy_info.circle.x - m_myinfo.circle.x) + (m_enemy_info.circle.y - m_myinfo.circle.y)*(m_enemy_info.circle.y - m_myinfo.circle.y));
}

double CMyAI::getDistanceToSelf(Point c)
{
	return sqrt((c.x - m_myinfo.circle.x)*(c.x - m_myinfo.circle.x) + (c.y - m_myinfo.circle.y)*(c.y - m_myinfo.circle.y));
}
void CMyAI::moveforMySaw()
{
	{

		if (m_myinfo.remainingAmmo == 0 && (m_info.arsenal[0].respawning_time == 0 || m_info.arsenal[1].respawning_time == 0))
		{
			moveforAmmo();
		}
		else
		{
			movetoPoint(Point{ m_enemy_info.circle.x, m_enemy_info.circle.y });
		}

	}
}

void CMyAI::moveforMySawByWait()
{
	{

		if (m_myinfo.remainingAmmo == 0 && (m_info.arsenal[0].respawning_time == 0 || m_info.arsenal[1].respawning_time == 0))
		{
			moveforAmmo();
		}
		else
		{
			if (m_state == 0)
			{
				Point p = { m_info.obstacle[m_myID].x, m_info.obstacle[m_myID].y };
				movetoPoint(p);
				if (getDistanceFromEnemy() < 500)
				{
					m_state = 1;
				}
			}
			if (m_state == 1)
			{
				movetoPoint(Point{ m_enemy_info.circle.x, m_enemy_info.circle.y });
			}
		}

	}
}
void CMyAI::movetoPoint(Point p)
{
	double angle = RadianToAngle(atan2(-(m_myinfo.circle.y - p.y), -(m_myinfo.circle.x - p.x)));
	angle = AngleAdjust(angle);
	double dangle = angle-(m_myinfo.engineRotation)  ;

	dangle = AngleAdjust(dangle);

	
	switch (m_myinfo.engineTypeName)
	{
	case ET_Robotman:
	case ET_Shuttle:
	case ET_GhostTank:

		if (dangle>2)
		{
			m_trun = 1;
			m_run = 1;
		}
		else if (dangle<-2)
		{
			m_trun = -1;
			m_run = 1;
		}
		else
		{
			m_trun = 0;
			m_run = 1;
		}
		break;
	default:
		break;
	}
}

void CMyAI::moveforMissle()
{
	if (m_myinfo.remainingAmmo == 0 && (m_info.arsenal[0].respawning_time == 0 || m_info.arsenal[1].respawning_time == 0))
	{
		moveforAmmo();
	}
	else
	{
		if (m_state == 0)
		{
			movetoPoint(Point{ m_info.arsenal[1 - m_myID].circle.x, m_info.arsenal[1 - m_myID].circle.y });

			if (getDistanceToSelf(Point{ m_info.arsenal[1 - m_myID].circle.x, m_info.arsenal[1 - m_myID].circle.y })<300)
			{
				m_state = 1;
			}
		}
		else if (m_state == 1)
		{
			Circle p = m_info.obstacle[1 - m_myID];
			p.r += 60;
			moveAroundPoint(p);
		}
	}
}
void CMyAI::moveAroundAll()
{
	if (m_state == 0)
	{
		movetoPoint(Point{ m_info.arsenal[1 - m_myID].circle.x, m_info.arsenal[1 - m_myID].circle.y });
		if (getDistanceToSelf(Point{ m_info.arsenal[1 - m_myID].circle.x, m_info.arsenal[1 - m_myID].circle.y })<100)
		{
			m_state = 1;
		}
	}

	else if (m_state == 1)
	{
		movetoPoint(Point{ m_info.arsenal[1 - m_myID].circle.x, m_info.arsenal[m_myID].circle.y });

		if (getDistanceToSelf(Point{ m_info.arsenal[1 - m_myID].circle.x, m_info.arsenal[m_myID].circle.y })<200)
		{
			m_state = 2;
		}
	}
	else if (m_state == 2)
	{
		movetoPoint(Point{ m_info.arsenal[m_myID].circle.x, m_info.arsenal[m_myID].circle.y });

		if (getDistanceToSelf(Point{ m_info.arsenal[m_myID].circle.x, m_info.arsenal[m_myID].circle.y })<100)
		{
			m_state = 3;
		}
	}
	else if (m_state == 3)
	{
		movetoPoint(Point{ m_info.arsenal[m_myID].circle.x, m_info.arsenal[1 - m_myID].circle.y });

		if (getDistanceToSelf(Point{ m_info.arsenal[m_myID].circle.x, m_info.arsenal[1 - m_myID].circle.y })<200)
		{
			m_state = 0;
		}
	}
}
void CMyAI::moveforSaw()
{
	
		movetoPoint(Point{ m_info.arsenal[m_myID].circle.x, m_info.arsenal[m_myID].circle.y });
	
	/*
	if (m_state == 0)
	{
		movetoPoint(Point{ m_info.arsenal[1 - m_myID].circle.x,m_info.arsenal[1 - m_myID].circle.y });
		if (getDistanceToSelf(Point{ m_info.arsenal[1 - m_myID].circle.x, m_info.arsenal[1 - m_myID].circle.y })<100)
		{
			m_state =1;
		}
	}
	
	else if (m_state == 1)
	{
		movetoPoint(Point{ m_info.arsenal[1 - m_myID].circle.x, m_info.arsenal[m_myID].circle.y });
		
		if (getDistanceToSelf(Point{ m_info.arsenal[1 - m_myID].circle.x, m_info.arsenal[m_myID].circle.y })<200)
		{
			m_state = 2;
		}
	}
	else if (m_state == 2)
	{
		movetoPoint(Point{ m_info.arsenal[m_myID].circle.x, m_info.arsenal[m_myID].circle.y });

		if (getDistanceToSelf(Point{ m_info.arsenal[m_myID].circle.x, m_info.arsenal[m_myID].circle.y })<100)
		{
			m_state = 3;
		}
	}
	else if (m_state == 3)
	{
		movetoPoint(Point{ m_info.arsenal[ m_myID].circle.x, m_info.arsenal[1-m_myID].circle.y });

		if (getDistanceToSelf(Point{ m_info.arsenal[m_myID].circle.x, m_info.arsenal[1 - m_myID].circle.y })<200)
		{
			m_state = 0;
		}
	}
	*/
}
void CMyAI::moveAroundPoint(Circle p)
{
	if (getDistanceToSelf(Point{ p.x, p.y }) > p.r)
	{
		movetoPoint(Point{ p.x, p.y });
	}
	else
	{
		m_trun = 0;
		m_run = 1;
	}
}

void CMyAI::moveforMyTesla()
{
	if ((m_myinfo.remainingAmmo == 0 || m_enemy_info.remainingAmmo == 0) && (m_info.arsenal[0].respawning_time == 0 || m_info.arsenal[1].respawning_time == 0))
	{
		moveforAmmo();
	}
	else
	{
		{
			Circle p = Circle{ m_info.boundary.x, m_info.boundary.y, 300 };
			moveAroundPoint(p);
		}
	}

}

void CMyAI::moveforAmmo()
{
	
	if (m_ammo_state == 0)
	{
		if (getDistanceToSelf(Point{ m_info.arsenal[0].circle.x, m_info.arsenal[0].circle.y }) >
			getDistanceToSelf(Point{ m_info.arsenal[1].circle.x, m_info.arsenal[1].circle.y }))
		{
			if (m_info.arsenal[0].respawning_time == 0)
			{
				m_ammo_state = 1;
			}
			else
			{
				m_ammo_state = 2;
			}
		}
		else
		{
			m_ammo_state = 2;
		}
	}
	else if (m_ammo_state==1)
	{
		moveAroundPoint(Circle{ m_info.arsenal[0].circle.x, m_info.arsenal[0].circle.y,100});
		if (m_info.arsenal[0].respawning_time > 0)
		{
			m_ammo_state = 0;
		}
	}
	else if (m_ammo_state ==2)
	{
		moveAroundPoint(Circle{ m_info.arsenal[1].circle.x, m_info.arsenal[1].circle.y, 100 });
		if (m_info.arsenal[1].respawning_time > 0)
		{
			m_ammo_state = 0;
		}
	}
	
}
void CMyAI::moveforBall()
{
	if ((m_myinfo.remainingAmmo == 0 ) && (m_info.arsenal[0].respawning_time == 0 || m_info.arsenal[1].respawning_time == 0))
	{
		moveforAmmo();
	}
	else
	{
		Circle p =  m_info.obstacle[1-m_myID];
		p.r += 60;
		moveAroundPoint(p);
	}
}
void CMyAI::moveforDefault()
{
	if ((m_myinfo.remainingAmmo == 0 || m_enemy_info.remainingAmmo == 0) && (m_info.arsenal[0].respawning_time == 0 || m_info.arsenal[1].respawning_time == 0))
	{
		moveforAmmo();
	}
	else
	{
		{
			Circle p = getDistanceToSelf(Point{ m_info.obstacle[0].x, m_info.obstacle[0].y }) < getDistanceToSelf(Point{ m_info.obstacle[1].x, m_info.obstacle[1].y }) ? m_info.obstacle[0] : m_info.obstacle[1];
			p.r += 60;
			moveAroundPoint(p);
		}
	}
	
}

void CMyAI::moveforCannon()
{
	if ((m_myinfo.remainingAmmo == 0 || m_enemy_info.remainingAmmo == 0) && (m_info.arsenal[0].respawning_time == 0 || m_info.arsenal[1].respawning_time == 0))
	{
		moveforAmmo();
	}
	else
	{
		if (m_state==0)
		{
			Point p = { m_info.obstacle[m_myID].x, m_info.obstacle[m_myID].y };
			movetoPoint(p);
			if (getDistanceFromEnemy() < 600)
			{
				m_state = 1;
			}
		}
		if (m_state == 1)
		{
			moveforMySaw();
		}
		if (m_state == 2)
		{
			Point p = { m_info.obstacle[1-m_myID].x, m_info.obstacle[1-m_myID].y };
			moveAroundPoint({ p.x,p.y,210 });
			if (getDistanceToSelf({ m_info.arsenal[1 - m_myID].circle.x, m_info.arsenal[1 - m_myID].circle.y }) < 150)
			{
				m_state = 3;
			}
		}
		if (m_state == 3)
		{
			Point p = { m_info.arsenal[m_myID].circle.x, m_info.arsenal[m_myID].circle.y };
			moveAroundPoint({ p.x, p.y, 180 });
			
		}
		
	}
}
void CMyAI::moveforMine()
{
	{

		if (m_myinfo.remainingAmmo == 0 && (m_info.arsenal[0].respawning_time == 0 || m_info.arsenal[1].respawning_time == 0))
		{
			moveforAmmo();
		}
		else
		{
			movetoPoint(Point{ m_enemy_info.circle.x, m_enemy_info.circle.y });
		}

	}
}

void CMyAI::move()
{
	if (m_myinfo.weaponTypeName == WT_ElectricSaw)
	{
		switch (m_enemy_info.weaponTypeName)
		{
		case WT_MissileLauncher:
			moveforMissle();
			break;
		case WT_ElectricSaw:
			moveforMySaw();
			break;
		case WT_Apollo:
			moveforMySawByWait();
			break;
		default:
			moveforMySaw();
			break;
		}
	}
	else if (m_myinfo.weaponTypeName == WT_Tesla)
	{
		switch (m_enemy_info.weaponTypeName)
		{
		case WT_MissileLauncher:
			moveforMissle();
			break;
		case WT_ElectricSaw:
			moveforSaw();
			break;
		case WT_PlasmaTorch:
			moveforBall();
			break;
		case WT_Prism:
			moveforMySaw();
			break;
		case WT_Tesla:
			moveforMySaw();
			break;
		case WT_Machinegun:
			if ((m_myinfo.remainingAmmo == 0 || m_enemy_info.remainingAmmo <= 6) && (m_info.arsenal[0].respawning_time == 0 || m_info.arsenal[1].respawning_time == 0))
			{
				moveforAmmo();
			}
			else
			{
				{
					Circle p = getDistanceToSelf(Point{ m_info.obstacle[0].x, m_info.obstacle[0].y }) < getDistanceToSelf(Point{ m_info.obstacle[1].x, m_info.obstacle[1].y }) ? m_info.obstacle[0] : m_info.obstacle[1];
					p.r += 60;
					moveAroundPoint(p);
				}
			}
			break;
		case WT_MineLayer:
			moveforMine();
			break;
		case WT_Apollo:
			moveforCannon();
			break;
		default:
			moveforDefault();
			break;
		}
	}
	else if (m_myinfo.weaponTypeName == WT_Cannon)
	{
		switch (m_enemy_info.weaponTypeName)
		{
		case WT_MissileLauncher:
			moveforMissle();
			break;
		case WT_ElectricSaw:
			moveforSaw();
			break;
		case WT_PlasmaTorch:
			moveforBall();
			break;
		case WT_Prism:
			moveforMySaw();
			break;
		case WT_Tesla:
			moveforMySaw();
			break;
		case WT_Machinegun:
			if ((m_myinfo.remainingAmmo == 0 || m_enemy_info.remainingAmmo <= 2) && (m_info.arsenal[m_myID].respawning_time == 0))
			{
				moveAroundPoint(Circle{ m_info.arsenal[m_myID].circle.x, m_info.arsenal[m_myID].circle.y, 100 });
			}
			else
			{
				{
					Circle p = getDistanceToSelf(Point{ m_info.obstacle[0].x, m_info.obstacle[0].y }) < getDistanceToSelf(Point{ m_info.obstacle[1].x, m_info.obstacle[1].y }) ? m_info.obstacle[0] : m_info.obstacle[1];
					p.r += 60;
					moveAroundPoint(p);
				}
			}
			break;
		case WT_MineLayer:
			moveforMine();
			break;
		case WT_Apollo:
			if ((m_myinfo.remainingAmmo == 0 || m_enemy_info.remainingAmmo <= 2) && (m_info.arsenal[0].respawning_time == 0))
			{
				moveAroundPoint(Circle{ m_info.arsenal[0].circle.x, m_info.arsenal[0].circle.y, 100 });
			}
			else
			{
				{
					Circle p = getDistanceToSelf(Point{ m_info.obstacle[0].x, m_info.obstacle[0].y }) < getDistanceToSelf(Point{ m_info.obstacle[1].x, m_info.obstacle[1].y }) ? m_info.obstacle[0] : m_info.obstacle[1];
					p.r += 60;
					moveAroundPoint(p);
				}
			}
			break;
		default:
			moveforMySaw();
			break;
		}
	}
	else
	{
		switch (m_enemy_info.weaponTypeName)
		{
		case WT_MissileLauncher:
			moveforMissle();
			break;
		case WT_ElectricSaw:
			moveforSaw();
			break;
		case WT_PlasmaTorch:
			moveforBall();
			break;
		default:
			moveforDefault();
			break;
		}
	}
	
}
void CMyAI::rotateSelf(int direction,int time)
{
	if (!m_bRotateMove)
	{
		m_bRotateMove = true;
		m_counter_rotate = time;
	}
	if (m_counter_rotate > 0)
	{
		--m_counter_rotate;
		m_trun = direction;
		m_run = 0;
	}
	else
	{
		m_bRotateMove = false;
		m_trun = 0;
		m_run = 1;
	}
}
void CMyAI::detectObstacleToPredict(Circle& c)
{
	Beam b = getMyWeaponBeam();
	bool r1 = HitTestBeamCircle(b, c);

	if (r1)
	{
		double l1 = getDistanceToSelf(m_predict_enemy);
		double l2 = sqrt((c.x - m_myinfo.circle.x)*(c.x - m_myinfo.circle.x) + (c.y - m_myinfo.circle.y)*(c.y - m_myinfo.circle.y));
		if ((l1 - 5) > l2)
		{
			m_bFireLocate = 0;
		}
	}
}
void CMyAI::detectObstacle(Circle& c)
{
	Beam b = getMyWeaponBeam();
	bool r1=HitTestBeamCircle(b, c);

	if (r1)
	{
		double l1 = getDistanceFromEnemy();
		double l2 = sqrt((c.x - m_myinfo.circle.x)*(c.x - m_myinfo.circle.x) + (c.y - m_myinfo.circle.y)*(c.y - m_myinfo.circle.y));
		if ((l1-5) > l2)
		{
			m_bFireLocate = 0;
		}
	}
}

void CMyAI::locateWeapon()
{
	if (m_myinfo.weaponTypeName == WT_Prism || m_myinfo.weaponTypeName == WT_Tesla || m_myinfo.weaponTypeName == WT_ElectricSaw)
	{
		locateWeaponNoInterval();
	}
	else
	{

		if (getDistanceFromEnemy() > 230)
		{

			locateWeaponWithInterval();
		}
		else
		{
			locateWeaponNoInterval();
		}
	}
}

void CMyAI::locateWeaponWithInterval()
{
	Beam b = getMyWeaponBeam();

	/*

	double dx = (m_enemy_info.circle.x - m_enemy_last.circle.x);
	double dy = (m_enemy_info.circle.y - m_enemy_last.circle.y);
	double aa = (dx*dx + dy*dy - m_bullet_speed*m_bullet_speed);

	double xx = (m_enemy_info.circle.x - (m_myinfo.circle.x + cos(AngleToRadian(m_myinfo.weaponRotation))*95));
	double yy = (m_enemy_info.circle.y - (m_myinfo.circle.y + sin(AngleToRadian(m_myinfo.weaponRotation)) * 95));

	double bb = 2*(xx*dx+yy*dy);

	double cc = xx*xx+yy*yy;

	double t =0;
	if (abs(aa)> 0.0000000000001)
	{
	double dd = bb*bb - 4 * aa*cc;
	double t1 = (-bb + sqrt(dd)) / (2 * aa);
	double t2 = (-bb - sqrt(dd)) / (2 * aa);
	if (t1 < 0 && t2 >= 0)
	{
	t = t2;
	}
	else if (t1 >= 0 && t2 < 0 )
	{
	t = t1;
	}
	else if (t1>t2)
	{
	t = t2;
	}
	else
	{
	t = t1;
	}
	}
	else if ((abs(bb)> 0.0000000000001))
	{
	t = -cc / bb;
	}

	double time = t;

	double px = m_enemy_info.circle.x + (time)*(m_enemy_info.circle.x - m_enemy_last.circle.x);
	double py = m_enemy_info.circle.y + (time)*(m_enemy_info.circle.y - m_enemy_last.circle.y);

	if (m_predict_enemy.x == 0 && m_predict_enemy.y == 0)
	{
	m_predict_enemy.x = px;
	m_predict_enemy.y = py;
	}
	else
	{
	m_predict_enemy.x = (0  *m_predict_enemy.x + 5 * px) / 5;
	m_predict_enemy.y = (0 * m_predict_enemy.y + 5 * py) / 5;
	}
	double ll1 = sqrt((m_predict_enemy.x - m_predict_enemy_last.x)*(m_predict_enemy.x - m_predict_enemy_last.x)
	+ (m_predict_enemy.y - m_predict_enemy_last.y)*(m_predict_enemy.y - m_predict_enemy_last.y));
	if (ll1 > 30)
	{
	//	m_predict_enemy.x = (m_predict_enemy.x + m_predict_enemy_last.x) / 2;
	//	m_predict_enemy.y = (m_predict_enemy.y + m_predict_enemy_last.y) / 2;
	locateWeaponNoInterval();
	return;
	}
	*/
	{
		double sum_xy=0;
		double sum_x=0;
		double sum_y=0;
		double sum_x2=0;
		int N = 0;
		for (deque<Point>::iterator it = m_EnemyPaths.begin(); it != m_EnemyPaths.end(); ++it)
		{
			++N;
			sum_xy += N*(*it).y;
			sum_x += N;
			sum_y += (*it).y;
			sum_x2 += N*N;
		}
		double  a = (sum_xy - sum_x*sum_y / N) / (sum_x2 - sum_x*sum_x / N);
		double b = sum_y / N - a*sum_x / N;
		m_predict_enemy.y = a*(N + 1) + b;
	}
	{
		double sum_xy = 0;
		double sum_x = 0;
		double sum_y = 0;
		double sum_x2 = 0;
		int N = 0;
		for (deque<Point>::iterator it = m_EnemyPaths.begin(); it != m_EnemyPaths.end(); ++it)
		{
			++N;
			sum_xy += N*(*it).x;
			sum_x += N;
			sum_y += (*it).x;
			sum_x2 += N*N;
		}
		double  a = (sum_xy - sum_x*sum_y / N) / (sum_x2 - sum_x*sum_x / N);
		double b = sum_y / N - a*sum_x / N;
		m_predict_enemy.x = a*(N + 1) + b;
	}

	Circle c = { m_predict_enemy.x, m_predict_enemy.y, m_enemy_info.circle.r };
	Circle c2 = { m_enemy_info.circle.x, m_enemy_info.circle.y, m_enemy_info.circle.r };

	m_bFireLocate = HitTestBeamCircle(b, c);// || HitTestBeamCircle(b, c2);
	
	c.r = 10;
	m_bLocate = HitTestBeamCircle(b, c);
	if (m_bLocate)
	{
		m_order.wturn = 0;
	}
	else
	{

		double angle = RadianToAngle(atan2(-(m_myinfo.circle.y - m_enemy_info.circle.y), -(m_myinfo.circle.x - m_enemy_info.circle.x)));

		double dangle = angle - b.rotation;
		dangle = AngleAdjust(dangle);

		if (dangle>0)
		{
			m_order.wturn = 1;
		}
		else
		{
			m_order.wturn = -1;
		}
	}
}
void CMyAI::locateWeaponNoInterval()
{
	Beam b = getMyWeaponBeam();
	Circle c = m_enemy_info.circle;
	c.r -= 8;

	m_bFireLocate = HitTestBeamCircle(b, c);

	c.r = 10;
	m_bLocate = HitTestBeamCircle(b, c);
	if (m_bLocate)
	{
		m_order.wturn = 0;
	}
	else
	{
		
		double angle =  RadianToAngle(atan2(-(m_myinfo.circle.y - m_enemy_info.circle.y) , -(m_myinfo.circle.x - m_enemy_info.circle.x)));
		//angle=AngleAdjust(angle);
		
		double dangle = angle-b.rotation;
		dangle = AngleAdjust(dangle);		
	
		if (dangle>0)
		{
			m_order.wturn = 1;
		}
		else
		{
			m_order.wturn = -1;
		}
	}
}
