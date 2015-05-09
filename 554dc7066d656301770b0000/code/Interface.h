#pragma once
#include "RobotAI.h"
#include "Vector.h"
#include "Coordinatetrans.h"
#include "Firecontrol.h"
#include "Frame.h"
#include <iostream>



using namespace std;

//������
class weapon {
public:
	weapontypename wp;//��������
	bullettypename bt1;
	bullettypename bt2;
	int damage1;//�����˺�
	int damage2;
	double error;//�������
	int cooling;//������ȴ
	int max;//���ҩ
	int remain;//ʣ�൯ҩ
	int speed;//�����ٶ�
	int rospeed;//����ת��
	void setweapon(const weapontypename& wptypename){};

	//weapon(const weapontypename& wp, bullettypename bt1, const bullettypename&bt2){
	//	damage1 = get_bullet_damage(bt1);

	//}

};


class Inface{
public:
	//ȷ�����ǵڼ�֡����Ϣ.
	int framenumber;
	int myID;//�ط��±�
	int enID;//�ҷ��±�

	//��������;
	weapon mywp;
	weapon enwp;

	Circle Me;//�ҵ�λ��
	Circle En;//�ط�λ��

	vec MEv;//λ������
	vec AC_MEv;//��̬��ʱ��λ������
	vec C_MEv;//��̬˳ʱ��λ������


	vec MWv;//����������
	vec AC_MWv;//��ʱ��������ƫת
	vec C_MWv;//˳ʱ��������ƫת


	vec EWv;//����������
	vec AC_EWv;//��ʱ�������ƫת
	vec C_EWv;//˳ʱ�������ƫת


	vec OBv[2];//�ϰ�������1
	vec OBv_AC[2];//�ϰ���1����ƫ������
	vec OBv_C[2];//�ϰ���1��˳ƫ������



};
//������,������,���� λ������
vec MEvec(const double ix, const	double iy, const double ex, const double ey){
	vec temp(ex - ix, ey - iy);

	temp.Unitization();
	return temp;
}
vec MEvec(Circle mc, Circle ec){
	vec temp(ec.x - mc.x, ec.y - mc.y);
	return temp;
}


//�����������Բ,����λ��λ��ƫ����:
vec AC_MEvec(const vec&MEv, Circle Me,Circle En){
	vec temp;
	double length = sqrt((En.x - Me.x)*(En.x - Me.x) + (En.y - Me.y)*(En.y - Me.y));
	temp.SetAngel(MEv.arg + asin(En.r / length )* 180 / PI,length);
	return temp;
}


//����λ˳ƫ����
vec C_MEvec(const vec&MEv, Circle Me, Circle En){
	vec temp;
	double length = sqrt((En.x - Me.x)*(En.x - Me.x) + (En.y - Me.y)*(En.y - Me.y));
	temp.SetAngel(MEv.arg - asin(En.r / length) * 180 / PI,length);
	return temp;
}




//��ʱerroΪ3



//���� �������Ƕ�,���� ��������λ����
vec MWvec(double angle){
	//cout << "��������λ�������캯������!" << endl;
	vec temp;
	temp.SetAngel(angle);
	//cout << "��������λ����:("<<temp.x<<","<<temp.y<<")"<<"�Ƕ�:"<<temp.arg << endl;
	//cout << "��������λ�������캯�����ý���" << endl;
	return temp;
}


//-------------ע��!!!!error������
//-------------
//-------------
//������������ƫ����,��Ҫ����
vec AC_MWvec(const vec& MWv, weapon& wp){//Ŀǰ����ȷ������ƫ�ƽǶȵ����Ƕ���
	vec temp;
	wp.error = 3;
	temp.SetAngel(MWv.arg + wp.error / 2);
	return temp;
}

//����������˳ƫ����,��Ҫ����
vec C_MWvec(const vec& MWv, weapon& wp){//Ŀǰ����ȷ������ƫ�ƽǶȵ����Ƕ���
	vec temp;
	wp.error = 3;
	temp.SetAngel(MWv.arg - wp.error / 2);
	return temp;
}










//�����ϰ�������//�����ǵ�λ����
vec OBvec(Circle ob, Circle me){
	//
	//cout << "�����ϰ����������캯��" << endl;
	vec temp(ob.x-me.x,ob.y-me.y);

	//cout << "�ϰ�������:("<<temp.x<<","<<temp.y<<")"<<"�Ƕ�:"<<temp.arg << endl;
	//cout << "�ϰ����������캯�����ý���" << endl;
	return temp;
}

//������ƫ�ϰ�������,��λ����
vec OB_ACvec(vec OBv, Circle ob){
	//cout << "������ƫ�ϰ�������" << endl;
	vec temp;
	double length = sqrt(OBv.length*OBv.length + ob.r*ob.r);
	temp.SetAngel(OBv.arg + asin(ob.r / OBv.length) * 180 / PI, length);
	//cout << "��ƫ�ϰ�������:(" << temp.x << "," << temp.y << ")" << "�Ƕ�:" << temp.arg << endl;
	//cout << "��ƫ�ϰ����������캯�����ý���" << endl;
	return temp;

}

//����˳ƫ�ϰ�������,��λ����
vec OB_Cvec(vec OBv, Circle ob){
	//cout << "����˳ƫ�ϰ�������" << endl;
	double length = sqrt(OBv.length*OBv.length + ob.r*ob.r);

	vec temp;
	temp.SetAngel(OBv.arg - asin(ob.r / OBv.length) * 180 / PI,length);
	//cout << "˳ƫ�ϰ�������:(" << temp.x << "," << temp.y << ")" << "�Ƕ�:" << temp.arg << endl;
	//cout << "˳ƫ�ϰ����������캯�����ý���" << endl;
	return temp;
}








//���� �������Ƕ�,���� ��������λ����
vec EWvec(double angle){
	vec temp;
	temp.SetAngel(angle);
	return temp;
}

//���ص�������ƫ����,��Ҫ����
vec AC_EWvec(const vec& EWv, const weapon& wp){//Ŀǰ����ȷ������ƫ�ƽǶȵ����Ƕ���
	vec temp;
	temp.SetAngel(EWv.arg + wp.error / 2);
	return temp;
}

//���ص�����˳ƫ����,��Ҫ����
vec C_EWvec(const vec& EWv, const weapon& wp){//Ŀǰ����ȷ������ƫ�ƽǶȵ����Ƕ���
	vec temp;
	temp.SetAngel(EWv.arg - wp.error / 2);
	return temp;
}
















//����任��ӿ�ˢ��:
void SetInface(const RobotAI_BattlefieldInformation&info,int myID,Inface&vset){
	//����ID
	vset.myID = myID;
	vset.enID = 1 - myID;

	//����˫������
	vset.mywp.setweapon(info.robotInformation[vset.myID].weaponTypeName);
	vset.enwp.setweapon(info.robotInformation[vset.enID].weaponTypeName);


	//����λ��
	vset.Me = info.robotInformation[vset.myID].circle;
	vset.En = info.robotInformation[vset.enID].circle;


	//����λ������
	vset.MEv = MEvec(vset.Me, vset.En);

	//����λ������
	vset.AC_MEv = AC_MEvec(vset.MEv, vset.Me,vset.En);
	vset.C_MEv = C_MEvec(vset.MEv, vset.Me,vset.En);



	//��������������;
	vset.MWv = MWvec(info.robotInformation[vset.myID].weaponRotation);
	//����������ƫ����;
	vset.AC_MWv = AC_MWvec(vset.MWv, vset.mywp);
	//˳ƫ
	vset.C_MWv = C_MWvec(vset.MWv, vset.mywp);





	//���µ���������:
	vset.EWv = EWvec(info.robotInformation[vset.enID].weaponRotation);

	//���µ�����ƫ
	vset.AC_EWv = AC_EWvec(vset.EWv, vset.enwp);
	//˳ƫ
	vset.C_EWv = AC_EWvec(vset.EWv, vset.enwp);


	//�����ϰ�������:
	//�ϰ�����ƫ:
	vset.OBv[0] = OBvec(info.obstacle[0], vset.Me);
	vset.OBv_AC[0] = OB_ACvec(vset.OBv[0], info.obstacle[0]);
	vset.OBv_C[0] = OB_Cvec(vset.OBv[0], info.obstacle[0]);

	vset.OBv[1] = OBvec(info.obstacle[1], vset.Me);
	vset.OBv_AC[1] = OB_ACvec(vset.OBv[1], info.obstacle[1]);
	vset.OBv_C[1] = OB_Cvec(vset.OBv[1], info.obstacle[1]);


}


//����任��ӿڸ���//...����.
void CoordinateAndInterface(const RobotAI_BattlefieldInformation&info, const int&myID, 
	RobotAI_BattlefieldInformation&RHinfo, Inface& vset)


{
	RHinfo = FieldInfoTrans(info);
	SetInface(RHinfo, myID, vset);
	return;
}