#ifndef AIMING
#define AIMING
#include "RobotAI.h"

int aiming(const RobotAI_BattlefieldInformation& info, const double aimingX, const double aimingY, const int myID, const double myWpRotSpeed);
int fractionHit(const RobotAI_BattlefieldInformation& info, double myX, double myY, double myWpLength, double myWpRotAngle, double myWpUncertAngle, double enemyRadius, double enemyX, double enemyY);

#endif