#ifndef MYFUNC_H
#define MYFUNC_H

#include "RobotAI.h"
#include <fstream>
#include <list>
using namespace std;

enum strategy
{
	AVOID,
	RUSH,

};

class MyFunction{
private:
	int TrackCount;
	ofstream ofs;
	list<Circle> EnemyTrack;
public:
	MyFunction(){
	}
	~MyFunction(){
		ofs.close();
	}

	void init(int trackCount,Circle initEnemy){
		ofs.open("D:/mylog.txt");
		TrackCount=trackCount;

		for(int i=0;i<TrackCount;++i){
			EnemyTrack.push_back(initEnemy);
		}
	}

	void setTrack(int TrackCount,Circle enemy){
		
	}

	int TargetAt(Circle self,Circle enemy,double rotation,double error){
	
		
		double reAngle=GetRelativeAngle(self,enemy);

		double rRotation=(double)rotation/180*PI;

		reAngle<0?reAngle+=(2*PI):reAngle;
		rRotation<0?rRotation+=(2*PI):rRotation;
		double minus=rRotation-reAngle;
		if(minus>PI){
			minus-=2*PI;
		}else if(minus<-PI){
			minus+=2*PI;
		}

		double rError=error/180*PI;

		if(minus>rError){
			return -1;
		}else if(minus<-rError){
			return 1;
		}else return 0;
	}

	double GetRelativeAngle(Circle self,Circle enemy){
		double dx=enemy.x-self.x;
		double dy=enemy.y-self.y;
		double dis=sqrt(dx*dx+dy*dy);

		double angle=asin(abs(dy)/dis);
		//ofs<<"angle:"<<(angle/PI*180)<<endl;

		if(dx>=0&&dy>=0){
		}else if(dx>=0&&dy<0){
			angle=-angle;
		}else if(dx<0&&dy>=0){
			angle=PI-angle;
		}else if(dx<0&&dy<0){
			angle=angle-PI;
		}
		return angle;
	}
	
	void updateTrack(Circle enemy){
		EnemyTrack.pop_front();
		EnemyTrack.push_back(enemy);
	}

	Circle GetProPointRPG(Circle self){
		double dx=(EnemyTrack.back().x-EnemyTrack.front().x)/(TrackCount-1);
		double dy=(EnemyTrack.back().y-EnemyTrack.front().y)/(TrackCount-1);
		Circle target;
		target.x=EnemyTrack.back().x;
		target.y=EnemyTrack.back().y;
		double bulletDis=0.0;
		double bulletV=8.0;
 		for(;;){
 			target.x+=dx;
			target.y+=dy;
			bulletDis+=bulletV;
			bulletV*=1.05;
			double dis=sqrt((self.x-target.x)*(self.x-target.x)+(self.y-target.y)*(self.y-target.y));
			if(dis>bulletDis)continue;
			else break;
 		}
		return target;
	}

	Circle GetProPointCannon(Circle self){
		double dx=EnemyTrack.back().x-EnemyTrack.front().x;
		double dy=EnemyTrack.back().y-EnemyTrack.front().y;
		Circle target;
		target.x=EnemyTrack.back().x;
		target.y=EnemyTrack.back().y;
		double bulletDis=0.0;
		double bulletV=11.0;
		int control=100;
		for(int i=0;i<control;++i){
			target.x+=dx;
			target.y+=dy;
			bulletDis+=bulletV;
			double dis=sqrt((self.x-target.x)*(self.x-target.x)+(self.y-target.y)*(self.y-target.y));
			if(dis>bulletDis)continue;
			else break;
		}
		return target;
	}

	Circle GetTargetAddr(Circle self,Circle enemy,const Circle obstacle[],double ads){
		int oid=0;
		Circle obTarget=GetNearestCircle(self,obstacle);

		double dx=enemy.x-obTarget.x;
		double dy=enemy.y-obTarget.y;
		ofs<<"dx:"<<dx<<endl;
		ofs<<"dy:"<<dy<<endl;
		double reAngle=abs(GetRelativeAngle(obTarget,enemy));
		ofs<<"angle:"<<reAngle<<endl;
		reAngle>(PI/2)?reAngle=PI-reAngle:reAngle;
		double adsX=(ads+obstacle[0].r)*cos(reAngle);
		double adsY=(ads+obstacle[0].r)*sin(reAngle);
		ofs<<"ads.x:"<<adsX<<endl;
		ofs<<"ads.y:"<<adsY<<endl;
		Circle target;
		dx>0?target.x=obTarget.x-adsX:target.x=obTarget.x+adsX;
		dy>0?target.y=obTarget.y-adsY:target.y=obTarget.y+adsY;
		ofs<<"enemy.x:"<<enemy.x<<endl;
		ofs<<"enemy.y:"<<enemy.y<<endl;
		ofs<<"target.x:"<<target.x<<endl;
		ofs<<"target.y:"<<target.y<<endl<<endl;

		return target;
	}

	bool canShoot(Circle self,Circle enemy,const Circle obstacle[]){
		double dx=self.x-enemy.x;
		double dy=self.y-enemy.y;
		double A;
		double B;
		double C;
		bool res=false;
		if(dx==0){
			B=0;
			A=1;
			C=-self.x;
			double LToS0=abs(obstacle[0].x+C);
			double LToS1=abs(obstacle[1].x+C);
			if(LToS0>obstacle[0].r&&LToS1>obstacle[1].r){
				return true;
			}else{
				double top=self.y>enemy.y?self.y:enemy.y;
				double bottom=self.y<enemy.y?self.y:enemy.y;
				if(LToS0>obstacle[0].r&&LToS1<=obstacle[1].r){
					if(obstacle[1].y>=bottom&&obstacle[1].y<=top){
						return false;
					}else return true;
				}else if(LToS0<=obstacle[0].r&&LToS1>obstacle[1].r){
					if(obstacle[0].y>=bottom&&obstacle[0].y<=top){
						return false;
					}else return true;
				}else{
					if(obstacle[1].y>=bottom&&obstacle[1].y<=top){
						return false;
					}else if(obstacle[0].y>=bottom&&obstacle[0].y<=top){
						return false;
					}else return true;
				}
			}
		}if(dy==0){
			A=0;
			B=1;
			C=-self.y;
			double LToS0=abs(obstacle[0].y+C);
			double LToS1=abs(obstacle[1].y+C);
			if(LToS0>obstacle[0].r&&LToS1>obstacle[1].r){
				return true;
			}else{
				double right=self.x>enemy.x?self.x:enemy.x;
				double left=self.x<enemy.x?self.x:enemy.x;
				if(LToS0>obstacle[0].r&&LToS1<=obstacle[1].r){
					if(obstacle[1].x>=left&&obstacle[1].x<=right){
						return false;
					}else return true;
				}else if(LToS0<=obstacle[0].r&&LToS1>obstacle[1].r){
					if(obstacle[0].x>=left&&obstacle[0].x<=right){
						return false;
					}else return true;
				}else{
					if(obstacle[1].x>=left&&obstacle[1].x<=right){
						return false;
					}else if(obstacle[0].x>=left&&obstacle[0].x<=right){
						return false;
					}else return true;
				}
			}
		}else{
			B=1.0;
			A=-dy/dx;
			C=-(A*self.x+B*self.y);
			double LToS0=abs(A*obstacle[0].x+B*obstacle[0].y+C)/sqrt(A*A+B*B);
			double LToS1=abs(A*obstacle[1].x+B*obstacle[1].y+C)/sqrt(A*A+B*B);

			if(LToS0>obstacle[0].r&&LToS1>obstacle[1].r){
				return true;
			}else{
				double LC0=B*obstacle[0].x-A*obstacle[0].y;
				double vy0=-(B*C+A*LC0)/(A*A+B*B);
				double vx0=(B*LC0-A*C)/(A*A+B*B);

				double LC1=B*obstacle[1].x-A*obstacle[1].y;
				double vy1=-(B*C+A*LC1)/(A*A+B*B);
				double vx1=(B*LC1-A*C)/(A*A+B*B);

				double right=self.x>enemy.x?self.x:enemy.x;
				double left=self.x<enemy.x?self.x:enemy.x;
				double top=self.y>enemy.y?self.y:enemy.y;
				double bottom=self.y<enemy.y?self.y:enemy.y;

				if(LToS0>obstacle[0].r&&LToS1<=obstacle[1].r){
					if(vx1>=left&&vx1<=right&&vy1>=bottom&&vy1<=top){
						return false;
					}else return true;
				}else if(LToS0<=obstacle[0].r&&LToS1>obstacle[1].r){
					if(vx0>=left&&vx0<=right&&vy0>=bottom&&vy0<=top){
						return false;
					}else return true;
				}else if(LToS0<=obstacle[0].r&&LToS1<=obstacle[1].r){
					if(vx1>=left&&vx1<=right&&vy1>=bottom&&vy1<=top){
						return false;
					}if(vx0>=left&&vx0<=right&&vy0>=bottom&&vy0<=top){
						return false;
					}else return true;
				}
			}
		}
	}

	Circle GetNearestCircle(Circle self,const Circle circle[]){
		double dx0=self.x-circle[0].x;
		double dy0=self.y-circle[0].y;
		double dis0=dx0*dx0+dy0*dy0;

		double dx1=self.x-circle[1].x;
		double dy1=self.y-circle[1].y;
		double dis1=dx1*dx1+dy1*dy1;

		if(dis0>dis1){
			return circle[1];
		}else return circle[0];
	}

	int GetSafeArsenal(Circle self,Circle enemy,const RobotAI_ArsenalInformation* arsenal,double controlDis){
		double myDisToAr0=controlDis+sqrt((self.x-arsenal[0].circle.x)*(self.x-arsenal[0].circle.x)+(self.y-arsenal[0].circle.y)*(self.y-arsenal[0].circle.y));
		double myDisToAr1=controlDis+sqrt((self.x-arsenal[1].circle.x)*(self.x-arsenal[1].circle.x)+(self.y-arsenal[1].circle.y)*(self.y-arsenal[1].circle.y));
		double EnDisToAr0=sqrt((enemy.x-arsenal[0].circle.x)*(enemy.x-arsenal[0].circle.x)+(enemy.y-arsenal[0].circle.y)*(enemy.y-arsenal[0].circle.y));
		double EnDisToAr1=sqrt((enemy.x-arsenal[1].circle.x)*(enemy.x-arsenal[1].circle.x)+(enemy.y-arsenal[1].circle.y)*(enemy.y-arsenal[1].circle.y));

		if(myDisToAr0<=EnDisToAr0&&myDisToAr1<=EnDisToAr1){
			if(arsenal[0].respawning_time==0&&arsenal[1].respawning_time==0){
				return myDisToAr0>myDisToAr1?1:0;
			}else if(arsenal[0].respawning_time==0&&arsenal[1].respawning_time!=0){
				return 0;
			}else if(arsenal[0].respawning_time!=0&&arsenal[1].respawning_time==0){
				return 1;
			}else return -1;
		}else if(myDisToAr0>EnDisToAr0&&myDisToAr1>EnDisToAr1){
			if(arsenal[0].respawning_time==0&&arsenal[1].respawning_time==0){
				return myDisToAr0>myDisToAr1?1:0;
			}else return -1;
		}else if(myDisToAr0<=EnDisToAr0&&myDisToAr1>EnDisToAr1){
			if(arsenal[0].respawning_time==0){
				return 0;
			}else if(arsenal[0].respawning_time!=0){
				return -1;
			}	
		}else if(myDisToAr0>EnDisToAr0&&myDisToAr1<=EnDisToAr1){
			if(arsenal[1].respawning_time==0){
				return 1;
			}else if(arsenal[1].respawning_time!=0){
				return -1;
			}
		}
	}

	Circle GetRushAddr(Circle self,Circle enemy,double controlDis){
		double angle=abs(GetRelativeAngle(self,enemy));
		angle>(PI/2)?angle=PI-angle:angle;
		double dx=controlDis*cos(angle);
		double dy=controlDis*sin(angle);

		Circle target;
		dx>0?target.x=enemy.x+dx:target.x=enemy.x-dx;
		dy>0?target.y=enemy.y+dy:target.y=enemy.y-dy;
		return target;
	}

	double GetDis(Circle self,Circle enemy){
		return sqrt((self.x-enemy.x)*(self.x-enemy.x)+(self.y-enemy.y)*(self.y-enemy.y));
	}

	strategy Strategy(const RobotAI_RobotInformation& self,const RobotAI_RobotInformation& enemy){
		strategy res;
		int damage=0;

		if(GetDis(self.circle,enemy.circle)<=400){
			res=RUSH;
		}else{
			switch (enemy.weaponTypeName)
			{
			case WT_GrenadeThrower:
				damage=25;
				break;
			case WT_Machinegun:
				damage=7;
				break;
			case WT_MineLayer:
				damage=45;
				break;
			case WT_Prism:
				damage=20;
				break;
			case WT_PlasmaTorch:
				damage=18;
				break;
			case WT_MissileLauncher:
				damage=15;
				res=RUSH;
				break;
			case WT_RPG:
				damage=35;
				break;
			case WT_Apollo:
				damage=25;
				break;
			case WT_Cannon:
				damage=25;
				break;
			case WT_ElectricSaw:
				damage=5;
				res=RUSH;
				break;
			case WT_Shotgun:
				damage=50;
				break;
			default:
				break;
			}
			int RemainDamage=damage*enemy.remainingAmmo;
			RemainDamage>=self.hp?res=AVOID:res=RUSH;
		}	
		return res;
	}

	void Log(double s){
		ofs<<s<<endl;
	}

	void Log(string s){
		ofs<<s<<endl;
	}

	void Log(const RobotAI_BattlefieldInformation& info){
		ofs<<&info<<endl;
	}


};

#endif