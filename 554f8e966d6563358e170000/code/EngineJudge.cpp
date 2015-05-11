#include "EngineJudge.h"
#include <iostream>
using namespace std;

void EngineControl::GenerateCondidateChoice(const RobotAI_BattlefieldInformation& info, int myID)
{
	
	for (int i = 0; i < NODESIZE; i++)
	{
		//candidateChoice[i].x = rand() % (1366 - (int)info.robotInformation[myID].circle.r * 3) + info.robotInformation[myID].circle.r*1.5;
		//candidateChoice[i].y = rand() % (680 - (int)info.robotInformation[myID].circle.r * 3) + info.robotInformation[myID].circle.r*1.5;
		candidateChoice[i].x = rand() % ((int)info.robotInformation[myID ^ 1].circle.r * 4) + info.robotInformation[myID ^ 1].circle.x - 2*info.robotInformation[myID ^ 1].circle.r;
		candidateChoice[i].y = rand() % ((int)info.robotInformation[myID ^ 1].circle.r * 4) + info.robotInformation[myID ^ 1].circle.y - 2*info.robotInformation[myID ^ 1].circle.r;
		/*Vector2D temp_v[2];
		Vector2D myXY = info.robotInformation[myID].circle;
		Beam b;
		b.x = myXY.x;
		b.y = myXY.y;
		b.ex = candidateChoice[i].x;
		b.ey = candidateChoice[i].y;
		b.rotation = RadianToAngle((candidateChoice[i] - myXY).ATAN2());
		Circle obextra[2];
		obextra[0] = info.obstacle[0];
		obextra[1] = info.obstacle[1];
		obextra[0].r += info.robotInformation[myID].circle.r;
		obextra[1].r += info.robotInformation[myID].circle.r;
		if (HitTestBeamCircle(b, obextra[0])){
			GetTangentPoints(obextra[0], myXY, temp_v[0], temp_v[1]);
			temp_v[0] = (temp_v[0] - info.obstacle[0])*1.2 + info.obstacle[0];
			temp_v[1] = (temp_v[1] - info.obstacle[1])*1.2 + info.obstacle[1];
		}
		if (HitTestBeamCircle(b, obextra[1])){
			GetTangentPoints(obextra[1], myXY, temp_v[0], temp_v[1]);
			temp_v[0] = (temp_v[0] - info.obstacle[0])*1.2 + info.obstacle[0];
			temp_v[1] = (temp_v[1] - info.obstacle[1])*1.2 + info.obstacle[1];
		}
		if (HitTestCirclePoint(obextra[0], candidateChoice[i].x, candidateChoice[i].y))
		{
			GetTangentPoints(obextra[0], myXY, temp_v[0], temp_v[1]);
			temp_v[0] = (temp_v[0] - info.obstacle[0])*1.2 + info.obstacle[0];
			temp_v[1] = (temp_v[1] - info.obstacle[1])*1.2 + info.obstacle[1];
		}			
		if (HitTestCirclePoint(obextra[1], candidateChoice[i].x, candidateChoice[i].y))
		{
			GetTangentPoints(obextra[1], myXY, temp_v[0], temp_v[1]);
			temp_v[0] = (temp_v[0] - info.obstacle[0])*1.2 + info.obstacle[0];
			temp_v[1] = (temp_v[1] - info.obstacle[1])*1.2 + info.obstacle[1];
		}
		candidateChoice[i] = temp_v[rand() % 2];*/
	}
}
void EngineControl::StateChange(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment){
	if (Excute == NULL)
		Excute = &EngineControl::Tank_Defence_Mode;
	if (Excute != NULL)
	{
		//cout << "excute!!\n";
		(this->*Excute)(order, info, myID, lastMoment);
	}
		
}

void EngineControl::Wait(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment){
	order.eturn = 0;
	order.run = 0;
}

void EngineControl::Tank_Defence_Mode(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID, const Status& lastMoment)
{
	GenerateCondidateChoice(info, myID);
	int maxn, choice=0, cnt = 0;
	Vector2D FinalChoice;
	Vector2D myXY = info.robotInformation[myID].circle;
	if (lastChoice.Distance(info.robotInformation[myID ^ 1].circle) > 2 * info.robotInformation[myID ^ 1].circle.r)
	{
		lastChoice.x = -1;
		lastChoice.y = -1;
	}
	if (lastChoice.x == -1)
	{
		maxn = INF;
	}
	else
	{

		double temp_dist = myXY.Distance(lastChoice);
		double temp_t = temp_dist / sqrt(info.robotInformation[myID].vx*info.robotInformation[myID].vx + info.robotInformation[myID].vy*info.robotInformation[myID].vy);
		Circle target = info.robotInformation[myID].circle;
		target.x = lastChoice.x;
		target.x = lastChoice.y;
		
		for (int j = 0; j < info.num_bullet; j++)
		{
			if (info.bulletInformation[j].launcherID != myID&&SegmentCross(myXY, lastChoice, info.bulletInformation[j].circle, (Vector2D)info.bulletInformation[j].circle + temp_t*(Vector2D)info.bulletInformation[j]))
			{
				cnt++;
			}
			Beam b;
			b.x = info.bulletInformation[j].circle.y;
			b.y = info.bulletInformation[j].circle.x;
			b.ex = info.bulletInformation[j].circle.x + info.bulletInformation[j].vx * temp_t;
			b.ey = info.bulletInformation[j].circle.y + info.bulletInformation[j].vy * temp_t;
			b.rotation = RadianToAngle(atan2(info.bulletInformation[j].vy, info.bulletInformation[j].vx));
			//cout << info.bulletInformation[j].vy << info.bulletInformation[j].vx;
			if (info.bulletInformation[j].launcherID != myID&&HitTestBeamCircle(b, target))
			{
				cnt++;
			}
			if (info.bulletInformation[j].launcherID != myID&&HitTestBeamCircle(b, info.robotInformation[myID].circle))
			{
				cnt++;
			}
		}
		maxn = cnt;
		choice = -1;
		//cout << cnt << endl;
	}
	for (int i = 0; i < NODESIZE; i++)
	{
		cnt = 0;
		//cout << candidateChoice[i] << endl;
		double temp_dist = myXY.Distance(candidateChoice[i]);
		double temp_t = temp_dist / sqrt(info.robotInformation[myID].vx*info.robotInformation[myID].vx + info.robotInformation[myID].vy*info.robotInformation[myID].vy);
		Circle target = info.robotInformation[myID].circle;
		target.x = candidateChoice[i].x;
		target.y = candidateChoice[i].y;
		for (int j = 0; j < info.num_bullet; j++)
		{
			if (info.bulletInformation[j].launcherID != myID&&SegmentCross(myXY, candidateChoice[i], info.bulletInformation[j].circle, (Vector2D)info.bulletInformation[j].circle + temp_t*(Vector2D)info.bulletInformation[j]))
			{
				cnt++;
			}
			Beam b;
			b.x = info.bulletInformation[j].circle.y;
			b.y = info.bulletInformation[j].circle.x;
			b.ex = info.bulletInformation[j].circle.x + info.bulletInformation[j].vx * temp_t;
			b.ey = info.bulletInformation[j].circle.y + info.bulletInformation[j].vy * temp_t;
			b.rotation = RadianToAngle(atan2(info.bulletInformation[j].vy, info.bulletInformation[j].vx));
			if (info.bulletInformation[j].launcherID != myID&&HitTestBeamCircle(b,target))
			{
				cnt++;
			}
			if (info.bulletInformation[j].launcherID != myID&&HitTestBeamCircle(b, info.robotInformation[myID].circle))
			{
				cnt++;
			}
			if (cnt < maxn)
			{
				maxn = cnt;
				choice = i;
			}
		}
	}
	if (choice == -1)
	{
		FinalChoice = lastChoice;
		//cout << "LC" << lastChoice<<endl;
	}
	else
	{
		lastChoice = candidateChoice[choice];
		//cout << "choice" << choice << endl;
		FinalChoice = lastChoice;
	}
	//cout << info.robotInformation[myID^1].circle.x << " " << info.robotInformation[myID^1].circle.y << endl;
	//cout << FinalChoice << endl;
	if (myXY.Distance(info.robotInformation[myID ^ 1].circle) <= 5 * info.robotInformation[myID ^ 1].circle.r)
	//cout << myXY.Distance(info.robotInformation[myID ^ 1].circle)<<endl;;
		FinalChoice = info.robotInformation[myID ^ 1].circle;
	//cout << FinalChoice << endl;
	Vector2D Vxy;
	Vxy.x = info.robotInformation[myID].vx;
	Vxy.y = info.robotInformation[myID].vy;
	double angleT = RadianToAngle(Vxy.ATAN2());
	double angleC = RadianToAngle((FinalChoice - myXY).ATAN2());
	if (angleT - angleC>180)
		order.eturn = 1;
	if (angleC - angleT>180)
		order.eturn = -1;
	if (angleC - angleT>0 && angleC - angleT<180)
		order.eturn = 1;
	if (angleT-angleC>0&&angleT-angleC<180)
		order.eturn = -1;
	order.run = 1;
}