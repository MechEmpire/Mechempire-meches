#include "Battlefield.h"
#include "BulletInfo.h"

void updatebulletInfo(const RobotAI_BattlefieldInformation& info, bulletInfo *bullet, int& bulletNum, const int enemyID)
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
			(*(bullet + bulletNum)).tag = bulletNum;
			bulletNum++;
		}
	}
}

void updateBattlefieldDensity(const RobotAI_BattlefieldInformation& info, const int myCurrentX, const int myCurrentY, const int myID, const int myRadius, const int DENSITY_CALCULATE_TIME, bulletInfo *bullet, const int bulletNum, double* battlefieldDensity)
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
			if (isOutOfMap(info, (*(bullet + i)).x, (*(bullet + i)).y))
			{
				break;
			}
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

bool isOutOfMap(const RobotAI_BattlefieldInformation& info, const double x, const double y)
{
	if (x < 0 || x > 1366 || y < 0 || y > 680)
	{
		return true;
	}
	for (int i = 0; i < 2; i++)
	{
		if (pow(x - info.obstacle[i].x, 2) + pow(y - info.obstacle[i].y, 2) < pow(info.obstacle[i].r, 2))
		{
			return true;
		}
	}
	return false;
}

void bulletUpdate(const RobotAI_BattlefieldInformation& info, const bulletInfo *bullet, const int bulletNum, bulletInfo *bulletNew, int bulletNewNum[], const int updateTimes)
{
	for (bulletNewNum[0] = 0; bulletNewNum[0] < bulletNum; bulletNewNum[0]++)
	{
		*(bulletNew + bulletNewNum[0]) = *(bullet + bulletNewNum[0]);
	}
	for (int time = 1; time < updateTimes; time++)
	{
		int deletedBullet = 0;
		for (int i = 0; i < bulletNewNum[time - 1]; i++)
		{
			*(bulletNew + time * 500 + i) = *(bulletNew + (time - 1) * 500 + i);
			(*(bulletNew + time * 500 + i)).x += (*(bulletNew + time * 500 + i)).vx;
			(*(bulletNew + time * 500 + i)).y += (*(bulletNew + time * 500 + i)).vy;

			if (isOutOfMap(info, (*(bulletNew + time * 500 + i)).x, (*(bulletNew + time * 500 + i)).y))
			{
				deletedBullet++;
			}
			else
			{
				*(bulletNew + time * 500 + i - deletedBullet) = *(bulletNew + time * 500 + i);
			}
		}

		bulletNewNum[time] = bulletNewNum[time - 1] - deletedBullet;
	}
}