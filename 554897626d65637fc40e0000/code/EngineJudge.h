#pragma once
#include "Global.h"
#include "RobotAI.h"
#include "Vector2D.h"
#include "utils.h"
#include <iostream>
using namespace std;
#define INF 1e30
#define NODESIZE 50
class EngineControl{
public:
	EngineControl()
	{
		//cout << "EngineFSM is OK\n";
		Excute = NULL;
		lastChoice.x = -1;
		lastChoice.y = -1;
	}
	Vector2D lastChoice;
	Vector2D candidateChoice[NODESIZE];
	void GenerateCondidateChoice(const RobotAI_BattlefieldInformation& info, int myID);
	void StateChange(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
	void (EngineControl::*Excute)(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
	void Wait(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
	//Tank
	void Tank_Defence_Mode(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
	//UFO 
	//Spider 
	//Robot
};

//void EngineJudge(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment);
