#pragma once
#include <cmath>
#include <struct.h>
#include <iostream>
using namespace std;






double ReturnPIAngel(double xx, double yy){
	double PIarg;
	if (xx >= 0 && yy >= 0){
		if (xx == 0 && yy == 0){
			PIarg = 0;
		}
		else
		PIarg = asin(yy / sqrt(xx*xx + yy*yy));
	}
	else if (xx<0 && yy>0)
		PIarg = PI - asin(yy/sqrt(xx*xx + yy*yy));
	else if (xx > 0 && yy < 0)
		PIarg = asin(yy / sqrt(xx*xx + yy*yy));
	else if (xx < 0 && yy < 0)
		PIarg = -PI - asin(yy / sqrt(xx*xx + yy*yy));
	return PIarg;
}


class vec
{
public:
	double x;
	double y;

	double length;//��������
	double arg;//�����Ƕ� -180��180
	double PIarg;//��������-PI��PI
	vec(double xx = 0, double yy = 0){
		x = xx;
		y = yy;
		length = sqrt(xx*xx + yy*yy);
		PIarg = ReturnPIAngel(xx, yy);
		arg = PIarg * 180 / PI;
	}//����xy���깹��

	void SetAngel(double aarg = 0,double len=1){//�ƺ�������?
		x = cos(aarg*PI/180);
		y = sin(aarg*PI/180);
		length = len;
		arg = aarg;
		PIarg = arg*PI / 180;
		//cout << "�����ǵ���setangelʱ������" << endl;
		//cout << "x=" << x << '\t' << "y=" << y << endl;
		//cout << "length=" << length << endl;
		//cout << "arg=" << arg << endl;
		//cout << "PIarg=" << PIarg << endl;
		//cout << "SetAngel���ý���" << endl;
		//cout << endl;

	}//�������Ⱥͷ��ǹ���
	void set(double xx = 0, double yy = 0){
		x = xx;
		y = yy;
		length = sqrt(xx*xx + yy*yy);
		PIarg = ReturnPIAngel(xx, yy);
		arg = PIarg * 180 / PI;
	}//����xy���깹��
	vec operator+(vec a);
	vec operator-(vec a);
	double operator*(vec a);
	void Unitization();//��λ������
	vec Returnvec(double x, double y);//�������귵����������
	double ClockwiseAngle(vec b)const;//��������a��b��˳ʱ��н�
	double AntiClockwiseAngle(vec b)const;//����a��b����ʱ��н�
	friend ostream& operator<<(ostream&, vec&);//------------���кܴ�����...


};//������

vec vec::operator+(vec a){
	return vec(x + a.x, y + a.y);
}//��������������ӷ�

vec vec::operator-(vec a){
	return vec(x - a.x, y - a.y);
}//�����������������

double vec::operator*(vec a){
	return x*a.x + y*a.y;
}//�����������



void vec::Unitization(){
	double len = length;
	x = x / len; 
	y = y / len;
	length = 1;

}//�����Ϊ��λ����.

vec vec::Returnvec(double x, double y){
	vec temp(x, y);
	return temp;
}//�������귵�������������

double vec::ClockwiseAngle(vec b)const{	
	//cout << endl;
	//cout << "�����ǵ���˳ʱ��нǼ��㺯��ʱ������" << endl;
	double Carg;
	if (arg - b.arg >= 0)
		Carg = arg - b.arg;
	else
		Carg = arg - b.arg + 360;

	//cout << "Carg=" << Carg << endl;
	//cout << "ClockwiseAngle���ý���" << endl; 
	return Carg;
}//����˳ʱ��н�

double vec::AntiClockwiseAngle(vec b)const{
	double ACarg;
	if (b.arg - arg >= 0)
		ACarg = b.arg - arg;
	else
		ACarg = b.arg - arg + 360;
	//cout << endl;
	//cout << "�����ǵ���˳ʱ��нǼ��㺯��ʱ������" << endl;
	//cout << "ACarg=" << ACarg << endl;
	//cout << "AntiClockwiseAngle���ý���" << endl;
	return ACarg;

}//������ʱ��н�





//���vec
ostream& operator<<(ostream&s, vec&v){
	return s << "(" << v.x << "," << v.y << ")" << '\t' << "�Ƕ�:" << v.arg << '\t' << "����:" << v.PIarg << endl;
}



