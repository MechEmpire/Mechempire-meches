#include "WeaponJudge.h"
#include "miaozhun.h"
#include "struct.h"
void WeaponControl::StateChange(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment){
	if (Excute == NULL)
		Excute = &WeaponControl::WT_Machinegun_NAIVE;
	if (Excute != NULL)
		(this->*Excute)(order, info, myID, lastMoment);
}

void WeaponControl::Wait(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment){
	order.eturn = 0;
	order.run = 0;
}

void WeaponControl::WT_ElectricSaw_NAIVE(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment)
{
	double angleT = info.robotInformation[myID].engineRotation;
	Vector2D vv = (Vector2D)info.robotInformation[myID ^ 1].circle - (Vector2D)info.robotInformation[myID].circle;
	vv.y = -vv.y;
	double angleC = RadianToAngle(vv.ATAN2());
	//cout << angleT << "," << angleC << endl;
	if (angleC > angleT){
		if (fabs(angleC - angleT) < 180){
			order.wturn = 1;
		}
		else{
			order.wturn = -1;
		}
	}
	else{
		if (fabs(angleC - angleT) < 180){
			order.wturn = -1;
		}
		else{
			order.wturn = 1;
		}
	}
	if (((Vector2D)info.robotInformation[myID ^ 1].circle).Distance((Vector2D)info.robotInformation[myID].circle) < 95 + info.robotInformation[myID^1].circle.r)
	{
		order.fire = 1;
	}
	else 
	{
		order.fire = 0;
	}
}
void WeaponControl::WT_Machinegun_NAIVE(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment)
{
	double temp;
	int tt = info.robotInformation[myID].remainingAmmo;
	VECTOR p;
	p = takeAim(order, info, myID);
	temp = p.y;
	Beam shootLine;
	shootLine.x = info.robotInformation[myID].circle.x;
	shootLine.y = info.robotInformation[myID].circle.y;
	shootLine.rotation = temp;
	//if (willShoot(info, myID, shootLine, p.x)) order.fire = 1;
	//else order.fire = 0;
	double xx = info.robotInformation[myID].circle.x + 95 * cos(AngleToRadian(info.robotInformation[myID].weaponRotation));
	double yy = info.robotInformation[myID].circle.y + 95 * sin(AngleToRadian(info.robotInformation[myID].weaponRotation));
	if (((info.robotInformation[myID ^ 1].circle.x - xx)*(info.robotInformation[myID ^ 1].circle.x - xx) + 
		 (info.robotInformation[myID ^ 1].circle.y - yy)*(info.robotInformation[myID ^ 1].circle.y - yy))<
		 (info.robotInformation[myID ^ 1].circle.r*info.robotInformation[myID ^ 1].circle.r))
		order.fire = 1;
	else
		order.fire = 0;
}