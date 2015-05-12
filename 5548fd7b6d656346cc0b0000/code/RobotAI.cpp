#include "RobotAI.h"

RobotAI::RobotAI()
{

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

	double vspo[2],vspi[2];
	double dsx[2],dsy[2];
	double dd[2];//障碍间距
	dsx[0]=info.obstacle[0].x - info.robotInformation[myID].circle.x;dsy[0]=info.obstacle[0].y-info.robotInformation[myID].circle.y;
	dd[0] = sqrt(pow(dsx[0],2)+pow(dsy[0],2));
	
	dsx[1]=info.obstacle[1].x - info.robotInformation[myID].circle.x;dsy[1]=info.obstacle[1].y-info.robotInformation[myID].circle.y;
	dd[1] = sqrt(pow(dsx[1],2)+pow(dsy[1],2));
	
	vspi[0] = asin(info.obstacle[0].r/dd[0])*180/PI;//误差
	vspo[0] = atan2(dsy[0],dsx[0])*180/PI;//圆心

	vspi[1] = asin(info.obstacle[1].r/dd[1])*180/PI;
	vspo[1] = atan2(dsy[1],dsx[1])*180/PI;
				
	double dssx[2],dssy[2];
	dssx[0]=info.arsenal[0].circle.x - info.robotInformation[myID].circle.x;
	dssy[0]=info.arsenal[0].circle.y - info.robotInformation[myID].circle.y;
	dssx[1]=info.arsenal[1].circle.x - info.robotInformation[myID].circle.x;
	dssy[1]=info.arsenal[1].circle.y - info.robotInformation[myID].circle.y;
	double  vammo[2];
	vammo[0] = atan2(dssy[0],dssx[0])*180/PI;//
	vammo[1] = atan2(dssy[1],dssx[1])*180/PI;//圆心
	
	//预判
	
	//x,y轴间距
	double dx,dy;
	dx = info.robotInformation[1-myID].circle.x - info.robotInformation[myID].circle.x;	
	dy = info.robotInformation[1-myID].circle.y - info.robotInformation[myID].circle.y;
	//夹角数据组
	double d = sqrt(pow(dx,2)+pow(dy,2));//坦克间距
	//面对角
	double vpo;
	vpo = atan2(dy,dx)*180/PI;

	//瞄准
	int fire=0;
	if(info.robotInformation[myID].weaponRotation > vpo-6){
		if(info.robotInformation[myID].weaponRotation - vpo > 180){ order.wturn = 1;}
		else{ order.wturn = -1;}
	}
	else if(info.robotInformation[myID].weaponRotation < vpo+6){ 
		if(vpo - info.robotInformation[myID].weaponRotation > 180){ order.wturn = -1;}
		else{ order.wturn = 1;}
	}
	if(abs(info.robotInformation[myID].weaponRotation - vpo) < 10){ fire=1;}

	//开火
	if(info.robotInformation[myID].remainingAmmo != 0){	
		if(fire == 1){
			if(
			(info.robotInformation[myID].weaponRotation > vspo[0]+vspi[0] 
			|| info.robotInformation[myID].weaponRotation < vspo[0]-vspi[0]) 
				&& 
			(info.robotInformation[myID].weaponRotation > vspo[1]+vspi[1]
			|| info.robotInformation[myID].weaponRotation < vspo[1]-vspi[1])
			){
				order.fire = 1;
			}
			else { order.fire = 0;}
		}
		
		//移动
		if(info.robotInformation[1-myID].weaponTypeName == WT_MissileLauncher){
			//猥琐流爱国者
			if(myID == 0 ){
			//左上角
				if(dd[1] < 300 ){ 					
					raoStone(order,info.robotInformation[myID].engineRotation,vspo[1]);
				}
				else { 
					gotoCircle(order,info.robotInformation[myID].circle,info.robotInformation[myID].engineRotation);	
				}			
			}
			else{
				//右下角
				if(dd[0] < 300 ){ 					
					raoStone(order,info.robotInformation[myID].engineRotation,vspo[0]);
				}
				else { 
					gotoCircle(order,info.robotInformation[myID].circle,info.robotInformation[myID].engineRotation);		
				}			
			}	 
			order.run = 1;
		}

		else if(info.robotInformation[1-myID].weaponTypeName == WT_Prism){
			//WT_Prism光棱 
			if(info.robotInformation[1-myID].hp < info.robotInformation[myID].hp + 40){
				gotoFight(order,info.robotInformation[myID].engineRotation,vpo);
			}
			else{
				gotoCircle(order,info.robotInformation[myID].circle,info.robotInformation[myID].engineRotation);
			}
			order.run = 1;
		}

		else if(info.robotInformation[1-myID].weaponTypeName == WT_RPG
			|| info.robotInformation[1-myID].weaponTypeName == WT_Machinegun
			|| info.robotInformation[1-myID].weaponTypeName == WT_Shotgun ){
			//WT_RPG 火箭筒  WT_Machinegun 旋转机枪
			if(info.robotInformation[1-myID].remainingAmmo > 3){
				int i=myID;
				if(myID == 1 && info.robotInformation[1-myID].circle.x>700) { i = 0;}
				if(myID == 0 && info.robotInformation[1-myID].circle.x<500) { i = 1;}
				if(i == 1){
					//左上角
					if(dd[1] < 300 ){ 					
						raoStone(order,info.robotInformation[myID].engineRotation,vspo[1]);
					}
					else{
						gotoFight(order,info.robotInformation[myID].engineRotation,vspo[1]);
					}
				}
				else{
					//右下角
					if(dd[0] < 300 ){ 					
						raoStone(order,info.robotInformation[myID].engineRotation,vspo[0]);				
					}
					else{
						gotoFight(order,info.robotInformation[myID].engineRotation,vspo[0]);
					}
				}
			}
			else{
				gotoFight(order,info.robotInformation[myID].engineRotation,vpo);
			}
			order.run = 1;
		}

		else if(info.robotInformation[1-myID].weaponTypeName == WT_Cannon
			|| info.robotInformation[1-myID].weaponTypeName == WT_Apollo){
			//WT_Cannon 加农炮  WT_Apollo阿波罗
			if(info.robotInformation[1-myID].remainingAmmo * 25 >= info.robotInformation[myID].hp){
				if(myID == 1 ){
					if(dd[1] > 200 ){ 					
						raoStone(order,info.robotInformation[myID].engineRotation,vspo[1]);
					}
					else { 
						gotoFight(order,info.robotInformation[myID].engineRotation,vspo[1]);		
					}			
				}
				else{
					if(dd[0] > 200 ){ 					
						raoStone(order,info.robotInformation[myID].engineRotation,vspo[0]);
					}
					else { 
						gotoFight(order,info.robotInformation[myID].engineRotation,vspo[0]);		
					}			
				}
			}
			else{
				gotoFight(order,info.robotInformation[myID].engineRotation,vpo);
			}
			order.run = 1;
		}

		else if( info.robotInformation[1-myID].weaponTypeName == WT_Tesla){
			//WT_Tesla磁暴线圈
			if(info.robotInformation[1-myID].hp < info.robotInformation[myID].hp){
				gotoFight(order,info.robotInformation[myID].engineRotation,vpo);
			}
			else{
				gotoCircle(order,info.robotInformation[myID].circle,info.robotInformation[myID].engineRotation);	
			}
			order.run = 1;
		}
		else if(info.robotInformation[1-myID].weaponTypeName == WT_ElectricSaw){
			//WT_ElectricSaw电锯  
			if(myID == 1){
			//左上角
				if(dd[1] < 300 ){ 					
					raoStone(order,info.robotInformation[myID].engineRotation,vspo[1]);
				}
				else { 
					gotoCircle(order,info.robotInformation[myID].circle,info.robotInformation[myID].engineRotation);		
				}			
			}
			else{
				//右下角
				if(dd[0] < 300 ){ 					
									
				}
				else { 
					gotoCircle(order,info.robotInformation[myID].circle,info.robotInformation[myID].engineRotation);		
				}			
			}	 
			if(d < 200){order.run = 1;}//绕行
			else if(d>300){ order.run = 1;}
		}
		else {
			//其他策略
			//WT_GrenadeThrower手雷 WT_PlasmaTorch 等离子弹弹 —— 躲墙消耗弹药
			//WT_MineLayer 布雷车
			if(info.robotInformation[myID].hp>35){//就是干
				gotoFight(order,info.robotInformation[myID].engineRotation,vpo);
			}
			else{//守弹药点
				double tempx,tempy;
				tempx=info.arsenal[1].circle.x - info.robotInformation[myID].circle.x;
				tempy=(info.arsenal[1].circle.y-120) - info.robotInformation[myID].circle.y;
				double  vpp;
				vpp = atan2(tempx,tempy)*180/PI;
				gotoFight(order,info.robotInformation[myID].engineRotation,vpp);
			}
			order.run = 1; 
		}	
	}
	else{	
			int j = 0;
			if(dd[0] <= dd[1]){ j=0;}
			else{ j=1;}

			if(info.arsenal[j].respawning_time <= 3){
				gotoFight(order,info.robotInformation[myID].engineRotation,vammo[j]);
			}
			else if(info.arsenal[1-j].respawning_time <= 3){
				gotoFight(order,info.robotInformation[myID].engineRotation,vammo[1-j]);	
			}

			else {
				gotoCircle(order,info.robotInformation[myID].circle,info.robotInformation[myID].engineRotation);		
			}
			order.run = 1;
	}

	if(d < 300){
		order.fire = 1;
	}
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

	weapon = WT_Tesla ;
	engine = ET_AFV;  
}











//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "蛋堡";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "啊冬啊冬";
}



//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return 138;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 43;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return 226;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 138;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 43;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 226;
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
	//躲子弹
}


void RobotAI::onHit(int launcherID,bullettypename btn)
{
	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
}



//TODO:这里可以实现你自己的函数
void RobotAI::gotoFight(RobotAI_Order& order,double er,double vpo)
{
	if(er > vpo){
		if(er - vpo > 180){ order.eturn = 1;}
		else{ order.eturn = -1;}
	}
	else if(er < vpo){ 
		if(vpo - er > 180){ order.eturn = -1;}
		else{ order.eturn = 1;}
	}
}
void RobotAI::gotoCircle(RobotAI_Order& order,const Circle& circle,double er){
		
		if(circle.x >= 1280){
				if(er <= 90 && er >= 0){
						order.eturn = 1;
				}
			}
			if(circle.y >= 600){
				if(er >= 90 && er <= 180){
						order.eturn = 1;
				}
			}
			if(circle.x <= 60){
				if(er <= -90 || er >= 0){
						order.eturn = 1;
				}
			}
			if(circle.y <= 60 ){
				if(er >= -90 && er <= 0){
						order.eturn = 1;
				}
			}	
}

void RobotAI::raoStone(RobotAI_Order& order,double er,double vspo){
	if(vspo <= 0 && er >= 0){
		if(er - vspo < 290){ order.eturn = 1;}		
	}
	if(vspo <= 0 && er <= 0){
		if(vspo - er > 70){ order.eturn = 1;}	
	}
	if(vspo >= 0 && er >= 0){
		if(vspo - er > 70){ order.eturn = 1;}	
	}
	if(vspo >= 0 && er <= 0){
		if(vspo - er > 70){ order.eturn = 1;}
	}	
}
