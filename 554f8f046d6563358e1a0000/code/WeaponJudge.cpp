#include "WeaponJudge.h"


void WeaponControl::StateChange(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment){
	if (Excute == NULL)
		Excute = &WeaponControl::WT_ElectricSaw_NAIVE;
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
