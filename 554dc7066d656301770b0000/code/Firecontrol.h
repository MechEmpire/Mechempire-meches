#pragma once
#include "RobotAI.h"
#include "Vector.h"
#include "Interface.h"
#include <iostream>
using namespace std;






//------��׼����










//-------------------------������������;//��������...��ȫ
void AdjustWepon(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info,Inface&vset){
	//cout << "����������������" << endl;
	//�ж������н�:
	double C_MW_ME, AC_MW_ME;
	AC_MW_ME = vset.MWv.AntiClockwiseAngle(vset.MEv);
	C_MW_ME = vset.MWv.ClockwiseAngle(vset.MEv);
	//cout << "�����ʱ�������Ƕ�:" << info.robotInformation[vset.myID].weaponRotation << endl;
	//cout << "�����ʱ��������  :" << vset.MWv << endl;
	//cout << "��һ֡����������λ����������ʱ��н�:" << AC_MW_ME<<endl;
	//cout << "��һ֡����������λ��������˳ʱ��н�:" << C_MW_ME << endl;

	if (C_MW_ME < AC_MW_ME){
		order.wturn = 1;//д��1��˳ʱ����ת...
		//cout << "����˳ʱ����ת" << endl;
	}
	else
	{
		order.wturn = -1;
		//cout << "������ʱ����ת" << endl;
	}
	//cout << "���������������ý���" << endl;
}





//-------------------------��̬�����ʼ��㺯������-------------------//�������ű�,���
double HitRate(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, Inface&vset){
	//cout << endl;
	//cout << endl;
	//cout << "�����ǵ��������ʺ���ʱ�����" << endl;
	//����������:
	double hitrate;
	//��Ϊλ�üн�С��һ������ô��...Ϊ�˷����������ô��:
	//����λƫ�н�,����ƫ�Ƽн�:

	double argME, argMW;
	argME = vset.AC_MEv.ClockwiseAngle(vset.C_MEv);
	argMW = vset.AC_MWv.ClockwiseAngle(vset.C_MWv);
	//cout << "��ƫλ������:" << vset.AC_MEv;
	//cout << "˳ƫλ������:" << vset.C_MEv;


	//cout << "λ�üн�:" << argME << endl;
	//cout << "�������:" << argMW << endl;
	//�õ���ƫ����������˳ƫλ������֮���˳�� ����ƫλ�����
	double AC_ACMWv_ACMEv;
	double C_ACMWv_CMEv;
	
	
	AC_ACMWv_ACMEv = vset.AC_MWv.AntiClockwiseAngle(vset.AC_MEv);
	C_ACMWv_CMEv = vset.AC_MWv.ClockwiseAngle(vset.C_MEv);
	//cout << endl;
	//cout << "������ƫ����:" << vset.AC_MWv << endl;
	//cout << "��ƫ��������λ�н�:" << AC_ACMWv_ACMEv << endl;
	//cout << "��ƫ������˳λ�н�:" << C_ACMWv_CMEv << endl;

	//��ƫ���������Ƿ���λ����˳����֮��


	//���ǰ��������Ǻ���argme,�Ǿ�˵��������:
	//�����Ƿ������еĲ���ֵ:
	bool wpac_bool;
	if (C_ACMWv_CMEv + AC_ACMWv_ACMEv == argME){
		wpac_bool = true;
		//cout << "�� ����" << endl;
	}
	else{
		wpac_bool = false;
		//cout << "�� δ����" << endl;
	}


	//�������ж� ˳ƫ��������:
	double AC_CMWv_ACMEv;
	double C_CMWv_CMEv;
	

	AC_CMWv_ACMEv = vset.C_MWv.AntiClockwiseAngle(vset.AC_MEv);
	C_CMWv_CMEv = vset.C_MWv.ClockwiseAngle(vset.C_MEv);
	
	//cout << endl;
	//cout << "����˳ƫ����:" << vset.C_MWv << endl;
	//cout << "˳ƫ��������λ�н�:" << AC_CMWv_ACMEv << endl;
	//cout << "˳ƫ������˳λ�н�:" << C_CMWv_CMEv << endl;

	bool wpc_bool;
	if (C_CMWv_CMEv + AC_CMWv_ACMEv == argME){
		wpc_bool = true;
		//cout << "˳ ����" << endl;
	}
	else {
		wpc_bool = false;
		//cout << "˳ δ����" << endl;
	}

	//�������ȫ��
	if (wpac_bool&&wpc_bool == 1){
		hitrate = 1;
		//cout << "��ȫ����" << endl;
	}
	else if (wpac_bool == 1 && wpc_bool == 0){
		hitrate = C_ACMWv_CMEv / vset.mywp.error;
		//cout << "�������" << endl;
	}
	else if (wpac_bool == 0 && wpc_bool == 1){
		hitrate = AC_CMWv_ACMEv / vset.mywp.error;
		//cout << "˳������" << endl;
	}

	else{
		hitrate = 0;
		//cout << "δ����" << endl;
	}
	cout << "������Ϊ:" << hitrate << endl;

	//cout << "�����ʺ������ý���" << endl;
	//cout << endl;
	return hitrate;
}


//��ǰ���赲�ж�
////--------------------------�ж��ϰ����赲��������------------------//�������ƫ������,������������.
////���ﻹ��Ҫ�ж��Ƿ�����ϰ����赲����
//void ObstacleCheck(Inface&vset,int i)
//{
//	cout << "�����ϰ����赲�жϺ���" << endl;
//
//	//�ж��ϰ�������1������ƫ��������Ĺ�ϵ
//	//�ж��ϰ��������ļнǺ�������������Ĵ�С,
//	//�����ϰ��������нǴ����������
//	//�ж��Ƿ����
//	//case
//	double argob1;//�ϰ��������н�
//	argob1 = vset.OB1v_AC.ClockwiseAngle(vset.OB1v_C);
//	double argob2;
//	argob2 = vset.OB2v_AC.ClockwiseAngle(vset.OB2v_C);
//	double argwp = vset.mywp.error;
//	cout << "�ϰ��������н�1=" << argob1 << endl;
//	cout << "�ϰ��������н�2=" << argob2 << endl;
//	cout << "�����н�:" << argwp << endl;
//
//
//	//erroֱ�Ӿ��������н�
//
//	//�жϴ�Сob1
//
//	if (argob1 >= argwp)
//	{
//		cout << endl;
//		cout << "�ϰ���1�ļнǴ����������" << endl;
//		//�ж����������Ƿ�������:
//		double AC_ACMWv_ACOB1v;
//		double C_ACMWv_COB1v;
//		AC_ACMWv_ACOB1v = vset.AC_MWv.AntiClockwiseAngle(vset.OB1v_AC);
//		C_ACMWv_COB1v = vset.AC_MWv.ClockwiseAngle(vset.OB1v_C);
//
//		cout << endl;
//		cout << "��ƫ�������ϰ�1�����������:" << AC_ACMWv_ACOB1v << endl;
//		cout << "��ƫ�������ϰ�1˳������˳��:" << C_ACMWv_COB1v << endl;
//
//		double wpacbool;//�����Ƿ��ڵĲ���ֵ
//
//		if (AC_ACMWv_ACOB1v + C_ACMWv_COB1v == argob1){
//			wpacbool = 1;
//			cout << "�ϰ���1�赲������ƫ����" << endl;
//		}
//		else {
//			wpacbool = 0;
//			cout << "�ϰ���1δ--�赲������ƫ����" << endl;
//		}
//
//		//�����ж�˳ƫ����
//		double AC_CMWv_ACOB1v;
//		double C_CMWv_COB1v;
//		AC_CMWv_ACOB1v = vset.C_MWv.AntiClockwiseAngle(vset.OB1v_AC);
//		C_CMWv_COB1v = vset.C_MWv.ClockwiseAngle(vset.OB1v_C);
//
//		cout << endl;
//		cout << "˳ƫ�������ϰ�1�����������:" << AC_CMWv_ACOB1v << endl;
//		cout << "˳ƫ�������ϰ�1˳������˳��:" << C_CMWv_COB1v << endl;
//
//		double wpcbool;//�����Ƿ��ڵĲ���ֵ
//
//		if (AC_CMWv_ACOB1v + C_CMWv_COB1v == argob1){
//			wpcbool = 1;
//			cout << "�ϰ���1�赲����˳ƫ����" << endl;
//		}
//		else {
//			wpacbool = 0;
//			cout << "�ϰ���1δ--�赲����˳ƫ����" << endl;
//		}
//
//		//���ݲ���ֵ�ж�
//		if (wpacbool == 1 && wpcbool == 1){
//			cout << "�ϰ���1��ȫ�赲����!" << endl;
//			vset.AC_MWv.set(0, 0);
//			vset.C_MWv.set(0, 0);
//			cout << "��ȫ�赲ʱ����������:" << vset.AC_MWv << endl;
//			cout << "��ȫ�赲ʱ����˳����:" << vset.C_MWv << endl;
//		}
//		else if (wpacbool == 1 && wpcbool == 0){
//			cout << "�ϰ���1�赲������ƫ����" << endl;
//			vset.AC_MWv = vset.OB1v_C;
//			cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
//			cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
//		}
//		else if (wpacbool == 0 && wpcbool == 1){
//			cout << "�ϰ���1�赲����˳ƫ����" << endl;
//			vset.C_MWv = vset.OB1v_AC;
//			cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
//			cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
//		}
//		else if (wpacbool == 0 && wpcbool == 0){
//			cout << "�ϰ���1��ȫδ�赲��������" << endl;
//			cout << "δ�赲ʱ����������:" << vset.AC_MWv << endl;
//			cout << "δ�赲ʱ����˳����:" << vset.C_MWv << endl;
//		}
//	}
//
//
//		//-----------------
//	else //����ƫ�����������ϰ�������
//	{
//			cout << endl;
//			cout << "�ϰ���1�ļн�С���������" << endl;
//			double AC_ACOB1v_ACMWv;
//			double C_ACOB1v_CMWv;
//			AC_ACOB1v_ACMWv = vset.OB1v_AC.AntiClockwiseAngle(vset.AC_MWv);
//			C_ACOB1v_CMWv = vset.OB1v_AC.ClockwiseAngle(vset.C_MWv);
//
//			cout << endl;
//			cout << "�ϰ�1����������ƫ���������:" << AC_ACOB1v_ACMWv << endl;
//			cout << "�ϰ�1��������˳ƫ������˳��:" << C_ACOB1v_CMWv << endl;
//
//			double obacbool;//�����Ƿ��ڵĲ���ֵ
//
//			if (AC_ACOB1v_ACMWv + C_ACOB1v_CMWv == argwp)
//			{
//				obacbool = 1;
//				cout << "�ϰ���1�赲����˳ƫ����" << endl;
//			}
//			else 
//			{
//				obacbool = 0;
//				cout << "�ϰ���1δ--�赲����˳ƫ����" << endl;
//			}
//
//
//			//�����ж�˳ƫ����
//			double AC_COB1v_ACMWv;
//			double C_COB1v_CMWv;
//			AC_COB1v_ACMWv = vset.OB1v_C.AntiClockwiseAngle(vset.AC_MWv);
//			C_COB1v_CMWv = vset.OB1v_C.ClockwiseAngle(vset.C_MWv);
//
//			cout << endl;
//			cout << "�ϰ�1��������˳ƫ���������:" << AC_COB1v_ACMWv << endl;
//			cout << "�ϰ�1��������˳ƫ������˳��:" << C_COB1v_CMWv << endl;
//
//			double obcbool;//�����Ƿ��ڵĲ���ֵ
//
//			if (AC_COB1v_ACMWv + C_COB1v_CMWv == argwp){
//				obcbool = 1;
//				cout << "�ϰ���1�赲������ƫ����" << endl;
//			}
//			else {
//				obcbool = 0;
//				cout << "�ϰ���1δ--�赲������ƫ����" << endl;
//			}
//
//
//
//			//���ݲ���ֵ�ж�
//			if (obacbool == 1 && obcbool == 1){
//				cout << "�ϰ���������֮��!" << endl;
//				cout << "��Ŀǰ��û�����ô����" << endl;
//			}
//			else if (obacbool == 1 && obcbool == 0){
//				cout << "�ϰ���1�赲����˳ƫ����" << endl;
//				vset.C_MWv = vset.OB1v_AC;
//				cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
//				cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
//			}
//			else if (obacbool == 0 && obcbool == 1){
//				cout << "�ϰ���1�赲������ƫ����" << endl;
//				vset.AC_MWv = vset.OB1v_C;
//				cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
//				cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
//			}
//			else if (obacbool == 0 && obcbool == 0){
//				cout << "�ϰ���1��ȫδ�赲��������" << endl;
//				cout << "δ�赲ʱ����������:" << vset.AC_MWv << endl;
//				cout << "δ�赲ʱ����˳����:" << vset.C_MWv << endl;
//			}
//	}
//	
//
//
//
//
//
//	////�жϵڶ����ϰ���
//
//	////�жϴ�Сob2
//
//	//if (argob2 >= argwp)
//	//{
//	//	cout << endl;
//	//	cout << "�ϰ���2�ļнǴ����������" << endl;
//	//	//�ж����������Ƿ�������:
//	//	double AC_ACMWv_ACOB2v;
//	//	double C_ACMWv_COB2v;
//	//	AC_ACMWv_ACOB2v = vset.AC_MWv.AntiClockwiseAngle(vset.OB2v_AC);
//	//	C_ACMWv_COB2v = vset.AC_MWv.ClockwiseAngle(vset.OB2v_C);
//
//	//	cout << endl;
//	//	cout << "��ƫ�������ϰ�2�����������:" << AC_ACMWv_ACOB2v << endl;
//	//	cout << "��ƫ�������ϰ�2˳������˳��:" << C_ACMWv_COB2v << endl;
//
//	//	double wpacbool;//�����Ƿ��ڵĲ���ֵ
//
//	//	if (AC_ACMWv_ACOB2v + C_ACMWv_COB2v == argob2){
//	//		wpacbool = 1;
//	//		cout << "�ϰ���2�赲������ƫ����" << endl;
//	//	}
//	//	else {
//	//		wpacbool = 0;
//	//		cout << "�ϰ���2δ--�赲������ƫ����" << endl;
//	//	}
//
//
//
//	//	//�����ж�˳ƫ����
//	//	double AC_CMWv_ACOB2v;
//	//	double C_CMWv_COB2v;
//	//	AC_CMWv_ACOB2v = vset.C_MWv.AntiClockwiseAngle(vset.OB2v_AC);
//	//	C_CMWv_COB2v = vset.C_MWv.ClockwiseAngle(vset.OB2v_C);
//
//	//	cout << endl;
//	//	cout << "˳ƫ�������ϰ�2�����������:" << AC_CMWv_ACOB2v << endl;
//	//	cout << "˳ƫ�������ϰ�2˳������˳��:" << C_CMWv_COB2v << endl;
//
//	//	double wpcbool;//�����Ƿ��ڵĲ���ֵ
//
//	//	if (AC_CMWv_ACOB2v + C_CMWv_COB2v == argob2){
//	//		wpcbool = 1;
//	//		cout << "�ϰ���2�赲����˳ƫ����" << endl;
//	//	}
//	//	else {
//	//		wpacbool = 0;
//	//		cout << "�ϰ���2δ--�赲����˳ƫ����" << endl;
//	//	}
//
//
//
//	//	//���ݲ���ֵ�ж�
//	//	if (wpacbool == 1 && wpcbool == 1){
//	//		cout << "�ϰ���2��ȫ�赲����!" << endl;
//	//		vset.AC_MWv.set(0,0);
//	//		vset.C_MWv.set(0,0);
//	//		cout << "��ȫ�赲ʱ����������:" << vset.AC_MWv << endl;
//	//		cout << "��ȫ�赲ʱ����˳����:" << vset.C_MWv << endl;
//	//	}
//	//	else if (wpacbool == 1 && wpcbool == 0){
//	//		cout << "�ϰ���2�赲������ƫ����" << endl;
//	//		vset.AC_MWv = vset.OB2v_C;
//	//		cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
//	//		cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
//	//	}
//	//	else if (wpacbool == 0 && wpcbool == 1){
//	//		cout << "�ϰ���2�赲����˳ƫ����" << endl;
//	//		vset.C_MWv = vset.OB2v_AC;
//	//		cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
//	//		cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
//	//	}
//	//	else if (wpacbool == 0 && wpcbool == 0){
//	//		cout << "�ϰ���2��ȫδ�赲��������" << endl;
//	//		cout << "δ�赲ʱ����������:" << vset.AC_MWv << endl;
//	//		cout << "δ�赲ʱ����˳����:" << vset.C_MWv << endl;
//	//	}
//	//}
//
//
//
//	//	//-----------------
//	//	else //����ƫ�����������ϰ�������
//	//	{
//	//		cout << endl;
//	//		cout << "�ϰ���2�ļн�С���������" << endl;
//	//		double AC_ACOB2v_ACMWv;
//	//		double C_ACOB2v_CMWv;
//	//		AC_ACOB2v_ACMWv = vset.OB2v_AC.AntiClockwiseAngle(vset.AC_MWv);
//	//		C_ACOB2v_CMWv = vset.OB2v_AC.ClockwiseAngle(vset.C_MWv);
//
//	//		cout << endl;
//	//		cout << "�ϰ�2����������ƫ���������:" << AC_ACOB2v_ACMWv << endl;
//	//		cout << "�ϰ�2��������˳ƫ������˳��:" << C_ACOB2v_CMWv << endl;
//
//	//		double obacbool;//�����Ƿ��ڵĲ���ֵ
//
//	//		if (AC_ACOB2v_ACMWv + C_ACOB2v_CMWv == argwp){
//	//			obacbool = 1;
//	//			cout << "�ϰ���2�赲����˳ƫ����" << endl;
//	//		}
//	//		else {
//	//			obacbool = 0;
//	//			cout << "�ϰ���2δ--�赲����˳ƫ����" << endl;
//	//		}
//
//
//	//		//�����ж�˳ƫ����
//	//		double AC_COB2v_ACMWv;
//	//		double C_COB2v_CMWv;
//	//		AC_COB2v_ACMWv = vset.OB2v_C.AntiClockwiseAngle(vset.AC_MWv);
//	//		C_COB2v_CMWv = vset.OB2v_C.ClockwiseAngle(vset.C_MWv);
//
//	//		cout << endl;
//	//		cout << "�ϰ�2��������˳ƫ���������:" << AC_COB2v_ACMWv << endl;
//	//		cout << "�ϰ�2��������˳ƫ������˳��:" << C_COB2v_CMWv << endl;
//
//	//		double obcbool;//�����Ƿ��ڵĲ���ֵ
//
//	//		if (AC_COB2v_ACMWv + C_COB2v_CMWv == argwp)
//	//		{
//	//			obcbool = 1;
//	//			cout << "�ϰ���2�赲������ƫ����" << endl;
//	//		}
//	//		else {
//	//			obcbool = 0;
//	//			cout << "�ϰ���2δ--�赲������ƫ����" << endl;
//	//		}
//
//
//
//	//		//���ݲ���ֵ�ж�
//	//		if (obacbool == 1 && obcbool == 1){
//	//			cout << "�ϰ���������֮��!" << endl;
//	//			cout << "��Ŀǰ��û�����ô����" << endl;
//	//		}
//	//		else if (obacbool == 1 && obcbool == 0){
//	//			cout << "�ϰ���2�赲����˳ƫ����" << endl;
//	//			vset.C_MWv = vset.OB2v_AC;
//	//			cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
//	//			cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
//	//		}
//	//		else if (obacbool == 0 && obcbool == 1){
//	//			cout << "�ϰ���2�赲������ƫ����" << endl;
//	//			vset.AC_MWv = vset.OB2v_C;
//	//			cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
//	//			cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
//	//		}
//	//		else if (obacbool == 0 && obcbool == 0){
//	//			cout << "�ϰ���2��ȫδ�赲��������" << endl;
//	//			cout << "δ�赲ʱ����������:" << vset.AC_MWv << endl;
//	//			cout << "δ�赲ʱ����˳����:" << vset.C_MWv << endl;
//	//		}
//	//	}
//
//}/




//--------------------------�ж��ϰ����赲��������------------------//�������ƫ������,������������.
//���ﻹ��Ҫ�ж��Ƿ�����ϰ����赲����
void ObstacleCheck(Inface&vset, int i)
{
	//cout << "�����ϰ����赲�жϺ���" << endl;

	//�ж��ϰ�������1������ƫ��������Ĺ�ϵ
	//�ж��ϰ��������ļнǺ�������������Ĵ�С,
	//�����ϰ��������нǴ����������
	//�ж��Ƿ����
	//case
	double argob;
	argob = vset.OBv_AC[i].ClockwiseAngle(vset.OBv_C[i]);

	double argwp = vset.AC_MWv.ClockwiseAngle(vset.C_MWv);//������ʵ�������н�
	//cout << "�ϰ��������н�=" << argob << endl;

	//cout << "�����н�:" << argwp << endl;


	//��Ҫ����ʵ�������н�

	//�жϴ�Сob1

	if (argob >= argwp)
	{
		//cout << endl;
		//cout << "�ϰ���ļнǴ����������" << endl;
		//�ж����������Ƿ�������:
		double AC_ACMWv_ACOBv;
		double C_ACMWv_COBv;
		AC_ACMWv_ACOBv = vset.AC_MWv.AntiClockwiseAngle(vset.OBv_AC[i]);
		C_ACMWv_COBv = vset.AC_MWv.ClockwiseAngle(vset.OBv_C[i]);

		//cout << endl;
		//cout << "��ƫ�������ϰ������������:" << AC_ACMWv_ACOBv << endl;
		//cout << "��ƫ�������ϰ�˳������˳��:" << C_ACMWv_COBv << endl;

		double wpacbool;//�����Ƿ��ڵĲ���ֵ

		if (AC_ACMWv_ACOBv + C_ACMWv_COBv == argob){
			wpacbool = 1;
			//cout << "�ϰ����赲������ƫ����" << endl;
		}
		else {
			wpacbool = 0;
			//cout << "�ϰ���δ--�赲������ƫ����" << endl;
		}

		//�����ж�˳ƫ����
		double AC_CMWv_ACOB1v;
		double C_CMWv_COB1v;
		AC_CMWv_ACOB1v = vset.C_MWv.AntiClockwiseAngle(vset.OBv_AC[i]);
		C_CMWv_COB1v = vset.C_MWv.ClockwiseAngle(vset.OBv_C[i]);

		//cout << endl;
		//cout << "˳ƫ�������ϰ������������:" << AC_CMWv_ACOB1v << endl;
		//cout << "˳ƫ�������ϰ�˳������˳��:" << C_CMWv_COB1v << endl;

		double wpcbool;//�����Ƿ��ڵĲ���ֵ

		if (AC_CMWv_ACOB1v + C_CMWv_COB1v == argob){
			wpcbool = 1;
			//cout << "�ϰ����赲����˳ƫ����" << endl;
		}
		else {
			wpacbool = 0;
			//cout << "�ϰ���δ--�赲����˳ƫ����" << endl;
		}

		//���ݲ���ֵ�ж�
		if (wpacbool == 1 && wpcbool == 1){
			//cout << "�ϰ�����ȫ�赲����!" << endl;
			vset.AC_MWv.set(0, 0);
			vset.C_MWv.set(0, 0);
			//cout << "��ȫ�赲ʱ����������:" << vset.AC_MWv << endl;
			//cout << "��ȫ�赲ʱ����˳����:" << vset.C_MWv << endl;
		}
		else if (wpacbool == 1 && wpcbool == 0){
			//cout << "�ϰ����赲������ƫ����" << endl;
			vset.AC_MWv = vset.OBv_C[i];
			//cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
			//cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
		}
		else if (wpacbool == 0 && wpcbool == 1){
			//cout << "�ϰ����赲����˳ƫ����" << endl;
			vset.C_MWv = vset.OBv_AC[i];
			//cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
			//cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
		}
		else if (wpacbool == 0 && wpcbool == 0){
			//cout << "�ϰ�����ȫδ�赲��������" << endl;
			//cout << "δ�赲ʱ����������:" << vset.AC_MWv << endl;
			//cout << "δ�赲ʱ����˳����:" << vset.C_MWv << endl;
		}
	}


	//-----------------
	else //����ƫ�����������ϰ�������
	{
		//cout << endl;
		//cout << "�ϰ���ļн�С���������" << endl;
		double AC_ACOB1v_ACMWv;
		double C_ACOB1v_CMWv;
		AC_ACOB1v_ACMWv = vset.OBv_AC[i].AntiClockwiseAngle(vset.AC_MWv);
		C_ACOB1v_CMWv = vset.OBv_AC[i].ClockwiseAngle(vset.C_MWv);

		//cout << endl;
		//cout << "�ϰ�����������ƫ���������:" << AC_ACOB1v_ACMWv << endl;
		//cout << "�ϰ���������˳ƫ������˳��:" << C_ACOB1v_CMWv << endl;

		double obacbool;//�����Ƿ��ڵĲ���ֵ

		if (AC_ACOB1v_ACMWv + C_ACOB1v_CMWv == argwp)
		{
			obacbool = 1;
			//cout << "�ϰ����赲����˳ƫ����" << endl;
		}
		else
		{
			obacbool = 0;
			//cout << "�ϰ���δ--�赲����˳ƫ����" << endl;
		}


		//�����ж�˳ƫ����
		double AC_COB1v_ACMWv;
		double C_COB1v_CMWv;
		AC_COB1v_ACMWv = vset.OBv_C[i].AntiClockwiseAngle(vset.AC_MWv);
		C_COB1v_CMWv = vset.OBv_C[i].ClockwiseAngle(vset.C_MWv);

		//cout << endl;
		//cout << "�ϰ���������˳ƫ���������:" << AC_COB1v_ACMWv << endl;
		//cout << "�ϰ���������˳ƫ������˳��:" << C_COB1v_CMWv << endl;

		double obcbool;//�����Ƿ��ڵĲ���ֵ

		if (AC_COB1v_ACMWv + C_COB1v_CMWv == argwp){
			obcbool = 1;
			//cout << "�ϰ����赲������ƫ����" << endl;
		}
		else {
			obcbool = 0;
			//cout << "�ϰ���δ--�赲������ƫ����" << endl;
		}



		//���ݲ���ֵ�ж�
		if (obacbool == 1 && obcbool == 1){
			//cout << "�ϰ���������֮��!" << endl;
			//cout << "��Ŀǰ��û�����ô����" << endl;
		}
		else if (obacbool == 1 && obcbool == 0){
			//cout << "�ϰ����赲����˳ƫ����" << endl;
			vset.C_MWv = vset.OBv_AC[i];
			//cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
			//cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
		}
		else if (obacbool == 0 && obcbool == 1){
			//cout << "�ϰ����赲������ƫ����" << endl;
			vset.AC_MWv = vset.OBv_C[i];
			//cout << "����赲ʱ������ƫ����" << vset.AC_MWv << endl;
			//cout << "����赲ʱ����˳ƫ����" << vset.C_MWv << endl;
		}
		else if (obacbool == 0 && obcbool == 0){
			//cout << "�ϰ�����ȫδ�赲��������" << endl;
			//cout << "δ�赲ʱ����������:" << vset.AC_MWv << endl;
			//cout << "δ�赲ʱ����˳����:" << vset.C_MWv << endl;
		}
	}
}


//-------------------------�ж��Ƿ�����赲��麯������-------------//��������ж��Ƿ����

void ObCkCheck(const RobotAI_BattlefieldInformation&info, Inface& vset, int i)
{
	//cout << endl;
	//cout << "�����赲Ԥ��麯��:" << endl;
	//�жϼ��޾���:
	//��Ҫƫ�������ĳ��Ⱥ͵з�����İ뾶
	double r = info.robotInformation[vset.enID].circle.r;
	double l = vset.OBv_C[i].length;
	//cout << "�з��뾶:" << r << endl;
	//cout << "�ϰ���ƫ��������:" << l << endl;
	double limdis = sqrt(r*r + l*l);
	//cout << "�жϼ��޾���:" << limdis<< endl;

	double distance = vset.MEv.length;
	if (distance > limdis){
		ObstacleCheck(vset, i);
		//cout << "�ڼ��޾���֮��!��Ҫ�����赲����!" << endl;
		//cout << endl;
		//cout << endl;

	}

	return;
}




//�������赲���
void ObckckCk(const RobotAI_BattlefieldInformation&info, Inface& vset){

	ObCkCheck(info, vset, 0);
	ObCkCheck(info, vset, 1);
}








