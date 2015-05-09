#pragma once
#include "RobotAI.h"
#include "Vector.h"
#include "Coordinatetrans.h"
#include "Firecontrol.h"
#include "Frame.h"
#include <iostream>



using namespace std;

//武器类
class weapon {
public:
	weapontypename wp;//武器名称
	bullettypename bt1;
	bullettypename bt2;
	int damage1;//武器伤害
	int damage2;
	double error;//武器误差
	int cooling;//武器冷却
	int max;//最大弹药
	int remain;//剩余弹药
	int speed;//武器速度
	int rospeed;//炮塔转速
	void setweapon(const weapontypename& wptypename){};

	//weapon(const weapontypename& wp, bullettypename bt1, const bullettypename&bt2){
	//	damage1 = get_bullet_damage(bt1);

	//}

};


class Inface{
public:
	//确定这是第几帧的信息.
	int framenumber;
	int myID;//地方下标
	int enID;//我方下标

	//武器参数;
	weapon mywp;
	weapon enwp;

	Circle Me;//我的位置
	Circle En;//地方位置

	vec MEv;//位置向量
	vec AC_MEv;//静态逆时针位置向量
	vec C_MEv;//静态顺时针位置向量


	vec MWv;//我武器向量
	vec AC_MWv;//逆时针我武器偏转
	vec C_MWv;//顺时针我武器偏转


	vec EWv;//敌武器向量
	vec AC_EWv;//逆时针敌武器偏转
	vec C_EWv;//顺时针敌武器偏转


	vec OBv[2];//障碍物向量1
	vec OBv_AC[2];//障碍物1的逆偏移向量
	vec OBv_C[2];//障碍物1的顺偏移向量



};
//输入我,敌坐标,返回 位置向量
vec MEvec(const double ix, const	double iy, const double ex, const double ey){
	vec temp(ex - ix, ey - iy);

	temp.Unitization();
	return temp;
}
vec MEvec(Circle mc, Circle ec){
	vec temp(ec.x - mc.x, ec.y - mc.y);
	return temp;
}


//输入敌我坐标圆,返回位置位逆偏向量:
vec AC_MEvec(const vec&MEv, Circle Me,Circle En){
	vec temp;
	double length = sqrt((En.x - Me.x)*(En.x - Me.x) + (En.y - Me.y)*(En.y - Me.y));
	temp.SetAngel(MEv.arg + asin(En.r / length )* 180 / PI,length);
	return temp;
}


//返回位顺偏向量
vec C_MEvec(const vec&MEv, Circle Me, Circle En){
	vec temp;
	double length = sqrt((En.x - Me.x)*(En.x - Me.x) + (En.y - Me.y)*(En.y - Me.y));
	temp.SetAngel(MEv.arg - asin(En.r / length) * 180 / PI,length);
	return temp;
}




//暂时erro为3



//输入 我武器角度,返回 我武器单位向量
vec MWvec(double angle){
	//cout << "我武器单位向量构造函数调用!" << endl;
	vec temp;
	temp.SetAngel(angle);
	//cout << "我武器单位向量:("<<temp.x<<","<<temp.y<<")"<<"角度:"<<temp.arg << endl;
	//cout << "我武器单位向量构造函数调用结束" << endl;
	return temp;
}


//-------------注意!!!!error有问题
//-------------
//-------------
//返回我武器逆偏向量,需要武器
vec AC_MWvec(const vec& MWv, weapon& wp){//目前并不确定他的偏移角度到底是多少
	vec temp;
	wp.error = 3;
	temp.SetAngel(MWv.arg + wp.error / 2);
	return temp;
}

//返回我武器顺偏变量,需要武器
vec C_MWvec(const vec& MWv, weapon& wp){//目前并不确定他的偏移角度到底是多少
	vec temp;
	wp.error = 3;
	temp.SetAngel(MWv.arg - wp.error / 2);
	return temp;
}










//返回障碍物向量//并不是单位向量
vec OBvec(Circle ob, Circle me){
	//
	//cout << "调用障碍物向量构造函数" << endl;
	vec temp(ob.x-me.x,ob.y-me.y);

	//cout << "障碍物向量:("<<temp.x<<","<<temp.y<<")"<<"角度:"<<temp.arg << endl;
	//cout << "障碍物向量构造函数调用结束" << endl;
	return temp;
}

//返回逆偏障碍物向量,单位向量
vec OB_ACvec(vec OBv, Circle ob){
	//cout << "调用逆偏障碍物向量" << endl;
	vec temp;
	double length = sqrt(OBv.length*OBv.length + ob.r*ob.r);
	temp.SetAngel(OBv.arg + asin(ob.r / OBv.length) * 180 / PI, length);
	//cout << "逆偏障碍物向量:(" << temp.x << "," << temp.y << ")" << "角度:" << temp.arg << endl;
	//cout << "逆偏障碍物向量构造函数调用结束" << endl;
	return temp;

}

//返回顺偏障碍物向量,单位向量
vec OB_Cvec(vec OBv, Circle ob){
	//cout << "调用顺偏障碍物向量" << endl;
	double length = sqrt(OBv.length*OBv.length + ob.r*ob.r);

	vec temp;
	temp.SetAngel(OBv.arg - asin(ob.r / OBv.length) * 180 / PI,length);
	//cout << "顺偏障碍物向量:(" << temp.x << "," << temp.y << ")" << "角度:" << temp.arg << endl;
	//cout << "顺偏障碍物向量构造函数调用结束" << endl;
	return temp;
}








//输入 敌武器角度,返回 敌武器单位向量
vec EWvec(double angle){
	vec temp;
	temp.SetAngel(angle);
	return temp;
}

//返回敌武器逆偏向量,需要武器
vec AC_EWvec(const vec& EWv, const weapon& wp){//目前并不确定他的偏移角度到底是多少
	vec temp;
	temp.SetAngel(EWv.arg + wp.error / 2);
	return temp;
}

//返回敌武器顺偏向量,需要武器
vec C_EWvec(const vec& EWv, const weapon& wp){//目前并不确定他的偏移角度到底是多少
	vec temp;
	temp.SetAngel(EWv.arg - wp.error / 2);
	return temp;
}
















//坐标变换与接口刷新:
void SetInface(const RobotAI_BattlefieldInformation&info,int myID,Inface&vset){
	//更新ID
	vset.myID = myID;
	vset.enID = 1 - myID;

	//更新双方武器
	vset.mywp.setweapon(info.robotInformation[vset.myID].weaponTypeName);
	vset.enwp.setweapon(info.robotInformation[vset.enID].weaponTypeName);


	//更新位置
	vset.Me = info.robotInformation[vset.myID].circle;
	vset.En = info.robotInformation[vset.enID].circle;


	//更新位置向量
	vset.MEv = MEvec(vset.Me, vset.En);

	//更新位逆向量
	vset.AC_MEv = AC_MEvec(vset.MEv, vset.Me,vset.En);
	vset.C_MEv = C_MEvec(vset.MEv, vset.Me,vset.En);



	//更新我武器向量;
	vset.MWv = MWvec(info.robotInformation[vset.myID].weaponRotation);
	//更新我武逆偏向量;
	vset.AC_MWv = AC_MWvec(vset.MWv, vset.mywp);
	//顺偏
	vset.C_MWv = C_MWvec(vset.MWv, vset.mywp);





	//更新敌武器向量:
	vset.EWv = EWvec(info.robotInformation[vset.enID].weaponRotation);

	//更新敌武逆偏
	vset.AC_EWv = AC_EWvec(vset.EWv, vset.enwp);
	//顺偏
	vset.C_EWv = AC_EWvec(vset.EWv, vset.enwp);


	//更新障碍物向量:
	//障碍物逆偏:
	vset.OBv[0] = OBvec(info.obstacle[0], vset.Me);
	vset.OBv_AC[0] = OB_ACvec(vset.OBv[0], info.obstacle[0]);
	vset.OBv_C[0] = OB_Cvec(vset.OBv[0], info.obstacle[0]);

	vset.OBv[1] = OBvec(info.obstacle[1], vset.Me);
	vset.OBv_AC[1] = OB_ACvec(vset.OBv[1], info.obstacle[1]);
	vset.OBv_C[1] = OB_Cvec(vset.OBv[1], info.obstacle[1]);


}


//坐标变换与接口更新//...调整.
void CoordinateAndInterface(const RobotAI_BattlefieldInformation&info, const int&myID, 
	RobotAI_BattlefieldInformation&RHinfo, Inface& vset)


{
	RHinfo = FieldInfoTrans(info);
	SetInface(RHinfo, myID, vset);
	return;
}