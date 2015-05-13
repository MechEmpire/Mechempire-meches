#include "RobotAI.h"
#include "Aiming.h"
#include <fstream>
#include "Battlefield.h"
#include "UFO.h"

RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}

bulletInfo *bullet;
int bulletNum = 0;

int frame = 0;

int myID = 0;
int enemyID = 1;

int myCurrentX = 0;
int myCurrentY = 0;

double myRotationalSpeed = 0;
double myWpRotSpeed = 0;
double myAcceleration = 0;
const double myRadius = 46;

const int DENSITY_CALCULATE_TIME = 40;
double* battlefieldDensity;


int transformOrder(int ord)
{
	if (ord % 3 == 2)
	{
		return -1;
	}
	else
	{
		return ord % 3;
	}
}

void diskBestOperation(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info)
{
	double *nextTenMove = new double[1296];

	double bestRes = 1000000;
	bool isBreak = false;
	for (int i = 0; i < 1296; i++)
	{
		double x = info.robotInformation[myID].circle.x;
		double y = info.robotInformation[myID].circle.y;
		double vx = info.robotInformation[myID].vx;
		double vy = info.robotInformation[myID].vy;
		double engineAngle = info.robotInformation[myID].engineRotation;

		for (int time = 0; time < 4; time++)
		{
			for (int n = 0; n < 10; n++)
			{
				int ord = int(i / int(pow(6, time))) % 6;
				diskMove(info, x, y, vx, vy, engineAngle, ord / 2, transformOrder(ord));

				*(nextTenMove + i) += *(battlefieldDensity + (time * 10 + n) * 620 * 620 + (int(floor(x)) - myCurrentX + 300) * 620 + (int(floor(y)) - myCurrentY + 300));
				if (*(nextTenMove + i) > bestRes)
				{
					break;
					isBreak = true;
				}
			}
			if (isBreak)
			{
				isBreak = false;
				break;
			}

		}
		if (*(nextTenMove + i) < bestRes)
		{
			bestRes = *(nextTenMove + i);
		}
	}

	double leastVal = *nextTenMove;
	int leastIndex = 0;
	for (int i = 1; i < 1296; i++)
	{
		if (leastVal > *(nextTenMove + i))
		{
			leastVal = *(nextTenMove + i);
			leastIndex = i;
			
		}
	}

	delete[] nextTenMove;
	if (abs(leastVal - 0) > 0.00001)
	{
		printf("Frame %i: Will be hit: %f\n", frame, leastVal);
	}
	order.run = (leastIndex % 6) / 2;
	order.eturn = transformOrder(leastIndex % 6);
}

void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	//printf("%i", frame);
	myCurrentX = int(floor(info.robotInformation[myID].circle.x));
	myCurrentY = int(floor(info.robotInformation[myID].circle.y));
	updatebulletInfo(info, bullet, bulletNum, enemyID);
	updateBattlefieldDensity(info, myCurrentX, myCurrentY, myID, myRadius, DENSITY_CALCULATE_TIME, bullet, bulletNum, battlefieldDensity);
	if (bulletNum > 0)
	{
		diskBestOperation(order, info);
	}
	else
	{
		order.run = 1;
		order.eturn = 0;
	}
	order.wturn = aiming(info, info.robotInformation[enemyID].circle.x, info.robotInformation[enemyID].circle.y, myID, myWpRotSpeed);
	int firePower = fractionHit(info, info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y, 95, info.robotInformation[myID].weaponRotation, 5, 46, info.robotInformation[enemyID].circle.x, info.robotInformation[enemyID].circle.y);
	printf("Frame: %i. Firepower %i\n", frame, firePower);
	if (firePower % 100 > 30)
	{
		order.fire = 1;
	}
	else
	{
		order.fire = 0;
	}
	frame++;
}

void RobotAI::ChooseArmor(weapontypename& weapon, enginetypename& engine, bool a)
{
	weapon = WT_Cannon;
	engine = ET_UFO;
}

string RobotAI::GetName()
{
	return "Cannon Dust";
}

string RobotAI::GetAuthor()
{
	return "DoorDash";
}


int RobotAI::GetWeaponRed()
{
	return -50;
}
int RobotAI::GetWeaponGreen()
{
	return -50;
}
int RobotAI::GetWeaponBlue()
{
	return -50;
}

int RobotAI::GetEngineRed()
{
	return -50;
}
int RobotAI::GetEngineGreen()
{
	return -50;
}
int RobotAI::GetEngineBlue()
{
	return -50;
}

void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info, int myIDPassin)
{
	myID = myIDPassin;
	enemyID = 1 - myID;
	bullet = new bulletInfo[500];
	myRotationalSpeed = get_engine_rotationSpeed(info.robotInformation[myID].engineTypeName);
	myWpRotSpeed = get_weapon_rotationSpeed(info.robotInformation[myID].weaponTypeName);
	myAcceleration = get_engine_acceleration(info.robotInformation[myID].engineTypeName);

	battlefieldDensity = new double[DENSITY_CALCULATE_TIME * 620 * 620]; // DENSITY_CALCULATE_TIME * 620 * 620
}

void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info, int myID)
{
	delete[] bullet;
	delete[] battlefieldDensity;
}

void RobotAI::onSomeoneFire(int fireID)
{

}

void RobotAI::onHit(int launcherID, bullettypename btn)
{
	printf("Frame: %i hit\n", frame);
}