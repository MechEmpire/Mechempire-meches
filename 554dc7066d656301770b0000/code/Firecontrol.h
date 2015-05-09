#pragma once
#include "RobotAI.h"
#include "Vector.h"
#include "Interface.h"
#include <iostream>
using namespace std;






//------瞄准初步










//-------------------------武器调整函数;//调整过后...安全
void AdjustWepon(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info,Inface&vset){
	//cout << "调用武器调整函数" << endl;
	//判断向量夹角:
	double C_MW_ME, AC_MW_ME;
	AC_MW_ME = vset.MWv.AntiClockwiseAngle(vset.MEv);
	C_MW_ME = vset.MWv.ClockwiseAngle(vset.MEv);
	//cout << "输出此时的武器角度:" << info.robotInformation[vset.myID].weaponRotation << endl;
	//cout << "输出此时武器向量  :" << vset.MWv << endl;
	//cout << "上一帧武器向量与位置向量的逆时针夹角:" << AC_MW_ME<<endl;
	//cout << "上一帧武器向量与位置向量的顺时针夹角:" << C_MW_ME << endl;

	if (C_MW_ME < AC_MW_ME){
		order.wturn = 1;//写成1是顺时针旋转...
		//cout << "武器顺时针旋转" << endl;
	}
	else
	{
		order.wturn = -1;
		//cout << "武器逆时针旋转" << endl;
	}
	//cout << "武器调整函数调用结束" << endl;
}





//-------------------------静态命中率计算函数初步-------------------//对象光棱磁暴,完成
double HitRate(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, Inface&vset){
	//cout << endl;
	//cout << endl;
	//cout << "以下是调用命中率函数时的输出" << endl;
	//定义命中率:
	double hitrate;
	//因为位置夹角小角一定能这么算...为了方便认清便这么算:
	//定义位偏夹角,武器偏移夹角:

	double argME, argMW;
	argME = vset.AC_MEv.ClockwiseAngle(vset.C_MEv);
	argMW = vset.AC_MWv.ClockwiseAngle(vset.C_MWv);
	//cout << "逆偏位置向量:" << vset.AC_MEv;
	//cout << "顺偏位置向量:" << vset.C_MEv;


	//cout << "位置夹角:" << argME << endl;
	//cout << "武器误差:" << argMW << endl;
	//得到逆偏武器向量和顺偏位置向量之间的顺角 与逆偏位间逆角
	double AC_ACMWv_ACMEv;
	double C_ACMWv_CMEv;
	
	
	AC_ACMWv_ACMEv = vset.AC_MWv.AntiClockwiseAngle(vset.AC_MEv);
	C_ACMWv_CMEv = vset.AC_MWv.ClockwiseAngle(vset.C_MEv);
	//cout << endl;
	//cout << "武器逆偏向量:" << vset.AC_MWv << endl;
	//cout << "逆偏武器与逆位夹角:" << AC_ACMWv_ACMEv << endl;
	//cout << "逆偏武器与顺位夹角:" << C_ACMWv_CMEv << endl;

	//逆偏武器向量是否在位置逆顺向量之间


	//如果前面的两个角和是argme,那就说明在其中:
	//定义是否在其中的布尔值:
	bool wpac_bool;
	if (C_ACMWv_CMEv + AC_ACMWv_ACMEv == argME){
		wpac_bool = true;
		//cout << "逆 锁定" << endl;
	}
	else{
		wpac_bool = false;
		//cout << "逆 未锁定" << endl;
	}


	//接下来判断 顺偏武器向量:
	double AC_CMWv_ACMEv;
	double C_CMWv_CMEv;
	

	AC_CMWv_ACMEv = vset.C_MWv.AntiClockwiseAngle(vset.AC_MEv);
	C_CMWv_CMEv = vset.C_MWv.ClockwiseAngle(vset.C_MEv);
	
	//cout << endl;
	//cout << "武器顺偏向量:" << vset.C_MWv << endl;
	//cout << "顺偏武器与逆位夹角:" << AC_CMWv_ACMEv << endl;
	//cout << "顺偏武器与顺位夹角:" << C_CMWv_CMEv << endl;

	bool wpc_bool;
	if (C_CMWv_CMEv + AC_CMWv_ACMEv == argME){
		wpc_bool = true;
		//cout << "顺 锁定" << endl;
	}
	else {
		wpc_bool = false;
		//cout << "顺 未锁定" << endl;
	}

	//如果两个全真
	if (wpac_bool&&wpc_bool == 1){
		hitrate = 1;
		//cout << "完全锁定" << endl;
	}
	else if (wpac_bool == 1 && wpc_bool == 0){
		hitrate = C_ACMWv_CMEv / vset.mywp.error;
		//cout << "逆半锁定" << endl;
	}
	else if (wpac_bool == 0 && wpc_bool == 1){
		hitrate = AC_CMWv_ACMEv / vset.mywp.error;
		//cout << "顺半锁定" << endl;
	}

	else{
		hitrate = 0;
		//cout << "未锁定" << endl;
	}
	cout << "命中率为:" << hitrate << endl;

	//cout << "命中率函数调用结束" << endl;
	//cout << endl;
	return hitrate;
}


//以前的阻挡判断
////--------------------------判断障碍物阻挡函数初步------------------//变更武器偏移向量,来计算命中率.
////这里还需要判断是否调用障碍物阻挡函数
//void ObstacleCheck(Inface&vset,int i)
//{
//	cout << "调用障碍物阻挡判断函数" << endl;
//
//	//判断障碍物向量1和武器偏移向量组的关系
//	//判断障碍物向量的夹角和武器误差向量的大小,
//	//假如障碍物向量夹角大于武器误差
//	//判断是否包含
//	//case
//	double argob1;//障碍物向量夹角
//	argob1 = vset.OB1v_AC.ClockwiseAngle(vset.OB1v_C);
//	double argob2;
//	argob2 = vset.OB2v_AC.ClockwiseAngle(vset.OB2v_C);
//	double argwp = vset.mywp.error;
//	cout << "障碍物向量夹角1=" << argob1 << endl;
//	cout << "障碍物向量夹角2=" << argob2 << endl;
//	cout << "武器夹角:" << argwp << endl;
//
//
//	//erro直接就是武器夹角
//
//	//判断大小ob1
//
//	if (argob1 >= argwp)
//	{
//		cout << endl;
//		cout << "障碍物1的夹角大于武器误差" << endl;
//		//判断武器向量是否在里面:
//		double AC_ACMWv_ACOB1v;
//		double C_ACMWv_COB1v;
//		AC_ACMWv_ACOB1v = vset.AC_MWv.AntiClockwiseAngle(vset.OB1v_AC);
//		C_ACMWv_COB1v = vset.AC_MWv.ClockwiseAngle(vset.OB1v_C);
//
//		cout << endl;
//		cout << "逆偏武器与障碍1逆向量的逆角:" << AC_ACMWv_ACOB1v << endl;
//		cout << "逆偏武器与障碍1顺向量的顺角:" << C_ACMWv_COB1v << endl;
//
//		double wpacbool;//定义是否在的布尔值
//
//		if (AC_ACMWv_ACOB1v + C_ACMWv_COB1v == argob1){
//			wpacbool = 1;
//			cout << "障碍物1阻挡武器逆偏向量" << endl;
//		}
//		else {
//			wpacbool = 0;
//			cout << "障碍物1未--阻挡武器逆偏向量" << endl;
//		}
//
//		//下面判断顺偏向量
//		double AC_CMWv_ACOB1v;
//		double C_CMWv_COB1v;
//		AC_CMWv_ACOB1v = vset.C_MWv.AntiClockwiseAngle(vset.OB1v_AC);
//		C_CMWv_COB1v = vset.C_MWv.ClockwiseAngle(vset.OB1v_C);
//
//		cout << endl;
//		cout << "顺偏武器与障碍1逆向量的逆角:" << AC_CMWv_ACOB1v << endl;
//		cout << "顺偏武器与障碍1顺向量的顺角:" << C_CMWv_COB1v << endl;
//
//		double wpcbool;//定义是否在的布尔值
//
//		if (AC_CMWv_ACOB1v + C_CMWv_COB1v == argob1){
//			wpcbool = 1;
//			cout << "障碍物1阻挡武器顺偏向量" << endl;
//		}
//		else {
//			wpacbool = 0;
//			cout << "障碍物1未--阻挡武器顺偏向量" << endl;
//		}
//
//		//根据布尔值判断
//		if (wpacbool == 1 && wpcbool == 1){
//			cout << "障碍物1完全阻挡武器!" << endl;
//			vset.AC_MWv.set(0, 0);
//			vset.C_MWv.set(0, 0);
//			cout << "完全阻挡时武器逆向量:" << vset.AC_MWv << endl;
//			cout << "完全阻挡时武器顺向量:" << vset.C_MWv << endl;
//		}
//		else if (wpacbool == 1 && wpcbool == 0){
//			cout << "障碍物1阻挡武器逆偏向量" << endl;
//			vset.AC_MWv = vset.OB1v_C;
//			cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
//			cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
//		}
//		else if (wpacbool == 0 && wpcbool == 1){
//			cout << "障碍物1阻挡武器顺偏向量" << endl;
//			vset.C_MWv = vset.OB1v_AC;
//			cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
//			cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
//		}
//		else if (wpacbool == 0 && wpcbool == 0){
//			cout << "障碍物1完全未阻挡武器向量" << endl;
//			cout << "未阻挡时武器逆向量:" << vset.AC_MWv << endl;
//			cout << "未阻挡时武器顺向量:" << vset.C_MWv << endl;
//		}
//	}
//
//
//		//-----------------
//	else //武器偏移向量大于障碍物向量
//	{
//			cout << endl;
//			cout << "障碍物1的夹角小于武器误差" << endl;
//			double AC_ACOB1v_ACMWv;
//			double C_ACOB1v_CMWv;
//			AC_ACOB1v_ACMWv = vset.OB1v_AC.AntiClockwiseAngle(vset.AC_MWv);
//			C_ACOB1v_CMWv = vset.OB1v_AC.ClockwiseAngle(vset.C_MWv);
//
//			cout << endl;
//			cout << "障碍1逆向量与逆偏武器的逆角:" << AC_ACOB1v_ACMWv << endl;
//			cout << "障碍1逆向量与顺偏武器的顺角:" << C_ACOB1v_CMWv << endl;
//
//			double obacbool;//定义是否在的布尔值
//
//			if (AC_ACOB1v_ACMWv + C_ACOB1v_CMWv == argwp)
//			{
//				obacbool = 1;
//				cout << "障碍物1阻挡武器顺偏向量" << endl;
//			}
//			else 
//			{
//				obacbool = 0;
//				cout << "障碍物1未--阻挡武器顺偏向量" << endl;
//			}
//
//
//			//下面判断顺偏向量
//			double AC_COB1v_ACMWv;
//			double C_COB1v_CMWv;
//			AC_COB1v_ACMWv = vset.OB1v_C.AntiClockwiseAngle(vset.AC_MWv);
//			C_COB1v_CMWv = vset.OB1v_C.ClockwiseAngle(vset.C_MWv);
//
//			cout << endl;
//			cout << "障碍1逆向量与顺偏武器的逆角:" << AC_COB1v_ACMWv << endl;
//			cout << "障碍1逆向量与顺偏武器的顺角:" << C_COB1v_CMWv << endl;
//
//			double obcbool;//定义是否在的布尔值
//
//			if (AC_COB1v_ACMWv + C_COB1v_CMWv == argwp){
//				obcbool = 1;
//				cout << "障碍物1阻挡武器逆偏向量" << endl;
//			}
//			else {
//				obcbool = 0;
//				cout << "障碍物1未--阻挡武器逆偏向量" << endl;
//			}
//
//
//
//			//根据布尔值判断
//			if (obacbool == 1 && obcbool == 1){
//				cout << "障碍物在武器之中!" << endl;
//				cout << "呃目前还没想好怎么处理" << endl;
//			}
//			else if (obacbool == 1 && obcbool == 0){
//				cout << "障碍物1阻挡武器顺偏向量" << endl;
//				vset.C_MWv = vset.OB1v_AC;
//				cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
//				cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
//			}
//			else if (obacbool == 0 && obcbool == 1){
//				cout << "障碍物1阻挡武器逆偏向量" << endl;
//				vset.AC_MWv = vset.OB1v_C;
//				cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
//				cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
//			}
//			else if (obacbool == 0 && obcbool == 0){
//				cout << "障碍物1完全未阻挡武器向量" << endl;
//				cout << "未阻挡时武器逆向量:" << vset.AC_MWv << endl;
//				cout << "未阻挡时武器顺向量:" << vset.C_MWv << endl;
//			}
//	}
//	
//
//
//
//
//
//	////判断第二个障碍物
//
//	////判断大小ob2
//
//	//if (argob2 >= argwp)
//	//{
//	//	cout << endl;
//	//	cout << "障碍物2的夹角大于武器误差" << endl;
//	//	//判断武器向量是否在里面:
//	//	double AC_ACMWv_ACOB2v;
//	//	double C_ACMWv_COB2v;
//	//	AC_ACMWv_ACOB2v = vset.AC_MWv.AntiClockwiseAngle(vset.OB2v_AC);
//	//	C_ACMWv_COB2v = vset.AC_MWv.ClockwiseAngle(vset.OB2v_C);
//
//	//	cout << endl;
//	//	cout << "逆偏武器与障碍2逆向量的逆角:" << AC_ACMWv_ACOB2v << endl;
//	//	cout << "逆偏武器与障碍2顺向量的顺角:" << C_ACMWv_COB2v << endl;
//
//	//	double wpacbool;//定义是否在的布尔值
//
//	//	if (AC_ACMWv_ACOB2v + C_ACMWv_COB2v == argob2){
//	//		wpacbool = 1;
//	//		cout << "障碍物2阻挡武器逆偏向量" << endl;
//	//	}
//	//	else {
//	//		wpacbool = 0;
//	//		cout << "障碍物2未--阻挡武器逆偏向量" << endl;
//	//	}
//
//
//
//	//	//下面判断顺偏向量
//	//	double AC_CMWv_ACOB2v;
//	//	double C_CMWv_COB2v;
//	//	AC_CMWv_ACOB2v = vset.C_MWv.AntiClockwiseAngle(vset.OB2v_AC);
//	//	C_CMWv_COB2v = vset.C_MWv.ClockwiseAngle(vset.OB2v_C);
//
//	//	cout << endl;
//	//	cout << "顺偏武器与障碍2逆向量的逆角:" << AC_CMWv_ACOB2v << endl;
//	//	cout << "顺偏武器与障碍2顺向量的顺角:" << C_CMWv_COB2v << endl;
//
//	//	double wpcbool;//定义是否在的布尔值
//
//	//	if (AC_CMWv_ACOB2v + C_CMWv_COB2v == argob2){
//	//		wpcbool = 1;
//	//		cout << "障碍物2阻挡武器顺偏向量" << endl;
//	//	}
//	//	else {
//	//		wpacbool = 0;
//	//		cout << "障碍物2未--阻挡武器顺偏向量" << endl;
//	//	}
//
//
//
//	//	//根据布尔值判断
//	//	if (wpacbool == 1 && wpcbool == 1){
//	//		cout << "障碍物2完全阻挡武器!" << endl;
//	//		vset.AC_MWv.set(0,0);
//	//		vset.C_MWv.set(0,0);
//	//		cout << "完全阻挡时武器逆向量:" << vset.AC_MWv << endl;
//	//		cout << "完全阻挡时武器顺向量:" << vset.C_MWv << endl;
//	//	}
//	//	else if (wpacbool == 1 && wpcbool == 0){
//	//		cout << "障碍物2阻挡武器逆偏向量" << endl;
//	//		vset.AC_MWv = vset.OB2v_C;
//	//		cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
//	//		cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
//	//	}
//	//	else if (wpacbool == 0 && wpcbool == 1){
//	//		cout << "障碍物2阻挡武器顺偏向量" << endl;
//	//		vset.C_MWv = vset.OB2v_AC;
//	//		cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
//	//		cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
//	//	}
//	//	else if (wpacbool == 0 && wpcbool == 0){
//	//		cout << "障碍物2完全未阻挡武器向量" << endl;
//	//		cout << "未阻挡时武器逆向量:" << vset.AC_MWv << endl;
//	//		cout << "未阻挡时武器顺向量:" << vset.C_MWv << endl;
//	//	}
//	//}
//
//
//
//	//	//-----------------
//	//	else //武器偏移向量大于障碍物向量
//	//	{
//	//		cout << endl;
//	//		cout << "障碍物2的夹角小于武器误差" << endl;
//	//		double AC_ACOB2v_ACMWv;
//	//		double C_ACOB2v_CMWv;
//	//		AC_ACOB2v_ACMWv = vset.OB2v_AC.AntiClockwiseAngle(vset.AC_MWv);
//	//		C_ACOB2v_CMWv = vset.OB2v_AC.ClockwiseAngle(vset.C_MWv);
//
//	//		cout << endl;
//	//		cout << "障碍2逆向量与逆偏武器的逆角:" << AC_ACOB2v_ACMWv << endl;
//	//		cout << "障碍2逆向量与顺偏武器的顺角:" << C_ACOB2v_CMWv << endl;
//
//	//		double obacbool;//定义是否在的布尔值
//
//	//		if (AC_ACOB2v_ACMWv + C_ACOB2v_CMWv == argwp){
//	//			obacbool = 1;
//	//			cout << "障碍物2阻挡武器顺偏向量" << endl;
//	//		}
//	//		else {
//	//			obacbool = 0;
//	//			cout << "障碍物2未--阻挡武器顺偏向量" << endl;
//	//		}
//
//
//	//		//下面判断顺偏向量
//	//		double AC_COB2v_ACMWv;
//	//		double C_COB2v_CMWv;
//	//		AC_COB2v_ACMWv = vset.OB2v_C.AntiClockwiseAngle(vset.AC_MWv);
//	//		C_COB2v_CMWv = vset.OB2v_C.ClockwiseAngle(vset.C_MWv);
//
//	//		cout << endl;
//	//		cout << "障碍2逆向量与顺偏武器的逆角:" << AC_COB2v_ACMWv << endl;
//	//		cout << "障碍2逆向量与顺偏武器的顺角:" << C_COB2v_CMWv << endl;
//
//	//		double obcbool;//定义是否在的布尔值
//
//	//		if (AC_COB2v_ACMWv + C_COB2v_CMWv == argwp)
//	//		{
//	//			obcbool = 1;
//	//			cout << "障碍物2阻挡武器逆偏向量" << endl;
//	//		}
//	//		else {
//	//			obcbool = 0;
//	//			cout << "障碍物2未--阻挡武器逆偏向量" << endl;
//	//		}
//
//
//
//	//		//根据布尔值判断
//	//		if (obacbool == 1 && obcbool == 1){
//	//			cout << "障碍物在武器之中!" << endl;
//	//			cout << "呃目前还没想好怎么处理" << endl;
//	//		}
//	//		else if (obacbool == 1 && obcbool == 0){
//	//			cout << "障碍物2阻挡武器顺偏向量" << endl;
//	//			vset.C_MWv = vset.OB2v_AC;
//	//			cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
//	//			cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
//	//		}
//	//		else if (obacbool == 0 && obcbool == 1){
//	//			cout << "障碍物2阻挡武器逆偏向量" << endl;
//	//			vset.AC_MWv = vset.OB2v_C;
//	//			cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
//	//			cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
//	//		}
//	//		else if (obacbool == 0 && obcbool == 0){
//	//			cout << "障碍物2完全未阻挡武器向量" << endl;
//	//			cout << "未阻挡时武器逆向量:" << vset.AC_MWv << endl;
//	//			cout << "未阻挡时武器顺向量:" << vset.C_MWv << endl;
//	//		}
//	//	}
//
//}/




//--------------------------判断障碍物阻挡函数初步------------------//变更武器偏移向量,来计算命中率.
//这里还需要判断是否调用障碍物阻挡函数
void ObstacleCheck(Inface&vset, int i)
{
	//cout << "调用障碍物阻挡判断函数" << endl;

	//判断障碍物向量1和武器偏移向量组的关系
	//判断障碍物向量的夹角和武器误差向量的大小,
	//假如障碍物向量夹角大于武器误差
	//判断是否包含
	//case
	double argob;
	argob = vset.OBv_AC[i].ClockwiseAngle(vset.OBv_C[i]);

	double argwp = vset.AC_MWv.ClockwiseAngle(vset.C_MWv);//计算真实的武器夹角
	//cout << "障碍物向量夹角=" << argob << endl;

	//cout << "武器夹角:" << argwp << endl;


	//需要求真实的武器夹角

	//判断大小ob1

	if (argob >= argwp)
	{
		//cout << endl;
		//cout << "障碍物的夹角大于武器误差" << endl;
		//判断武器向量是否在里面:
		double AC_ACMWv_ACOBv;
		double C_ACMWv_COBv;
		AC_ACMWv_ACOBv = vset.AC_MWv.AntiClockwiseAngle(vset.OBv_AC[i]);
		C_ACMWv_COBv = vset.AC_MWv.ClockwiseAngle(vset.OBv_C[i]);

		//cout << endl;
		//cout << "逆偏武器与障碍逆向量的逆角:" << AC_ACMWv_ACOBv << endl;
		//cout << "逆偏武器与障碍顺向量的顺角:" << C_ACMWv_COBv << endl;

		double wpacbool;//定义是否在的布尔值

		if (AC_ACMWv_ACOBv + C_ACMWv_COBv == argob){
			wpacbool = 1;
			//cout << "障碍物阻挡武器逆偏向量" << endl;
		}
		else {
			wpacbool = 0;
			//cout << "障碍物未--阻挡武器逆偏向量" << endl;
		}

		//下面判断顺偏向量
		double AC_CMWv_ACOB1v;
		double C_CMWv_COB1v;
		AC_CMWv_ACOB1v = vset.C_MWv.AntiClockwiseAngle(vset.OBv_AC[i]);
		C_CMWv_COB1v = vset.C_MWv.ClockwiseAngle(vset.OBv_C[i]);

		//cout << endl;
		//cout << "顺偏武器与障碍逆向量的逆角:" << AC_CMWv_ACOB1v << endl;
		//cout << "顺偏武器与障碍顺向量的顺角:" << C_CMWv_COB1v << endl;

		double wpcbool;//定义是否在的布尔值

		if (AC_CMWv_ACOB1v + C_CMWv_COB1v == argob){
			wpcbool = 1;
			//cout << "障碍物阻挡武器顺偏向量" << endl;
		}
		else {
			wpacbool = 0;
			//cout << "障碍物未--阻挡武器顺偏向量" << endl;
		}

		//根据布尔值判断
		if (wpacbool == 1 && wpcbool == 1){
			//cout << "障碍物完全阻挡武器!" << endl;
			vset.AC_MWv.set(0, 0);
			vset.C_MWv.set(0, 0);
			//cout << "完全阻挡时武器逆向量:" << vset.AC_MWv << endl;
			//cout << "完全阻挡时武器顺向量:" << vset.C_MWv << endl;
		}
		else if (wpacbool == 1 && wpcbool == 0){
			//cout << "障碍物阻挡武器逆偏向量" << endl;
			vset.AC_MWv = vset.OBv_C[i];
			//cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
			//cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
		}
		else if (wpacbool == 0 && wpcbool == 1){
			//cout << "障碍物阻挡武器顺偏向量" << endl;
			vset.C_MWv = vset.OBv_AC[i];
			//cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
			//cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
		}
		else if (wpacbool == 0 && wpcbool == 0){
			//cout << "障碍物完全未阻挡武器向量" << endl;
			//cout << "未阻挡时武器逆向量:" << vset.AC_MWv << endl;
			//cout << "未阻挡时武器顺向量:" << vset.C_MWv << endl;
		}
	}


	//-----------------
	else //武器偏移向量大于障碍物向量
	{
		//cout << endl;
		//cout << "障碍物的夹角小于武器误差" << endl;
		double AC_ACOB1v_ACMWv;
		double C_ACOB1v_CMWv;
		AC_ACOB1v_ACMWv = vset.OBv_AC[i].AntiClockwiseAngle(vset.AC_MWv);
		C_ACOB1v_CMWv = vset.OBv_AC[i].ClockwiseAngle(vset.C_MWv);

		//cout << endl;
		//cout << "障碍逆向量与逆偏武器的逆角:" << AC_ACOB1v_ACMWv << endl;
		//cout << "障碍逆向量与顺偏武器的顺角:" << C_ACOB1v_CMWv << endl;

		double obacbool;//定义是否在的布尔值

		if (AC_ACOB1v_ACMWv + C_ACOB1v_CMWv == argwp)
		{
			obacbool = 1;
			//cout << "障碍物阻挡武器顺偏向量" << endl;
		}
		else
		{
			obacbool = 0;
			//cout << "障碍物未--阻挡武器顺偏向量" << endl;
		}


		//下面判断顺偏向量
		double AC_COB1v_ACMWv;
		double C_COB1v_CMWv;
		AC_COB1v_ACMWv = vset.OBv_C[i].AntiClockwiseAngle(vset.AC_MWv);
		C_COB1v_CMWv = vset.OBv_C[i].ClockwiseAngle(vset.C_MWv);

		//cout << endl;
		//cout << "障碍逆向量与顺偏武器的逆角:" << AC_COB1v_ACMWv << endl;
		//cout << "障碍逆向量与顺偏武器的顺角:" << C_COB1v_CMWv << endl;

		double obcbool;//定义是否在的布尔值

		if (AC_COB1v_ACMWv + C_COB1v_CMWv == argwp){
			obcbool = 1;
			//cout << "障碍物阻挡武器逆偏向量" << endl;
		}
		else {
			obcbool = 0;
			//cout << "障碍物未--阻挡武器逆偏向量" << endl;
		}



		//根据布尔值判断
		if (obacbool == 1 && obcbool == 1){
			//cout << "障碍物在武器之中!" << endl;
			//cout << "呃目前还没想好怎么处理" << endl;
		}
		else if (obacbool == 1 && obcbool == 0){
			//cout << "障碍物阻挡武器顺偏向量" << endl;
			vset.C_MWv = vset.OBv_AC[i];
			//cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
			//cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
		}
		else if (obacbool == 0 && obcbool == 1){
			//cout << "障碍物阻挡武器逆偏向量" << endl;
			vset.AC_MWv = vset.OBv_C[i];
			//cout << "逆半阻挡时武器逆偏向量" << vset.AC_MWv << endl;
			//cout << "逆半阻挡时武器顺偏向量" << vset.C_MWv << endl;
		}
		else if (obacbool == 0 && obcbool == 0){
			//cout << "障碍物完全未阻挡武器向量" << endl;
			//cout << "未阻挡时武器逆向量:" << vset.AC_MWv << endl;
			//cout << "未阻挡时武器顺向量:" << vset.C_MWv << endl;
		}
	}
}


//-------------------------判断是否调用阻挡检查函数初步-------------//计算距离判断是否调用

void ObCkCheck(const RobotAI_BattlefieldInformation&info, Inface& vset, int i)
{
	//cout << endl;
	//cout << "调用阻挡预检查函数:" << endl;
	//判断极限距离:
	//需要偏移向量的长度和敌方引擎的半径
	double r = info.robotInformation[vset.enID].circle.r;
	double l = vset.OBv_C[i].length;
	//cout << "敌方半径:" << r << endl;
	//cout << "障碍逆偏向量长度:" << l << endl;
	double limdis = sqrt(r*r + l*l);
	//cout << "判断极限距离:" << limdis<< endl;

	double distance = vset.MEv.length;
	if (distance > limdis){
		ObstacleCheck(vset, i);
		//cout << "在极限距离之外!需要调用阻挡函数!" << endl;
		//cout << endl;
		//cout << endl;

	}

	return;
}




//完整的阻挡检查
void ObckckCk(const RobotAI_BattlefieldInformation&info, Inface& vset){

	ObCkCheck(info, vset, 0);
	ObCkCheck(info, vset, 1);
}








