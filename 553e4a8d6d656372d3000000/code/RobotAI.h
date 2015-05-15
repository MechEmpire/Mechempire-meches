#pragma once

#include "RobotAI_Interface.h"



class RobotAI:public RobotAI_Interface
{
protected:
	//TODO:����������������Լ��ĳ�Ա����
	int MyCar = 0;
	const int color = 255;
	enum{
		AFV_Esaw,
		AFV_Prism,
		AFV_WT_Machinegun
	};
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

	bool HaveBarrier(const RobotAI_BattlefieldInformation& info,const int myID,const int ArID);//�жϺ͵ط�֮���Ƿ����ϰ���
	int GetNearetBul(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID);//��ȡ������ӵ��±�

	/*<<<<<<<��ÿ��������Ӧ�Ĳ���(������غ���ʱ�����·�Ϊ��Ӧ����)*/
	void onTag1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID);//��׼����������Ԥ�У�
	double onTagRota(const RobotAI_BattlefieldInformation& info, const int myID);//���������нǺ�����淽��ļнǲ�

	void onMove1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID,const Circle circle_tag);//�ƶ���(circle_tag.x,circle_tag.y���ĺ���
	void onMove1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID, const double x_tag, const double y_tag);//�ƶ���(x_tag,y_tag)�ĺ���
	void onMove1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID, const double rota_tag);//��rota_tag���Ƕȣ��ƶ�
	
	void onFire1(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID);//������

	bool onDodge(RobotAI_Order& order, const RobotAI_BattlefieldInformation& info, const int myID, const RobotAI_BulletInformation& bullet);//��������

	/*��ÿ��������Ӧ�Ĳ���>>>>>>>*/

	bool WillNotHit(const RobotAI_BulletInformation& bullet, const RobotAI_RobotInformation& rotinfo);//�ж��ӵ����Բ����
	bool WillHit(const RobotAI_BulletInformation& bullet, const RobotAI_RobotInformation& rotinfo);
	double abvalue(double x);
	double pointdis(const Circle a1, const Circle a2, const Circle a3);
	double pointdis(double x1, double y1, double x2, double y2, double x3, double y3); 
	double pointdis(double k,double x1,double y1,double x3, double y3);
	double dis(double x1, double y1, double x2, double y2);
	double dis(const Circle& a, const Circle& b);
	
};