#ifndef UFO
#define UFO
#include "RobotAI.h"

void reflectAngle(const RobotAI_BattlefieldInformation& info, double &x, double &y, double &vx, double &vy);
void diskMove(const RobotAI_BattlefieldInformation& info, double &x, double &y, double &vx, double &vy, double &engineAngle, const int fire, const int rotation);

#endif