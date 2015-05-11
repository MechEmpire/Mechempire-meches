#pragma once

#include <cmath>
#include "struct.h"
#include "Vector2D.h"
#include "RobotAIstruct.h"

#define EPS 1e-4
#define eps 1e-10
using namespace std;

class Status{
public:
	Circle circle;		
	double engineRotation;
	double weaponRotation;	
	double vx, vy, vr;
	int hp;					
	int remainingAmmo;		
	int cooling;
	double __X__;
	Status(){
	}
	Status(const RobotAI_RobotInformation& s){
		circle = s.circle;
		engineRotation = s.engineRotation;
		weaponRotation = s.weaponRotation;
		vx = s.vx;
		vy = s.vy;
		vr = s.vr;
		__X__ = 1;
	}
	Status& operator=(const RobotAI_RobotInformation& s)
	{
		circle = s.circle;
		engineRotation = s.engineRotation;
		weaponRotation = s.weaponRotation;
		vx = s.vx;
		vy = s.vy;
		vr = s.vr;
		return *this;
	}
	bool DoubleEqual(double x, double y)const{
		return fabs(x - y) < EPS;
	}
	bool operator==(const Status& s) const
	{
		return (DoubleEqual(s.vx, vx) && DoubleEqual(s.vy, vy) && DoubleEqual(s.vr, vr) && DoubleEqual(s.engineRotation, engineRotation));
	}

};


int Solve1_2Function(double a, double b, double c, double& root1, double& root2);
double Det2x2(double a00, double a01, double a10, double a11);
int Solve2_1Function(double x1, double y1, double z1, double x2, double y2, double z2, Vector2D ans);
int GetTangentPoints(Circle c, Vector2D v, Vector2D& ans1, Vector2D& ans2);
double min(double a, double b);
double max(double a, double b);
bool SegmentCross(Vector2D a, Vector2D b, Vector2D c, Vector2D d);
