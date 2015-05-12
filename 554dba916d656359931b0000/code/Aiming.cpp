#include "Aiming.h"

int aiming(const RobotAI_BattlefieldInformation& info, const double aimingX, const double aimingY, const int myID, const double myWpRotSpeed)
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

int fractionHit(const RobotAI_BattlefieldInformation& info, double myX, double myY, double myWpLength, double myWpRotAngle, double myWpUncertAngle, double enemyRadius, double enemyX, double enemyY)
{
	double startX, startY;
	startX = myX + myWpLength * cos(AngleToRadian(myWpRotAngle));
	startY = myY + myWpLength * sin(AngleToRadian(myWpRotAngle));

	double maxAngle = myWpRotAngle + myWpUncertAngle;
	double minAngle = myWpRotAngle - myWpUncertAngle;
	double angleRange = myWpUncertAngle * 2;

	double maxAngleList[3], minAngleList[3];

	for (int i = 0; i < 2; i++)
	{
		double angle = RadianToAngle(atan2((info.obstacle[i].y - startY), (info.obstacle[i].x - startX)));
		double dAngle = RadianToAngle(atan2(info.obstacle[i].r, sqrt(pow(info.obstacle[i].y - startY, 2) + pow(info.obstacle[i].x - startX, 2))));
		minAngleList[i] = angle - dAngle;
		maxAngleList[i] = angle + dAngle;

		double a = sqrt(pow(info.obstacle[i].y - startY, 2) + pow(info.obstacle[i].x - startX, 2));
		double b = sqrt(pow(info.obstacle[i].y - enemyY, 2) + pow(info.obstacle[i].x - enemyX, 2));
		double c = sqrt(pow(startY - enemyY, 2) + pow(startX - enemyX, 2));

		double r = RadianToAngle(acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / 2 / a / b));
		if (r < 90 - dAngle)
		{
			minAngleList[i] = 0;
			maxAngleList[i] = 0;
		}
	}

	double objAngle = RadianToAngle(atan2((enemyY - startY), (enemyX - startX)));
	double dObjAngle = RadianToAngle(atan2(enemyRadius, sqrt(pow((enemyY - startY), 2) + pow(enemyX - startX, 2))));
	minAngleList[2] = objAngle - dObjAngle;
	maxAngleList[2] = objAngle + dObjAngle;
	
	for (int i = 0; i < 3; i++)
	{
		if (minAngleList[i] < minAngle - 180)
		{
			minAngleList[i] += 360;
			maxAngleList[i] += 360;
		}
		else if (minAngleList[i] > minAngle + 180)
		{
			minAngleList[i] -= 360;
			maxAngleList[i] -= 360;
		}

		if (minAngleList[i] > minAngle && minAngleList[i] < maxAngle)
		{
			if (maxAngleList[i] > maxAngle)
			{
				maxAngleList[i] = maxAngle;
			}
		}
		else if (maxAngleList[i] > minAngle && maxAngleList[i] < maxAngle)
		{
			if (minAngleList[i] < minAngle)
			{
				minAngleList[i] = minAngle;
			}
		}
		else if (maxAngleList[i] > maxAngle && minAngleList[i] < minAngle)
		{
			maxAngleList[i] = maxAngle;
			minAngleList[i] = minAngle;
		}
		else
		{
			maxAngleList[i] = 0;
			minAngleList[i] = 0;
		}
	}
	//printf("%f, %f\n", maxAngleList[2], maxAngleList[2]);

	double blockedAngle[2][2];
	
	for (int i = 0; i < 2; i++)
	{
		blockedAngle[i][0] = minAngleList[i];
		blockedAngle[i][1] = maxAngleList[i];
	}

	if (blockedAngle[1][0] < blockedAngle[0][0])
	{
		double temp[2];
		for (int i = 0; i < 2; i++)
		{
			temp[i] = blockedAngle[1][i];
			blockedAngle[1][i] = blockedAngle[0][i];
			blockedAngle[0][i] = temp[i];
		}
	}

	if (blockedAngle[1][0] < blockedAngle[0][1])
	{
		blockedAngle[1][0] = blockedAngle[0][1];
		if (blockedAngle[1][1] < blockedAngle[1][0])
		{
			blockedAngle[1][1] = blockedAngle[1][0];
		}
	}

	int ratioBlocked = int(floor((blockedAngle[0][1] - blockedAngle[0][0] + blockedAngle[1][1] - blockedAngle[1][0]) / angleRange * 100));

	for (int i = 0; i < 2; i++)
	{
		if (minAngleList[2] > blockedAngle[i][0] && minAngleList[2] < blockedAngle[i][1])
		{
			minAngleList[2] = blockedAngle[i][1];
		}
		if (maxAngleList[2] > blockedAngle[i][0] && maxAngleList[2] < blockedAngle[i][1])
		{
			maxAngleList[2] = blockedAngle[i][0];
		}
	}
	
	if (maxAngleList[2] < minAngleList[2])
	{
		maxAngleList[2] = minAngleList[2];
	}

	int ratioHit = int(floor((maxAngleList[2] - minAngleList[2]) / angleRange * 100));
	if (ratioHit > 0)
	{
		ratioHit--;
	}
	return (ratioBlocked) * 100 + ratioHit;
}