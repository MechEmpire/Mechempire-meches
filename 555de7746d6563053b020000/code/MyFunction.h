#ifndef MYFUNC_H
#define MYFUNC_H

#include "RobotAI.h"
#include <fstream>
#include <list>
using namespace std;

class MyFunction{
private:
	ofstream ofs;
	list<Circle> EnemyTrack;
public:
	MyFunction(){
	}
	~MyFunction(){
		ofs.close();
	}

	void init(Circle initEnemy){
		ofs.open("D:/mylog.txt");
		EnemyTrack.push_back(initEnemy);
		EnemyTrack.push_back(initEnemy);
	}

	int TargetAt(Circle self,Circle enemy,double rotation,double error){
	
		ofs<<"start:"<<endl;
		
		double reAngle=GetRelativeAngle(self,enemy);
		ofs<<"reAngle:"<<reAngle<<" ";
		ofs<<"Angle:"<<reAngle/PI*180<<endl;

		double rRotation=(double)rotation/180*PI;
		ofs<<"rotation:"<<rRotation<<" ";
		ofs<<"rotation:"<<rRotation/PI*180<<endl;

		reAngle<0?reAngle+=(2*PI):reAngle;
		rRotation<0?rRotation+=(2*PI):rRotation;
		double minus=rRotation-reAngle;
		ofs<<"minus1:"<<minus<<endl;
		if(minus>PI){
			minus-=2*PI;
		}else if(minus<-PI){
			minus+=2*PI;
		}
		ofs<<"minus2:"<<minus<<endl;

		double rError=error/180*PI;
		ofs<<"rError:"<<rError<<endl<<endl;

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
			angle=PI+angle;
		}
		return angle;
	}
	
	void updateTrack(Circle enemy){
		EnemyTrack.pop_front();
		EnemyTrack.push_back(enemy);
	}

	Circle GetProPoint(){
		double dx=EnemyTrack.front().x-EnemyTrack.back().x;
		double dy=EnemyTrack.front().y-EnemyTrack.back().y;
		Circle target;
		int frame=0;
// 		for(;;){
// 			
// 		}
		return target;
	}

	Circle GetTargetAddr(Circle self,Circle enemy,const Circle obstacle[],double ads){
		int oid=0;
		Circle obTarget=GetNearestCircle(self,obstacle);

		double dx=enemy.x-obTarget.x;
		double dy=enemy.y-obTarget.y;
		double reAngle=abs(GetRelativeAngle(obTarget,enemy));
		double adsX=ads*cos(reAngle);
		double adsY=ads*sin(reAngle);
		Circle target;
		dx>0?target.x=obTarget.x-obTarget.r-adsX:target.x=obTarget.x+obTarget.r+adsX;
		dy>0?target.y=obTarget.y-obTarget.r-adsY:target.y=obTarget.y+obTarget.r+adsY;

		return target;
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

	Circle GetSafeArsenal(Circle self,Circle enemy,Circle arsenal[]){
		double myDisToAr0=(self.x-arsenal[0].x)*(self.x-arsenal[0].x)+(self.y-arsenal[0].y)*(self.y-arsenal[0].y);
		double myDisToAr1=(self.x-arsenal[1].x)*(self.x-arsenal[1].x)+(self.y-arsenal[1].y)*(self.y-arsenal[1].y);
		double EnDisToAr0=(enemy.x-arsenal[0].x)*(enemy.x-arsenal[0].x)+(enemy.y-arsenal[0].y)*(enemy.y-arsenal[0].y);
		double EnDisToAr1=(enemy.x-arsenal[1].x)*(enemy.x-arsenal[1].x)+(enemy.y-arsenal[1].y)*(enemy.y-arsenal[1].y);

		if(myDisToAr0<=EnDisToAr0&&myDisToAr1<=EnDisToAr1){
			return myDisToAr0>myDisToAr1?arsenal[1]:arsenal[0];
		}else if(myDisToAr0>EnDisToAr0&&myDisToAr1>EnDisToAr1){
			return myDisToAr0>myDisToAr1?arsenal[1]:arsenal[0];
		}else if(myDisToAr0<=EnDisToAr0&&myDisToAr1>EnDisToAr1){
			return arsenal[0];
		}else return arsenal[1];
	}

	void Log(double s){
		ofs<<s<<endl;
	}

	void Log(string s){
		ofs<<s<<endl;
	}
};

#endif