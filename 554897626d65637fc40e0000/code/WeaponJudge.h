#pragma once
#include "Global.h"
#include "RobotAI.h"
#include "Vector2D.h"
#include "utils.h"

class WeaponControl{
public:
	WeaponControl()
	{
		Excute = NULL;
	}
	void StateChange(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
	void (WeaponControl::*Excute)(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
	void Wait(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
	void WT_ElectricSaw_NAIVE(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
	void WT_Machinegun_NAIVE(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
	void WT_MissileLauncher_NAIVE(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);


};

//void WeaponJudge(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
