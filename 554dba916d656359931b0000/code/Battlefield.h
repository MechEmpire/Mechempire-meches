#ifndef BATTLEFIELD
#define BATTLEFIELD

#include "RobotAI.h"
#include "BulletInfo.h"

void updatebulletInfo(const RobotAI_BattlefieldInformation& info, bulletInfo *bullet, int& bulletNum, const int enemyID);
void updateBattlefieldDensity(const RobotAI_BattlefieldInformation& info, const int myCurrentX, const int myCurrentY, const int myID, const int myRadius, const int DENSITY_CALCULATE_TIME, bulletInfo *bullet, const int bulletNum, double* battlefieldDensity);
bool isOutOfMap(const RobotAI_BattlefieldInformation& info, const double x, const double y);
void bulletUpdate(const RobotAI_BattlefieldInformation& info, const bulletInfo *bullet, const int bulletNum, bulletInfo *bulletNew, int bulletNewNum[], const int updateTimes);

#endif