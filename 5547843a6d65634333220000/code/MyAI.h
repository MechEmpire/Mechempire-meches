#pragma once
#include "RobotAI_Interface.h"
#include <deque>
class CMyAI
{
private:
	RobotAI_Order m_order;
	RobotAI_BattlefieldInformation m_info;
	int m_myID;

	RobotAI_RobotInformation m_myinfo;
	RobotAI_RobotInformation m_myinfo_last;
	RobotAI_RobotInformation m_enemy_info;
	RobotAI_RobotInformation m_enemy_last;

	bool bBegin = true;
	int m_state = 0;
	int m_run=1;
	int m_trun = 0;
	bool m_bFireLocate = 0;
	bool m_bLocate = 0 ;
	const int m_time_rotate = 45;
	int m_counter_rotate = m_time_rotate;
	bool m_bRotateMove=0;
	int m_bullet_speed;
	int m_ammo_state=0;
private:
	Point m_predict_enemy ;
	Point m_predict_enemy_last;
	std::deque<Point> m_EnemyPaths;
public:
	void SetBulletSpeed(int s)
	{
		m_bullet_speed = s;
		int l = (sqrt(m_info.boundary.height*m_info.boundary.height + m_info.boundary.width*m_info.boundary.width)) / s;
		//m_EnemyPaths.resize(l, { 0, 0 });
	}
	void GetInput(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
	{
		m_order = order;
		m_info = info;
		m_myID = myID;

		m_myinfo = m_info.robotInformation[myID];
		m_enemy_info = m_info.robotInformation[1-myID];

		if (m_EnemyPaths.size() < 400)
		{
			m_EnemyPaths.push_back({ m_enemy_info.circle.x, m_enemy_info.circle.y });
		}
		else
		{
			m_EnemyPaths.pop_front();
			m_EnemyPaths.push_back({ m_enemy_info.circle.x, m_enemy_info.circle.y });
		}
		if (bBegin)
		{
			bBegin = false;
			m_enemy_last = m_enemy_info;
			m_myinfo_last = m_myinfo;
		}
	}
	void PostOutput(RobotAI_Order& order)
	{
		order = m_order;
		m_myinfo_last = m_myinfo;
		m_enemy_last = m_enemy_info;
		m_predict_enemy_last = m_predict_enemy;
	}
	void Update();
private:
	void moveforMine();
	void moveforMySawByWait();
	void moveforCannon();
	void moveforAmmo();
	void moveforMyTesla();
	void moveAroundAll();
	double getDistanceToSelf(Point c);
	double getDistanceFromEnemy();
	void fire();
	void movetoPoint(Point p);
	void moveAroundPoint(Circle p);
	void moveforBall();
	void moveforMySaw();
	void moveforMissle();
	void moveforSaw();
	void moveforDefault();
	void rotateSelf(int direction, int time);
	void move();
	void detectObstacle(Circle& c);
	void detectObstacleToPredict(Circle& c);
	void locateWeapon();
	void locateWeaponNoInterval();
	void locateWeaponWithInterval();
	Beam getMyWeaponBeam();
public:
	CMyAI();
	~CMyAI();
};

