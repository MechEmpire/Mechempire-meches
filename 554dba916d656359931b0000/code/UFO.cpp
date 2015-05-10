#include "UFO.h"

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