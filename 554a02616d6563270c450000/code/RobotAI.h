#pragma once

#include <string>
#include <vector>
#include "Vector2D.h"
//#include "../sys/RobotAI_Interface.h"
#include "RobotAI_Interface.h"

#define MY battle_info.robotInformation[myID]
#define ENEMY battle_info.robotInformation[enemyID]



struct BulletDisSort
{
	int idx;
	double dissq;

	BulletDisSort(){idx=0;dissq=1e20;}
};



class RobotAI:public RobotAI_Interface
{
protected:
	//TODO:����������������Լ��ĳ�Ա����
	vector<BulletDisSort> bullet_sort;
	double crusher_death_rush;	//�����������̾���
	int tmp_state;	//��ʱ��������״̬��


	Vector2D vec_me;
	Vector2D vec_enemy;


	double min_distance;		//���������
	double min_rotation;		//�Ƕ������

	double danger_distance;		//Σ�վ���
	//const double min_distance=4;		//���������
	//const double min_rotation=1;		//�Ƕ������
	
	
	int myID;
	int enemyID;	//����ID�����˾֣�

	RobotAI_BattlefieldInformation battle_info;	//ս����Ϣ��������ʣ�ÿ֡����

	//arsenal
	Vector2D vec_arsenal0,vec_arsenal1;

	//temp
	bool moving;

	int to_point;
	vector<Vector2D> wandering_point;	//Ѳ�ߵ�

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

	

	//----------�ײ��������---------------------------

	void UpdateBattlefieldInfo(const RobotAI_BattlefieldInformation& info);
	void UpdateBulletInfoVector();
	int NearestBulletIdx(int);

	//***ֱ�Ӳٿ�***
	void Fire(RobotAI_Order&);	//����
	bool AimAtPoint(RobotAI_Order&,const double &x,const double &y);	//��׼�㣨x,y)


	void DirectMoveToPoint(RobotAI_Order&order,const double &x,const double &y);	//ֱ���ƶ�����(x,y)...
	void DirectMoveAwayFromPoint(RobotAI_Order&order,const double &x,const double &y);	//ֱ���뿪��(x,y)



	void DirectMoveToPoint_Spider(RobotAI_Order&,const double &x,const double&y);	//ֱ���ƶ�����(x,y)...֩������ӷ���
	void DirectMoveToPoint_Robotman(RobotAI_Order&,const double &x,const double&y);	//ֱ���ƶ�����(x,y)...������
	void DirectMoveToPoint_AFV(RobotAI_Order&,const double &x,const double&y);	//ֱ���ƶ�����(x,y)...ս��̹��
	void DirectMoveToPoint_UFO(RobotAI_Order&,const double &x,const double&y);	//ֱ���ƶ�����(x,y)...�ĸ�
	

	void DirectMoveInDirection(RobotAI_Order& order, const double & r);		//���ض����򣨽Ƕȣ��ƶ�
	
	void DirectMoveInDirection_Spider(RobotAI_Order& order, const double & r);
	void DirectMoveInDirection_Robotman(RobotAI_Order& order, const double & r);
	void DirectMoveInDirection_AFV(RobotAI_Order& order, const double & r);
	void DirectMoveInDirection_UFO(RobotAI_Order& order, const double & r);



	void Attack_ElectricSaw(RobotAI_Order&order);


	//�ؾ߳���Ŀ���
	//Ϊ̹�˷��񣬸�һ���ǶȲ���?
	int AdjustDirection(const double &x,const double &y);



	void DirectMoveAwayFromPoint_Spider(RobotAI_Order&order,const double &x,const double &y);	//ֱ���뿪��(x,y)...֩�����
	

	//�ط��߷�����������
	void EscapeFromBeam(RobotAI_Order&order,const Beam &b);

	//************


	//��΢�߲�һ���


	//���Լ���û�����ӵ��ķ���·����
	//return -1--��
	//return 0,1,2... �ȴ򵽵�i���ϰ���
	//return other ... �����ӵ�����·����
	int HitTestBulletBeam(const RobotAI_BulletInformation& bi);


	void PursuitEnemy(RobotAI_Order & order);

	//��Ԥ�е���׼��׼
	void AimWithAnticipation(RobotAI_Order & order,const double & x,const double & y);


	//���ٵ����ϰ����ʱ����׼
	void Aim_Missile(RobotAI_Order & order);


	//��ָ���ϰ�����hunter
	Vector2D GetHidingPosition(const Circle & o,const Circle & hunter);




	//�����

	void Hide(RobotAI_Order & order,const Circle & hunter);


	//Ѱ��ȥ����ⲹ�ӵ�
	void Reload(RobotAI_Order & order);	//ȥ���ӵ�

	//���ϰ�����ӵ�
	void HideAndApproach(RobotAI_Order & order);	

	//ʹ����Է������ϰ����ܱƽ�
	void SafeApproach(RobotAI_Order& order);

	//�ص�·���н�
	void PathFollow(RobotAI_Order& order);




	//***�ж�*****************************************
	bool IsArriveAtPoint(const double &x, const double &y);	//�ж��Ƿ񵽴��(x,y);

	//�ǲ��ǵ��˵��ӵ�
	bool IsEnemyBullet(int bullet_idx);

	//�������ڵ�ʣ���ӵ��ܲ��ܸɵ���
	bool IsSafeToRush();


	//����ֱͨ���ӵ��������Ƿ񿪻�·����ͨ���Ƿ�����׼
	bool IsOkToFire();


	//�ж��Ƿ񱻵�����׼��
	bool IsBeAimed();

	//************





	void AddWanderPoint(Vector2D v);



	//***
	//����ͼ
	void ConstructNavGraph();	//����battle_info��������ͼ

	bool ConstructNavGraph_ValidPosition(const Vector2D &);

	//���ؾ�������ĵ�����
	int NavGraph_NearestNode(const Vector2D &);








	//���ϵ��״̬execute����
	void Scared_HideAndFlee(RobotAI_Order & order);
	void Scared_Fire(RobotAI_Order & order);


	//�������״̬execute����
	
	void Crusher_Attack(RobotAI_Order & order);
	
	//�������״̬ת���б�
	void Crusher_StateTransform();



};


