#include "RobotAI.h"
#include "Aiming.h"
#include "GeneralFunc.h"
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

double myRotationalSpeed = 0;
double myWpRotSpeed = 0;
double myAcceleration = 0;
const double myRadius = 46;

int enemyAmmoDamage = 0;
int enemyAmmoCapacity = 0;
int enemyEngine = -1; // 0 - spider, 1 - robotman, 2 - afv, 3 - ufo, 4 - shuttle
int enemyWeapon = -1; // 0 - cannon, machinegun, shotgun, plasmatouch, apollo; 1 - prism, tesla; 2 - missilelauncher, 3 - electricsaw, 4 - RPG, 5 - minelayer, 6 - grenadethrower

void RobotAI::Update(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	updatebulletInfo(info, bullet, bulletNum, enemyID);
	if (bulletNum > 0 && (info.robotInformation[myID].remainingAmmo >= 4 || info.robotInformation[enemyID].hp <= info.robotInformation[myID].remainingAmmo / 3 * 25))
	{
		int ord = ufoHide(bullet, bulletNum, info, myID);
		order.run = ord % 2;
		order.eturn = transformOrder(ord);
	}
	else if (info.robotInformation[myID].remainingAmmo < 4 && info.robotInformation[enemyID].hp > info.robotInformation[myID].remainingAmmo / 3 * 25)
	{
		int wTime = 0;
		int ord = addAmmoRun(info, myID, enemyID, wTime);
		if (ord != -1)
		{
			order.run = ord % 2;
			order.eturn = transformOrder(ord);
		}
		else
		{
			ord = ufoHide(bullet, bulletNum, info, myID);
			order.run = ord % 2;
			order.eturn = transformOrder(ord);
		}
	}
	else
	{
		order.run = 1;
		order.eturn = 0;
	}
	order.wturn = aiming(info, info.robotInformation[enemyID].circle.x, info.robotInformation[enemyID].circle.y, myID, myWpRotSpeed);
	int firePower = fractionHit(info, info.robotInformation[myID].circle.x, info.robotInformation[myID].circle.y, 95, info.robotInformation[myID].weaponRotation, 5, 46, info.robotInformation[enemyID].circle.x, info.robotInformation[enemyID].circle.y);
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

	switch (info.robotInformation[enemyID].engineTypeName)
	{
	case ET_Quad:
	case ET_Spider:
		enemyEngine = 0;
		break;
	case ET_Robotman:
		enemyEngine = 1;
		break;
	case ET_AFV:
	case ET_GhostTank:
		enemyEngine = 2;
		break;
	case ET_UFO:
	case ET_XCraft:
		enemyEngine = 3;
		break;
	case ET_Shuttle:
		enemyEngine = 4;
		break;
	}

	switch (info.robotInformation[enemyID].weaponTypeName)
	{
	case WT_Apollo:
	case WT_Cannon:
	case WT_Machinegun:
	case WT_PlasmaTorch:
	case WT_Shotgun:
		enemyWeapon = 0;
		break;
	case WT_Prism:
	case WT_Tesla:
		enemyWeapon = 1;
		break;
	case WT_MissileLauncher:
		enemyWeapon = 2;
		break;
	case WT_ElectricSaw:
		enemyWeapon = 3;
		break;
	case WT_RPG:
		enemyWeapon = 4;
		break;
	case WT_MineLayer:
		enemyWeapon = 5;
		break;
	case WT_GrenadeThrower:
		enemyWeapon = 6;
		break;
	}
	enemyAmmoCapacity = get_weapon_ammo(info.robotInformation[enemyID].weaponTypeName);
	switch (info.robotInformation[enemyID].weaponTypeName)
	{
	case WT_Apollo:
	case WT_Cannon:
		enemyAmmoDamage = 25;
		break;
	case WT_Machinegun:
		enemyAmmoDamage = 7;
		break;
	case WT_PlasmaTorch:
		enemyAmmoDamage = 18;
		break;
	case WT_Shotgun:
		enemyAmmoDamage = 10;
		break;
	case WT_Prism:
		enemyAmmoDamage = 20;
		break;
	case WT_Tesla:
		enemyAmmoDamage = 22;
		break;
	case WT_MissileLauncher:
		enemyAmmoDamage = 15;
		break;
	case WT_ElectricSaw:
		enemyAmmoDamage = 5;
		break;
	case WT_RPG:
		enemyAmmoDamage = 35;
		break;
	case WT_MineLayer:
		enemyAmmoDamage = 4;
		break;
	case WT_GrenadeThrower:
		enemyAmmoDamage = 25;
		break;
	}
}

void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info, int myID)
{
	delete[] bullet;
}

void RobotAI::onSomeoneFire(int fireID)
{

}

void RobotAI::onHit(int launcherID, bullettypename btn)
{
	//printf("Frame: %i hit\n", frame);
}