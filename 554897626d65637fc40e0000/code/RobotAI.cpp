#include "RobotAI.h"
#include "miaozhun.h"
#define dangerousDistance 200

RobotAI::RobotAI()
{
	xGoal = 1000;
	yGoal = 50;
	arrived = true;
	emergency = false;
	times = 0;
	for(int i=0;i<15;i++)
	{
		xTrack[i] = 0;
		yTrack[i] = 0;
	}
}


RobotAI::~RobotAI()
{

}


//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------


void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	double temp;
	temp = takeAim(order, info, myID);
	Beam shootLine;
	shootLine.x = info.robotInformation[myID].circle.x;
	shootLine.y = info.robotInformation[myID].circle.y;
	shootLine.rotation = temp;
	if (willShoot(info, myID, shootLine)) order.fire = 1;
	else order.fire = 0;

	double xNow = info.robotInformation[myID].circle.x;
	double yNow = info.robotInformation[myID].circle.y;
	double xEnemy = info.robotInformation[(myID+1)%2].circle.x;
	double yEnemy = info.robotInformation[(myID+1)%2].circle.y;
	double r  = info.robotInformation[myID].circle.r;
	double yArsenal = info.arsenal[0].circle.y;
	double xArsenal = info.arsenal[0].circle.x;
	double arsenalCD = 0;
	double xArsenalSecond = info.arsenal[1].circle.x;
	double yArsenalSecond = info.arsenal[1].circle.y;
	double arsenalCDSecond = 0;
	
	times++;
	xTrack[times%500]= xEnemy;
	yTrack[times%500]= yEnemy;
	bool steady = false;
	if(pow(abs(xNow-info.arsenal[0].circle.x),2)+pow(abs(yNow-info.arsenal[0].circle.y),2)<pow(abs(xNow-info.arsenal[1].circle.x),2)+pow(abs(yNow-info.arsenal[1].circle.y),2))
	{
		xArsenal = info.arsenal[0].circle.x;
		yArsenal = info.arsenal[0].circle.y;

		arsenalCD = info.arsenal[0].respawning_time;
		xArsenalSecond = info.arsenal[1].circle.x;
		yArsenalSecond = info.arsenal[1].circle.y;

		arsenalCDSecond = info.arsenal[1].respawning_time;
	}
	else
	{
		xArsenal = info.arsenal[1].circle.x;
		yArsenal = info.arsenal[1].circle.y;
		arsenalCD = info.arsenal[1].respawning_time;
		xArsenalSecond = info.arsenal[0].circle.x;
		yArsenalSecond = info.arsenal[0].circle.y;
		arsenalCDSecond = info.arsenal[0].respawning_time;
	}
	int ammoLimit = 0;
	switch (info.robotInformation[(myID+1)%2].weaponTypeName)
	{
		//WT_Cannon,WT_Shotgun,
		//WT_RPG,WT_Machinegun,WT_Prism,WT_Tesla,
		//WT_PlasmaTorch,WT_MissileLauncher,
		//WT_ElectricSaw
	case WT_Prism:
	case WT_Tesla:
	case WT_ElectricSaw:
		ammoLimit = 3;
		break;
	case WT_MissileLauncher:
		ammoLimit = 0;
		break;
	case WT_RPG:
		ammoLimit = 1;
		break;
	case  WT_Shotgun:
	case WT_Cannon:
		ammoLimit= 4;
		break;
	case WT_Machinegun:
		ammoLimit = 10;
		break;
	default:
		ammoLimit = 2;
		break;
	}
	if(pow(xNow-xEnemy,2)+pow(yNow-yEnemy,2)>=pow(dangerousDistance,2))
		emergency = false;
	else
		emergency = true;
	if(!emergency)
	{
		if(arrived)
		{

			arrived = false;
			steady = true;
			for(int i=1;i<500;i++)
			{
				if(xTrack[i-1]!=xTrack[i]||yTrack[i-1]!=yTrack[i])
				{
					steady = false;
					break;
				}
				steady = true;
			}
			//if(/*abs(yNow-yArsenal)>=r+5&&abs(xNow-xArsenal)>=r+5&&arsenalCD<=0&&*/info.robotInformation[myID].remainingAmmo<=ammoLimit)
			if(info.robotInformation[myID].remainingAmmo<=ammoLimit)
			{
				if (arsenalCD<=0&&pow(xEnemy-xArsenal,2)+pow(yEnemy-yArsenal,2)>pow(r+200,2))//(abs(yNow-yArsenal)>=r&&abs(xNow-xArsenal)>=r&&arsenalCD<=0&&pow(xEnemy-xArsenal,2)+pow(yEnemy-yArsenal,2)>pow(r+20,2))
				{
					xGoal=xArsenal;
					yGoal=yArsenal;
				}
				else
				{
					xGoal = xArsenalSecond;
					yGoal = yArsenalSecond;
				}

				TRACE("going to arsenal");
			}
			else
			{
				if(pow(xNow-xEnemy,2)+pow(yNow-yEnemy,2)>=pow(dangerousDistance,2))
				{
					TRACE("Shun Or Attack?");

					if(info.robotInformation[(myID+1)%2].remainingAmmo>=ammoLimit&&!steady)//info.robotInformation[myID].cooling>0)
					{
						double xB1 = info.obstacle[0].x;
						double yB1 = info.obstacle[0].y;
						double xB2 = info.obstacle[1].x;
						double yB2 = info.obstacle[1].y;
						double R = info.obstacle[0].r;
						TRACE("RUN!  SERGENT!");
						//Go around the obostacle;
						if (pow(xNow-300,2)+pow(yNow-250,2)<=pow(xNow-1066,2)+pow(yNow-430,2))
						{
							if(yEnemy>=-xEnemy+xB1+yB1&&yEnemy>=xEnemy+yB1-xB1)
							{
								//xGoal = xB1-R-r;
								//yGoal = yB1+(yB1-yEnemy)*(xB1-xGoal)/(xEnemy-xB1);
								yGoal = yB1-R-r;
								xGoal = xB1-(xEnemy-xB1)*(yB1-yGoal)/(yEnemy-yB1);
							}
							else if (yEnemy<xEnemy+yB1-xB1&&yEnemy>=-xEnemy+xB1+yB1)
							{
								xGoal = xB1-R-r;
								yGoal = yB1 - (yEnemy - yB1)*(xB1-xGoal)/(xEnemy-xB1);
							}
							else if(yEnemy<=xEnemy+yB1-xB1&&yEnemy<-xEnemy+xB1+yB1)
							{
								yGoal= yB1+R+r;
								xGoal = xB1 - (xEnemy-xB1)*(yB1-yGoal)/(yEnemy-yB1);
							}
							else
							{
								xGoal = xB1+R+r;
								yGoal = yB1 - (yEnemy - yB1)*(xB1-xGoal)/(xEnemy-xB1);
							}

						}
						else
						{
							if(yEnemy>=-xEnemy+xB2+yB2&&yEnemy>=xEnemy+yB2-xB2)
							{
								//xGoal = xB1-R-r;
								//yGoal = yB1+(yB1-yEnemy)*(xB1-xGoal)/(xEnemy-xB1);
								yGoal = yB2-R-r;
								xGoal = xB2-(xEnemy-xB2)*(yB2-yGoal)/(yEnemy-yB2);
							}
							else if (yEnemy<xEnemy+yB2-xB2&&yEnemy>=-xEnemy+xB2+yB2)
							{
								xGoal = xB2-R-r;
								yGoal = yB2 - (yEnemy - yB2)*(xB2-xGoal)/(xEnemy-xB2);
							}
							else if(yEnemy<=xEnemy+yB2-xB2&&yEnemy<-xEnemy+xB2+yB2)
							{
								yGoal= yB2+R+r;
								xGoal = xB2 - (xEnemy-xB2)*(yB2-yGoal)/(yEnemy-yB2);
							}
							else
							{
								//xGoal = xB2-R-r;
								xGoal = xB2+R+r;
								yGoal = yB2 - (yEnemy - yB2)*(xB2-xGoal)/(xEnemy-xB2);
							}
							
						}
						
						//But go and punch its face if he stay there for too long
						
					}

					else
					{
						TRACE("ATTACK Sergent!");
						if (abs(xNow-xEnemy)>=abs(yNow-yEnemy))
						{
							xGoal = xNow;
							yGoal = info.robotInformation[(myID+1)%2].circle.y;
						}
						else
						{

							yGoal = yNow;
							xGoal = info.robotInformation[(myID+1)%2].circle.x;
						}
						if(info.robotInformation[(myID+1)%2].weaponTypeName==WT_MissileLauncher)
						{
							if(pow(xEnemy-info.arsenal[0].circle.x,2)+pow(yEnemy-info.arsenal[0].circle.y,2)>pow(xEnemy+info.robotInformation[(myID+1)%2].vx-info.arsenal[0].circle.x,2)+pow(yEnemy+info.robotInformation[(myID+1)%2].vy-info.arsenal[0].circle.y,2))
							{
								xGoal = info.arsenal[0].circle.x;
								yGoal = info.arsenal[0].circle.y;
							}
							else
							{
								xGoal = info.arsenal[1].circle.x;
								yGoal = info.arsenal[1].circle.y;
							}
						}
						
						/*				xGoal = info.robotInformation[(myID+1)%2].circle.x;
						yGoal = info.robotInformation[(myID+1)%2].circle.y;*/
					}
				}

			}
			/*if(yGoal==50)
			{
			xGoal = xArsenal;
			yGoal = yArsenal;
			}
			else
			{
			xGoal = 50;
			yGoal = 50;
			}*/

		}
		else
		{
			if(info.robotInformation[myID].vx==0&&info.robotInformation[myID].vy)
			{
				if(xNow<=500&&yNow<=340)
				{
					xGoal = 50;
					yGoal = 50;
				}
				else if (xNow<=500&&yNow>340)
				{
					xGoal = info.arsenal[0].circle.x;
					yGoal = info.arsenal[0].circle.y;
				}
				else if (xNow>500&&yNow<=340)
				{
					xGoal = info.arsenal[1].circle.x;
					yGoal = info.arsenal[1].circle.y;
				}
				else
				{
					xGoal = 1066;
					yGoal =680;
				}
				/*if(pow(xNow-300,2)+pow(yNow-250,2)<=pow(75+r,2)+10)
				{
					if(xNow<300&&yNow<250)
					{
						xGoal=x[(int)xNow/10+8];
						yGoal=y[(int)yNow/10-4];

					}	
					else if(xNow<300&&yNow>=250)
					{
						xGoal= x[(int)xNow/10+8];
						yGoal= y[(int)yNow/10+4];
					}
					else if (xNow>=300&&yNow<250)
					{
						xGoal=x[(int)xNow/10-7];
						yGoal=y[(int)yNow/10-4];

					}
					else
					{
						xGoal=x[(int)xNow/10-7];
						yGoal=y[(int)yNow/10+4];
					}
					//headTo(order,info,myID);
					//cout<<xGoal<<','<<yGoal<<'\n';

				}
				else if (pow(xNow-1066,2)+pow(yNow-430,2)<=pow(75+r,2)+10)
				{
					if(xNow<1066&&yNow<430)
					{
						xGoal=x[(int)xNow/10+8];
						yGoal=y[(int)yNow/10-4];

					}	
					else if(xNow<1066&&yNow>=430)
					{
						xGoal= x[(int)xNow/10+8];
						yGoal= y[(int)yNow/10+4];
					}
					else if (xNow>=1066&&yNow<430)
					{
						xGoal=x[(int)xNow/10-7];
						yGoal=y[(int)yNow/10-4];

					}
					else
					{
						xGoal=x[(int)xNow/10-7];
						yGoal=y[(int)yNow/10+4];
					}
					*/

				}
			}
	}
	
	else
	{
		xGoal = xNow+xNow-xEnemy;
		yGoal = yNow+yNow-yEnemy;
	}
	if(xGoal>=1366-r)
		xGoal = 1366-r;
	if(yGoal>=680-r)
		yGoal = 680-r;
	if(xGoal<=r)
		xGoal = r;
	if(yGoal<=r)
		yGoal =r;
	
	



	/*if(info.robotInformation[myID].vx==0&&info.robotInformation[myID].vy)
	{
		if(pow(xNow-300,2)+pow(yNow-250,2)<=pow(75+r,2)+10)
		{
			if(xNow<300&&yNow<250)
			{
				xGoal=x[(int)xNow/10+8];
				yGoal=y[(int)yNow/10-4];

			}	
			else if(xNow<300&&yNow>=250)
			{
				xGoal= x[(int)xNow/10+8];
				yGoal= y[(int)yNow/10+4];
			}
			else if (xNow>=300&&yNow<250)
			{
				xGoal=x[(int)xNow/10-7];
				yGoal=y[(int)yNow/10-4];

			}
			else
			{
				xGoal=x[(int)xNow/10-7];
				yGoal=y[(int)yNow/10+4];
			}
			//headTo(order,info,myID);
			//cout<<xGoal<<','<<yGoal<<'\n';

		}
		else if (pow(xNow-1066,2)+pow(yNow-430,2)<=pow(75+r,2)+10)
		{
			if(xNow<1066&&yNow<430)
			{
				xGoal=x[(int)xNow/10+8];
				yGoal=y[(int)yNow/10-4];

			}	
			else if(xNow<1066&&yNow>=430)
			{
				xGoal= x[(int)xNow/10+8];
				yGoal= y[(int)yNow/10+4];
			}
			else if (xNow>=1066&&yNow<430)
			{
				xGoal=x[(int)xNow/10-7];
				yGoal=y[(int)yNow/10-4];

			}
			else
			{
				xGoal=x[(int)xNow/10-7];
				yGoal=y[(int)yNow/10+4];
			}


		}
	}*/
	//How to defeat Mr.Octopus?
	headTo(order,info,myID);
	//order.fire=1;
	//latest!!!!

}



void RobotAI::ChooseArmor(weapontypename& weapon,enginetypename& engine,bool a)
{
	//挑选装备函数
	//功能：在战斗开始时为你的机甲选择合适的武器炮塔和引擎载具
	//参数：weapon	...	代表你选择的武器，在函数体中给它赋值
	//		engine	...	代表你选择的引擎，在函数体中给它赋值
	//tip:	括号里的参数是枚举类型 weapontypename 或 enginetypename
	//		开发文档中有详细说明，你也可以在RobotAIstruct.h中直接找到它们的代码
	//tip:	最后一个bool是没用的。。那是一个退化的器官

	weapon = WT_Prism;	//啊，我爱加农炮
	engine = ET_Shuttle;	//啊，我爱小蜘蛛
}





//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "C";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "Leviathan";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return -50;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return -50;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return -50;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return -50;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return -50;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return -50;
}









//-----------------------------------------------------
//3.用不用随你的触发函数
//-----------------------------------------------------


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标

}

void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗结束时被调用，可能可以用来析构你动态分配的内存空间（如果你用了的话）
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
}


void RobotAI::onSomeoneFire(int fireID)
{
	//有机甲开火时被调用
	//参数：fireID	... 开火的机甲下标
}


void RobotAI::onHit(int launcherID,bullettypename btn)
{
	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
}



//TODO:这里可以实现你自己的函数
void RobotAI::headTo(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, int myID)
{
	double xNow = info.robotInformation[myID].circle.x;
	double yNow = info.robotInformation[myID].circle.y;
	double r = info.robotInformation[myID].circle.r;
	switch (info.robotInformation[myID].engineTypeName)
	{
	case ET_Spider:
		TRACE("Execute once!");
		if (pow(xNow - xGoal, 2) + pow(yNow - yGoal, 2) <= pow(r, 2) + 10)
			arrived = true;
		else
		{


			if (abs(yNow - yGoal)>r / 2)
			{
				if (yNow>yGoal)
				{
					order.run = 3;
					TRACE("Going up!");
				}
				else
				{
					order.run = 4;
					TRACE("Going Down!");
				}
			}
			else
			{

				if (xNow>xGoal)
				{
					order.run = 1;
					TRACE("Going left");
				}
				else
				{
					order.run = 2;
					TRACE("Going right");
				}

			}
		}
		break;
	case ET_Robotman:
	{
		int last;
		bool attack = false;
		if (last = info.robotInformation[(myID + 1) % 2].hp)
		{
			attack = true;

		}
		last = info.robotInformation[(myID + 1) % 2].hp;
	}
	}
}