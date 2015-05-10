#include "RobotAI.h"

RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}

struct bulletInfo
{
	bullettypename bulletType;
	double x;
	double y;
	double vx;
	double vy;
};

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
double lastForecastX;
double lastForecastY;
double lastForecastVX;
double lastForecastVY;
double lastForecastEngineAngle;

int aiming(const RobotAI_BattlefieldInformation& info, double aimingX, double aimingY)
{
	double myX = info.robotInformation[myID].circle.x;
	double myY = info.robotInformation[myID].circle.y;
	double wpAngle = info.robotInformation[myID].weaponRotation;
	double aimAngle = RadianToAngle(atan2(aimingY - myY, aimingX - myX));
	double movAngle = wpAngle - aimAngle;
	AngleAdjust(movAngle);

	if (movAngle < 0 && abs(movAngle) > myWpRotSpeed / 2.0)
	{
		return 1;
	}
	else if (movAngle > 0 && abs(movAngle) > myWpRotSpeed / 2.0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
	return 0;
}

void reflectAngle(const RobotAI_BattlefieldInformation& info, double &x, double &y, double &vx, double &vy)
{
	if (x < myRadius)
	{
		x = myRadius;
		vx = -vx;
	}
	else if (x > info.boundary.width - myRadius)
	{
		x = info.boundary.width - myRadius;
		vx = -vx;
	}

	if (y < myRadius)
	{
		y = myRadius;
		vy = -vy;
	}
	else if (y > info.boundary.height - myRadius)
	{
		y = info.boundary.height - myRadius;
		vy = -vy;
	}

	int obstacleHit = 2;
	double properDist = info.obstacle[0].r + myRadius;

	if (pow(x - info.obstacle[0].x, 2) + pow(y - info.obstacle[0].y, 2) < pow(properDist, 2))
	{
		obstacleHit = 0;
	}
	else if (pow(x - info.obstacle[1].x, 2) + pow(y - info.obstacle[1].y, 2) < pow(properDist, 2))
	{
		obstacleHit = 1;
	}

	if (obstacleHit != 2)
	{
		double distX = x - info.obstacle[obstacleHit].x;
		double distY = y - info.obstacle[obstacleHit].y;
		double angle = atan2(distY, distX); // Radians!

		double vOrth = vx * cos(-angle) - vy * sin(-angle);
		double vTang = vx * sin(-angle) + vy * cos(-angle);

		x = properDist * cos(angle) + info.obstacle[obstacleHit].x;
		y = properDist * sin(angle) + info.obstacle[obstacleHit].y;

		vOrth = -vOrth;
		vx = vOrth * cos(-angle) + vTang * sin(-angle);
		vy = -vOrth * sin(-angle) + vTang * cos(-angle);
	}
}

void diskMove(const RobotAI_BattlefieldInformation& info, double &x, double &y, double &vx, double &vy, double &engineAngle, int fire, int rotation)
{

	if (fire > 0)
	{
		vx += myAcceleration * cos(AngleToRadian(engineAngle));
		vy += myAcceleration * sin(AngleToRadian(engineAngle));
	}
	else if (fire < 0)
	{
		//vx -= myAcceleration * cos(AngleToRadian(engineAngle));
		//vy -= myAcceleration * sin(AngleToRadian(engineAngle));
	}

	if (pow(vx, 2) + pow(vy, 2) > 36)
	{
		double v = sqrt(pow(vx, 2) + pow(vy, 2));
		vx = 6 / v * vx;
		vy = 6 / v * vy;
	}

	if (rotation == 1)
	{
		engineAngle += myRotationalSpeed;
		AngleAdjust(engineAngle);
	}
	else if (rotation == -1)
	{
		engineAngle -= myRotationalSpeed;
		AngleAdjust(engineAngle);
	}

	x += vx;
	y += vy;

	reflectAngle(info, x, y, vx, vy);
}

void updateBattlefieldDensity(const RobotAI_BattlefieldInformation& info)
{
	for (int i = 0; i < DENSITY_CALCULATE_TIME * 620 * 620; i++)
	{
		(*(battlefieldDensity + i)) = 0;
	}

	for (int i = 0; i < bulletNum; i++)
	{
		for (int time = 0; time < DENSITY_CALCULATE_TIME; time++)
		{
			for (int xTest = int(floor((*(bullet + i)).x - myRadius)); xTest < int(ceil((*(bullet + i)).x + myRadius)); xTest++)
			{
				for (int yTest = int(floor((*(bullet + i)).y - myRadius)); yTest < int(ceil((*(bullet + i)).y + myRadius)); yTest++)
				{
					if (xTest >= myRadius && xTest <= 1366 - myRadius && xTest >= myCurrentX - 300 && xTest <= myCurrentX + 300 && yTest >= myRadius && yTest <= 680 - myRadius && 
						yTest >= myCurrentY - 300 && yTest <= myCurrentY + 300 && pow(xTest - (*(bullet + i)).x, 2) + pow(yTest - (*(bullet + i)).y, 2) <= pow(myRadius + 1, 2))
					{
						(*(battlefieldDensity + time * 620 * 620 + (xTest - myCurrentX + 300) * 620 + (yTest - myCurrentY + 300))) += 1;
					}
				}
			}
			(*(bullet + i)).x += (*(bullet + i)).vx;
			(*(bullet + i)).y += (*(bullet + i)).vy;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		double xObs = info.obstacle[i].x;
		double yObs = info.obstacle[i].y;
		double rObs = info.obstacle[i].r;

		for (int xTest = int(floor(xObs - myRadius - rObs)); xTest < int(ceil(xObs + myRadius + rObs)); xTest++)
		{
			for (int yTest = int(floor(yObs - myRadius - rObs)); yTest < int(ceil(yObs + myRadius + rObs)); yTest++)
			{
				if (xTest >= myCurrentX - 300 && xTest <= myCurrentX + 300 && yTest >= myCurrentY - 300 && yTest <= myCurrentY + 300 && pow(xTest - xObs, 2) + pow(yTest - yObs, 2) <= pow(myRadius + rObs, 2))
				{
					for (int time = 0; time < DENSITY_CALCULATE_TIME; time++)
					{
						(*(battlefieldDensity + time * 620 * 620 + (xTest - myCurrentX + 300) * 620 + (yTest - myCurrentY + 300))) += 10000;
					}
				}
			}
		}
	}
}

void diskBestOperation(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info)
{
	double *nextTenMove = new double[1296];

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
				diskMove(info, x, y, vx, vy, engineAngle, ord / 2, ord % 3 - 1);

				*(nextTenMove + i) += *(battlefieldDensity + (time * 10 + n) * 620 * 620 + (int(floor(x)) - myCurrentX + 300) * 620 + (int(floor(y)) - myCurrentY + 300));
			}

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

	order.run = (leastIndex % 6) / 2;
	order.eturn = (leastIndex % 6) % 3 - 1;
}

void updatebulletInfo(const RobotAI_BattlefieldInformation& info)
{
	bulletNum = 0;
	for (int i = 0; i < info.num_bullet; i++){
		if (info.bulletInformation[i].launcherID == enemyID)
		{
			(*(bullet + bulletNum)).bulletType = info.bulletInformation[i].type;
			(*(bullet + bulletNum)).x = info.bulletInformation[i].circle.x;
			(*(bullet + bulletNum)).y = info.bulletInformation[i].circle.y;
			(*(bullet + bulletNum)).vx = info.bulletInformation[i].vx;
			(*(bullet + bulletNum)).vy = info.bulletInformation[i].vy;
			bulletNum++;
		}
	}
}

void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	//printf("%i", frame);
	myCurrentX = int(floor(info.robotInformation[myID].circle.x));
	myCurrentY = int(floor(info.robotInformation[myID].circle.y));
	updatebulletInfo(info);
	updateBattlefieldDensity(info);
	diskBestOperation(order, info);
	order.wturn = aiming(info, info.robotInformation[enemyID].circle.x, info.robotInformation[enemyID].circle.y);
	order.fire = 1;
	frame++;

	lastForecastX = info.robotInformation[myID].circle.x;
	lastForecastY = info.robotInformation[myID].circle.y;
	lastForecastVX = info.robotInformation[myID].vx;
	lastForecastVY = info.robotInformation[myID].vy;
	lastForecastEngineAngle = info.robotInformation[myID].engineRotation;
	diskMove(info, lastForecastX, lastForecastY, lastForecastVX, lastForecastVY, lastForecastEngineAngle, order.run, order.eturn);
}

void RobotAI::ChooseArmor(weapontypename& weapon, enginetypename& engine, bool a)
{
	weapon = WT_Tesla;
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

	battlefieldDensity = new double[DENSITY_CALCULATE_TIME * 620 * 620]; // DENSITY_CALCULATE_TIME * 1366 * 680
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

}