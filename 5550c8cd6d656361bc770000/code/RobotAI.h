#pragma once

#include "RobotAI_Interface.h"



class RobotAI:public RobotAI_Interface
{
protected:
	//TODO:����������������Լ��ĳ�Ա����


public:

	//************************************************
	//��һ�κ����������Ҷ��������������ê�˱����

	RobotAI();
	virtual ~RobotAI();



	//-----------------------------------------------------
	//1.������ɵ�ս������
	//-----------------------------------------------------

	//֡���ݺ���
	//���ܣ���ÿһ֡�����ã������Ļ�������һ֡�Ķ�������
	//������order	...	���ײ���ָ����ں������и�����ֵ�Բ��ݻ�������һ֡����Ϊ
	//		info	...	ս����Ϣ
	//		myID	... �Լ�������info��robot�����Ӧ���±�
	//		(�⼸����������ϸ˵���ڿ����ֲ�����ҵ�����Ҳ������RobotAIstruct.h��ֱ���ҵ����ǵĴ���)
	virtual void Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID);



	//��ѡװ������
	//���ܣ���ս����ʼʱΪ��Ļ���ѡ����ʵ����������������ؾ�
	//������weapon	...	������ѡ����������ں������и�����ֵ
	//		engine	...	������ѡ������棬�ں������и�����ֵ
	//tip:	������Ĳ�����ö������ weapontypename �� enginetypename
	//		�����ĵ�������ϸ˵������Ҳ������RobotAIstruct.h��ֱ���ҵ����ǵĴ���
	//tip:	���һ��bool��û�õġ�������һ���˻�������
	virtual void ChooseArmor(weapontypename& weapon,enginetypename& engine,bool);


	//-----------------------------------------------------






	//-----------------------------------------------------
	//2.������Ϣ
	//-----------------------------------------------------

	//������Ļ��׵�����
	virtual string GetName();

	//���ػ��������˻��Ŷӵ�����
	virtual string GetAuthor();



	//����һ��(-255,255)֮��Ļ���������������ɫƫ��ֵ���졢�̡�����
	//�������flash�ͻ��˵Ĳ���Ԥ����Ԥ����ɫ�����Ч��
	virtual int GetWeaponRed();
	virtual int GetWeaponGreen();
	virtual int GetWeaponBlue();

	//����һ��(-255,255)֮��Ļ��������ؾߵ���ɫƫ��ֵ���졢�̡�����
	//�������flash�ͻ��˵Ĳ���Ԥ����Ԥ����ɫ�����Ч��
	virtual int GetEngineRed();
	virtual int GetEngineGreen();
	virtual int GetEngineBlue();

	//-----------------------------------------------------




	//-----------------------------------------------------
	//3.�ò�������Ĵ�������
	//-----------------------------------------------------


	//һ��ս����ʼʱ�����ã����ܿ���������ʼ��
	//������info	...	ս����Ϣ
	//		myID	... �Լ�������info��robot�����Ӧ���±�
	virtual void onBattleStart(const RobotAI_BattlefieldInformation& info,int myID);



	//һ��ս������ʱ�����ã����ܿ������������㶯̬������ڴ�ռ䣨��������˵Ļ���
	//������info	...	ս����Ϣ
	//		myID	... �Լ�������info��robot�����Ӧ���±�
	virtual void onBattleEnd(const RobotAI_BattlefieldInformation& info,int myID);


	//�л��׿���ʱ������
	//������fireID	... ����Ļ����±�
	virtual void onSomeoneFire(int);


	//���ӵ�����ʱ������
	//������btn	...	��������ӵ����ࣨö�����ͣ�
	virtual void onHit(int,bullettypename);

	//-------------------------------------------------------------



	//***************************************************************



	//TODO:����������������Լ��ĺ�������,����RobotAI.cpp�б�д��Ӧ�ĺ�������
	virtual int runAndrun(RobotAI_RobotInformation me, Circle target);
	virtual int Rotate(double angle_to, double angle_now);
	virtual Circle whichArsenal(RobotAI_ArsenalInformation ar1, RobotAI_ArsenalInformation ar2, Circle me);
	virtual double Distance(double x1, double y1, double x2, double y2);
	//ȷ����ת�Ƿ���
	virtual double howToRotate(Circle me, Circle armor, double weapon_rotation, double vx, double vy);
	//���ս��̹�ˣ�����ӵ���
	virtual int AvoidBulletAFV(RobotAI_BulletInformation bu, RobotAI_RobotInformation me);
	//������ӵ���в����
	virtual bool BulletShotMe(Circle bu, Circle me, double vx, double vy, weapontypename weapontype);
	//�����ϰ���
	virtual int avoidObstacleAFV(RobotAI_RobotInformation me, Circle obstacle);
	virtual Circle whichObstacle(Circle ob1, Circle ob2, Circle me);
	virtual int StrikeObstacle(Circle me, Circle obstacle[],const int num_obs);
	//�õ�����˾���500�ĵ�
	virtual Circle get500Point(Circle me, Circle armor, double dist);
	//����ǽ
	virtual int avoidWall(RobotAI_RobotInformation me);
	//ս��̹�˶��ũ�ںͰ����޵����
	virtual int AvoidCannonAFV(RobotAI_BulletInformation bu, RobotAI_RobotInformation me);


};