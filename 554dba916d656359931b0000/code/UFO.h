#ifndef UFO
#define UFO
#include "RobotAI.h"
#include "BulletInfo.h"

void reflectAngle(const RobotAI_BattlefieldInformation& info, double &x, double &y, double &vx, double &vy);
void diskMove(const RobotAI_BattlefieldInformation& info, double &x, double &y, double &vx, double &vy, double &engineAngle, const int fire, const int rotation);
int ufoHide(bulletInfo* bullet, const int bulletNum, const RobotAI_BattlefieldInformation& info, const int myID);
int addAmmoRun(const RobotAI_BattlefieldInformation& info, const int myID, const int enemyID, int &wanderingTime);

#endif