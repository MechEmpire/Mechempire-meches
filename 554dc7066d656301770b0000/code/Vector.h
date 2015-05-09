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

	double length;//向量长度
	double arg;//向量角度 -180到180
	double PIarg;//向量弧度-PI到PI
	vec(double xx = 0, double yy = 0){
		x = xx;
		y = yy;
		length = sqrt(xx*xx + yy*yy);
		PIarg = ReturnPIAngel(xx, yy);
		arg = PIarg * 180 / PI;
	}//给出xy坐标构造

	void SetAngel(double aarg = 0,double len=1){//似乎有问题?
		x = cos(aarg*PI/180);
		y = sin(aarg*PI/180);
		length = len;
		arg = aarg;
		PIarg = arg*PI / 180;
		//cout << "以下是调用setangel时候的输出" << endl;
		//cout << "x=" << x << '\t' << "y=" << y << endl;
		//cout << "length=" << length << endl;
		//cout << "arg=" << arg << endl;
		//cout << "PIarg=" << PIarg << endl;
		//cout << "SetAngel调用结束" << endl;
		//cout << endl;

	}//给出长度和幅角构造
	void set(double xx = 0, double yy = 0){
		x = xx;
		y = yy;
		length = sqrt(xx*xx + yy*yy);
		PIarg = ReturnPIAngel(xx, yy);
		arg = PIarg * 180 / PI;
	}//给出xy坐标构造
	vec operator+(vec a);
	vec operator-(vec a);
	double operator*(vec a);
	void Unitization();//单位化函数
	vec Returnvec(double x, double y);//输入坐标返回向量函数
	double ClockwiseAngle(vec b)const;//返回向量a到b的顺时针夹角
	double AntiClockwiseAngle(vec b)const;//返回a到b的逆时针夹角
	friend ostream& operator<<(ostream&, vec&);//------------还有很大问题...


};//向量类

vec vec::operator+(vec a){
	return vec(x + a.x, y + a.y);
}//返回向量运算符加法

vec vec::operator-(vec a){
	return vec(x - a.x, y - a.y);
}//返回向量运算符减法

double vec::operator*(vec a){
	return x*a.x + y*a.y;
}//返回向量点乘



void vec::Unitization(){
	double len = length;
	x = x / len; 
	y = y / len;
	length = 1;

}//改造成为单位向量.

vec vec::Returnvec(double x, double y){
	vec temp(x, y);
	return temp;
}//输入坐标返回坐标的向量；

double vec::ClockwiseAngle(vec b)const{	
	//cout << endl;
	//cout << "以下是调用顺时针夹角计算函数时候的输出" << endl;
	double Carg;
	if (arg - b.arg >= 0)
		Carg = arg - b.arg;
	else
		Carg = arg - b.arg + 360;

	//cout << "Carg=" << Carg << endl;
	//cout << "ClockwiseAngle调用结束" << endl; 
	return Carg;
}//返回顺时针夹角

double vec::AntiClockwiseAngle(vec b)const{
	double ACarg;
	if (b.arg - arg >= 0)
		ACarg = b.arg - arg;
	else
		ACarg = b.arg - arg + 360;
	//cout << endl;
	//cout << "以下是调用顺时针夹角计算函数时候的输出" << endl;
	//cout << "ACarg=" << ACarg << endl;
	//cout << "AntiClockwiseAngle调用结束" << endl;
	return ACarg;

}//返回逆时针夹角





//输出vec
ostream& operator<<(ostream&s, vec&v){
	return s << "(" << v.x << "," << v.y << ")" << '\t' << "角度:" << v.arg << '\t' << "弧度:" << v.PIarg << endl;
}



