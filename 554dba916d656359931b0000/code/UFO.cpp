#include "UFO.h"
#include "BulletInfo.h"
#include "Battlefield.h"
#include "GeneralFunc.h"

void reflectAngle(const RobotAI_BattlefieldInformation& info, double &x, double &y, double &vx, double &vy)
{
	if (x < 46)
	{
		x = 46;
		vx = -vx;
	}
	else if (x > info.boundary.width - 46)
	{
		x = info.boundary.width - 46;
		vx = -vx;
	}

	if (y < 46)
	{
		y = 46;
		vy = -vy;
	}
	else if (y > info.boundary.height - 46)
	{
		y = info.boundary.height - 46;
		vy = -vy;
	}

	int obstacleHit = 2;
	double properDist = info.obstacle[0].r + 46;

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

void diskMove(const RobotAI_BattlefieldInformation& info, double &x, double &y, double &vx, double &vy, double &engineAngle, const int fire, const int rotation)
{
	if (fire > 0)
	{
		vx += 0.2 * cos(AngleToRadian(engineAngle));
		vy += 0.2 * sin(AngleToRadian(engineAngle));
	}

	if (pow(vx, 2) + pow(vy, 2) > 36)
	{
		double v = sqrt(pow(vx, 2) + pow(vy, 2));
		vx = 6 / v * vx;
		vy = 6 / v * vy;
	}

	if (rotation == 1)
	{
		engineAngle += 4.5;
		AngleAdjust(engineAngle);
	}
	else if (rotation == -1)
	{
		engineAngle -= 4.5;
		AngleAdjust(engineAngle);
	}

	x += vx;
	y += vy;

	reflectAngle(info, x, y, vx, vy);
}

int ufoHide(bulletInfo* bullet, const int bulletNum, const RobotAI_BattlefieldInformation& info, const int myID)
{
	const int ForecastTimes = 4; // change following with comment
	const int SkipTimes = 5;
	bulletInfo *bulletChange = new bulletInfo[500 * ForecastTimes * SkipTimes];
	int bulletChangeNum[ForecastTimes * SkipTimes];
	double *possSol = new double[1296](); //
	for (int i = 0; i < int(pow(6, ForecastTimes)); i++)
	{
		*(possSol + i) = 0;
	}
	int bestSol = -1;
	double bestSolRes = -1;

	bulletUpdate(info, bullet, bulletNum, bulletChange, bulletChangeNum, ForecastTimes * SkipTimes);

	for (int i = 0; i < int(pow(6, ForecastTimes)); i++)
	{
		double myX = info.robotInformation[myID].circle.x;
		double myY = info.robotInformation[myID].circle.y;
		double myVX = info.robotInformation[myID].vx;
		double myVY = info.robotInformation[myID].vy;
		double myEngRot = info.robotInformation[myID].engineRotation;

		int crashedBullet[500];
		int crashedBulletNum = 0;

		for (int j = 0; j < ForecastTimes; j++)
		{
			int order = int(i / int(pow(6, j))) % 6;

			for (int rep = 0; rep < SkipTimes; rep++)
			{
				diskMove(info, myX, myY, myVX, myVY, myEngRot, order % 2, transformOrder(order));
				for (int bn = 0; bn < bulletChangeNum[j * SkipTimes + rep]; bn++)
				{
					bool isSkip = false;

					for (int cbn = 0; cbn < crashedBulletNum; cbn++)
					{
						if (crashedBullet[cbn] == (*(bulletChange + (j * SkipTimes + rep) * 500 + bn)).tag)
						{
							isSkip = true;
						}
					}
					double x = (*(bulletChange + (j * SkipTimes + rep) * 500 + bn)).x;
					double y = (*(bulletChange + (j * SkipTimes + rep) * 500 + bn)).y;
					double vx = (*(bulletChange + (j * SkipTimes + rep) * 500 + bn)).vx;
					double vy = (*(bulletChange + (j * SkipTimes + rep) * 500 + bn)).vy;

					if (pow(myX - x, 2) + pow(myY - y, 2) < 46 * 46)
					{
						if (!isSkip)
						{
							*(possSol + i) += 100;
							crashedBullet[crashedBulletNum] = (*(bulletChange + (j * SkipTimes + rep) * 500 + bn)).tag;
							crashedBulletNum++;
							isSkip = true;
						}
					}
					else if (pow(myX - x, 2) + pow(myY - y, 2) < 500 * 500)
					{
						double relX = myX - x;
						double relY = myY - y;
						if (!isSkip && (relX * vx + relY * vy > 0))
						{
							*(possSol + i) += 1 / sqrt(relX * relX + relY * relY) / abs(sin(acos((relX * vx + relY * vy) / sqrt(relX * relX + relY * relY) / sqrt(vx * vx + vy * vy)))) / sqrt(relX * relX + relY * relY);
						}
					}
				}
			}
		}
		if (i == 0 || bestSolRes > *(possSol + i))
		{
			bestSolRes = *(possSol + i);
			bestSol = i;
		}
	}
	delete[] bulletChange;
	delete[] possSol;

	return (bestSol % 6);
}

int addAmmoRun(const RobotAI_BattlefieldInformation& info, const int myID, const int enemyID, int &wanderingTime)
{
	double coolingTime[2];
	double distanceToAmmo[2][2];
	int selectedAmmo = 0;
	for (int i = 0; i < 2; i++)
	{
		distanceToAmmo[0][i] = sqrt(pow(info.robotInformation[myID].circle.x - info.arsenal[i].circle.x, 2) + pow(info.robotInformation[myID].circle.y - info.arsenal[i].circle.y, 2));
		distanceToAmmo[1][i] = sqrt(pow(info.robotInformation[enemyID].circle.x - info.arsenal[i].circle.x, 2) + pow(info.robotInformation[enemyID].circle.y - info.arsenal[i].circle.y, 2));
		coolingTime[i] = info.arsenal[i].respawning_time - distanceToAmmo[0][i] / 6 - 30;
	}

	if (distanceToAmmo[0][0] <= distanceToAmmo[0][1]) // 0 is near me
	{
		if (coolingTime[0] <= 0 && distanceToAmmo[0][0] <= distanceToAmmo[1][0])
		{
			selectedAmmo = 0;
		}
		else if (coolingTime[0] <= 0 && distanceToAmmo[0][0] > distanceToAmmo[1][0])
		{
			if (coolingTime[1] <= 0)
			{
				selectedAmmo = 1;
			}
			else
			{
				wanderingTime = int(coolingTime[1]);
				return -1;
			}
		}
		else if (coolingTime[1] <= 0 && distanceToAmmo[0][1] <= distanceToAmmo[1][1])
		{
			selectedAmmo = 1;
		}
		else if (coolingTime[1] <= 0 && distanceToAmmo[0][1] > distanceToAmmo[1][1])
		{
			selectedAmmo = 1;
		}
		else{
			if (info.robotInformation[enemyID].remainingAmmo == 0)
			{
				if (coolingTime[0] < coolingTime[1])
				{
					selectedAmmo = 0;
				}
				else
				{
					selectedAmmo = 1;
				}
			}
			else
			{
				if (coolingTime[0] < coolingTime[1])
				{
					wanderingTime = int(coolingTime[0]);
					return -1;
				}
				else
				{
					wanderingTime = int(coolingTime[1]);
					return -1;
				}
			}
		}
	}
	else // 1 is near me
	{
		if (coolingTime[1] <= 0 && distanceToAmmo[0][1] < distanceToAmmo[1][1])
		{
			selectedAmmo = 1;
		}
		else if (coolingTime[1] <= 0 && distanceToAmmo[0][1] > distanceToAmmo[1][1])
		{
			if (coolingTime[0] <= 0)
			{
				selectedAmmo = 0;
			}
			else
			{
				wanderingTime = int(coolingTime[0]);
				return -1;
			}
		}
		else if (coolingTime[0] <= 0 && distanceToAmmo[0][0] <= distanceToAmmo[1][0])
		{
			selectedAmmo = 0;
		}
		else if (coolingTime[0] <= 0 && distanceToAmmo[0][0] > distanceToAmmo[1][0])
		{
			selectedAmmo = 0;
		}
		else{
			if (info.robotInformation[enemyID].remainingAmmo == 0)
			{
				if (coolingTime[0] < coolingTime[1])
				{
					selectedAmmo = 0;
				}
				else
				{
					selectedAmmo = 1;
				}
			}
			else
			{
				if (coolingTime[0] < coolingTime[1])
				{
					wanderingTime = int(coolingTime[0]);
					return -1;
				}
				else
				{
					wanderingTime = int(coolingTime[1]);
					return -1;
				}
			}
		}
	}

	const int ForecastTimes = 5; // change following with comment
	const int SkipTimes = 5;
	int bestSol = -1;
	double bestSolRes = -1;

	for (int i = 0; i < int(pow(6, ForecastTimes)); i++)
	{
		double myX = info.robotInformation[myID].circle.x;
		double myY = info.robotInformation[myID].circle.y;
		double myVX = info.robotInformation[myID].vx;
		double myVY = info.robotInformation[myID].vy;
		double myEngRot = info.robotInformation[myID].engineRotation;

		for (int j = 0; j < ForecastTimes; j++)
		{
			int order = int(i / int(pow(6, j))) % 6;

			for (int rep = 0; rep < SkipTimes; rep++)
			{
				diskMove(info, myX, myY, myVX, myVY, myEngRot, order % 2, transformOrder(order));
			}
		}
		if (i == 0 || bestSolRes > sqrt(pow(myX + myVX - info.arsenal[selectedAmmo].circle.x, 2) + pow(myY + myVY - info.arsenal[selectedAmmo].circle.y, 2)))
		{
			bestSolRes = sqrt(pow(myX + myVX - info.arsenal[selectedAmmo].circle.x, 2) + pow(myY + myVY - info.arsenal[selectedAmmo].circle.y, 2));
			bestSol = i;
		}
	}
	return bestSol % 6;
}