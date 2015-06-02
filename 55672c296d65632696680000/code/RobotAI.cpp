#include "RobotAI.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>

#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <cmath>
#include <numeric>
#include <bitset>

#include <cstdio>
#include <cstring>
#define rep(i, n) for (int i = 0, _n = (int)(n); i < _n; i++)
#define fer(i, x, n) for (int i = (int)(x), _n = (int)(n); i < _n; i++)
#define rof(i, n, x) for (int i = (int)(n), _x = (int)(x); i-- > _x; )
#define fch(i, x) for (__typeof(x.begin()) i = x.begin(); i != x.end(); i++)
#define sz(x) (int((x).size()))
#define pb push_back
#define mkp make_pair
#define all(X) (X).begin(),(X).end()

#define X first
#define Y second

template<class T> inline void smin(T &a, T b){ if (b < a)a = b; }
template<class T> inline void smax(T &a, T b){ if (a < b)a = b; }
typedef pair<double, double> pdd;
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<pii> vii;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<vii> vvii;


typedef vector<char> VC;
typedef vector<string> VS;
typedef vector<ll> VL;
typedef vector<double> VD;
typedef set<int> SI;
typedef set<string> SS;
typedef map<int, int> MII;
typedef map<string, int> MSI;

template<class T> inline T min(T a, T b, T c){ return min(min(a, b), c); }
template<class T> inline T max(T a, T b, T c){ return max(max(a, b), c); }
template<class T> inline T min(T a, T b, T c, T d){ return min(min(a, b), min(c, d)); }
template<class T> inline T max(T a, T b, T c, T d){ return max(max(a, b), max(c, d)); }
template<class T> inline T sqr(T a){ return a*a; }
template<class T> inline T cub(T a){ return a*a*a; }

RobotAI::RobotAI()
{

}


RobotAI::~RobotAI()
{

}


//-----------------------------------------------------
//1.必须完成的战斗核心
//-----------------------------------------------------

int myID, enemyID, tmpID;
Circle me, he;
RobotAI_RobotInformation myInfo, enemyInfo;
Box boundary;
vector<Circle> enemyTrack;
vector<pdd> enemyVVector;

const double pi = acos(-1);
const double eps = 1e-3;
const double DotEps = 4;
const double SPD_RPG = 8.0;
static RobotAI_Order lastorder;
static int frame =0 ;
int lastChangeFrameId = 0;

static int defaultTurn = 1;
Circle obsLoc[2];
Circle obsLocCorner[2];

Circle locs[10000];
int loc;

Circle CircleDivide2(const Circle&a, const Circle &b){
	Circle ret;
	ret.x = (a.x + b.x) / 2.0;
	ret.y = (a.y + b.y) / 2.0;
	return ret;
}

//id编号robot的当前速率
double pureV(const RobotAI_BattlefieldInformation& info, int id = myID){
	return sqrt(sqr(info.robotInformation[id].vx) + sqr(info.robotInformation[id].vy));
}
//id编号robot的当前在l向量方向上的速率
double pureVL(const RobotAI_BattlefieldInformation& info, int id, pdd l){
	pdd tmp = mkp(info.robotInformation[id].vx, info.robotInformation[id].vy);
	tmp.X *= l.X; tmp.Y *= l.Y;
	return sqrt(sqr(tmp.X) + sqr(tmp.Y));
}

double pureV(const pdd v){
	return sqrt(sqr(v.X) + sqr(v.Y));
}

double vlength(const pdd v){
	return sqrt(sqr(v.X) + sqr(v.Y));
}

double speed_Bullet(weapontypename w){
	switch (w)
	{
	case WT_Cannon:
		return 11;
		break;
	case WT_Shotgun:
		return 10;
		break;
	case WT_RPG:
		return 8;
		break;
	case WT_Machinegun:
		return 12;
		break;
	case WT_Prism:
		return 5;
		break;
	case WT_Tesla:
		return 5;
		break;
	case WT_PlasmaTorch:
		return 5;
		break;
	case WT_MissileLauncher:
		return 4;
		break;
	case WT_ElectricSaw:
		return 10;
		break;
	case WT_GrenadeThrower:
		return 6;
		break;
	case WT_MineLayer:
		return 0.00001;
		break;
	case WT_Apollo:
		return 14;
		break;
	default:
		return 5;
		break;
	}
}
double speed_Engine(enginetypename engine){
	switch (engine)
	{
	case ET_Spider:
		return 4;
		break;
	case ET_Robotman:
		return 5;
		break;
	case ET_AFV:
		return 6;
		break;
	case ET_UFO:
		return 6;
		break;
	case ET_Quad:
		return 4;
		break;
	case ET_GhostTank:
		return 6;
		break;
	case ET_XCraft:
		return 6;
		break;
	case ET_Shuttle:
		return 1;
		break;
	default:
		break;
	}
}




pdd fly_engine(const RobotAI_BattlefieldInformation& info, int id, double t){
	pdd ret;
	ret.X = info.robotInformation[id].circle.x;
	ret.Y = info.robotInformation[id].circle.y;
	ret.X += info.robotInformation[id].vx * t;
	ret.Y += info.robotInformation[id].vy * t;
	return ret;
}

double gun_radius(weapontypename w){
	switch (w)
	{
	case WT_Cannon:
		return 95;
		break;
	case WT_Shotgun:
		return 65;
		break;
	case WT_RPG:
		return 85;
		break;
	case WT_Machinegun:
		return 69;
		break;
	case WT_Prism:
		return 76;
		break;
	case WT_Tesla:
		return 45;
		break;
	case WT_PlasmaTorch:
		return 75;
		break;
	case WT_MissileLauncher:
		return 50;
		break;
	case WT_ElectricSaw:
		return 95;
		break;
	case WT_GrenadeThrower:
		return 50; 
		break;
	case WT_MineLayer:
		return 50;
		break;
	case WT_Apollo:
		return 100;
		break;
	default:
		return 50;
		break;
	}
}


inline double fpito180(double x){
	return x * 180 / pi;
}
inline double f180topi(double x){
	AngleAdjust(x);
	return x*pi / 180.0;
}


bool doubleEqual(double a, double b){
	return (fabs(a - b) < 1e-5);
}
double dis(const Circle &a, const Circle &b){
	return sqrt( sqr(a.x-b.x)+sqr(a.y-b.y) );
}
double dis(const pdd &a, const Circle &b){
	return sqrt(sqr(a.X - b.x) + sqr(a.Y - b.y));
}
double dis(const Circle &a, const pdd &b){
	return sqrt(sqr(a.x - b.X) + sqr(a.y - b.Y));
}
double dis(const pdd &a, const pdd &b){
	return sqrt(sqr(a.X - b.X) + sqr(a.Y - b.Y));
}


double timeCalc(weapontypename w, double dis){
	switch (w)
	{
	case  WT_RPG:
		return (-speed_Bullet(WT_RPG) + sqrt(sqr(speed_Bullet(WT_RPG)) + 2 * 1.05*dis)) / 1.05;
		break;

	case WT_Cannon:
		return dis / speed_Bullet(WT_Cannon);
		break;
	default:
		return 1;
		break;
	}
}

pdd divid(const pdd a, const double b){
	if (fabs(b) > 1e-5)
		return mkp(a.X / b, a.Y / b);
	return mkp(0, 0);
}
pdd multi(const pdd a, const double b){
	return mkp(a.X * b, a.Y * b);
}
pdd multi(const pdd a, const pdd b){
	return mkp(a.X * b.X, a.Y * b.Y);
}
pdd add(const pdd a, const pdd b){
	return mkp(a.X + b.X, a.Y + b.Y);
}
pdd minus(const pdd a, const pdd b){
	return mkp(a.X - b.X, a.Y - b.Y);
}
pdd counterClockRotatePI(const pdd a, double A){
	return mkp(a.X*cos(A) - a.Y*sin(A), a.X*sin(A) + a.Y*cos(A));
}
pdd clockRotatePI(const pdd a, double A){
	return mkp(a.X*cos(A) + a.Y*sin(A), -a.X*sin(A) + a.Y*cos(A));
}
pdd counterClockRotate180(const pdd a, double A){
	AngleAdjust(A);
	A = f180topi(A);
	return mkp(a.X*cos(A) - a.Y*sin(A), a.X*sin(A) + a.Y*cos(A));
}
pdd clockRotate180(const pdd a, double A){
	AngleAdjust(A);
	A = f180topi(A);
	return mkp(a.X*cos(A) + a.Y*sin(A), -a.X*sin(A) + a.Y*cos(A));
}
bool inBox(double x, double y, double d = 0){
	return x >= 0+d && x <= 1366-d && y >= 0+d && y <= 680-d;
}
bool inBox(Circle c, double d = 0){
	return inBox(c.x, c.y, d);
}
bool inBox(pdd c, double d = 0){
	return inBox(c.X, c.Y, d);
}

//位置x,y是否在场地最外面d宽度那一圈范围内
bool inO(double x, double y, double d = me.r * 2){
	return x >= 0 && x <= d || x >= 1366 - d && x <= 1366 || y >= 0 && y <= d || y >= 680 - d && y <= 680;
}

//位置c是否在某个障碍物外面d的范围内. 返回碰上的obs的编号
int inObs(pdd c, double d = 0){
	rep(i, 2) if (dis(c, obsLoc[i]) <= obsLoc[i].r + d + eps) return i;
	return -1;
}
int inObs(Circle c, double d = 0){
	rep(i, 2) if (dis(c, obsLoc[i]) <= obsLoc[i].r + d + eps) return i;
	return -1;
}
int inObs(double x, double y, double d = 0){
	return inObs(mkp(x, y), d);
}

double rot(const Circle &from, const Circle &to){
	double ret = atan2(to.y - from.y, to.x - from.x) * 180. / pi;
	AngleAdjust(ret);
	return ret;
}

double rotVPure180(const pdd &from, const pdd &to){
	return fpito180(acos((from.X*to.X + from.Y*to.Y) / (pureV(from)*pureV(to))));
}
double rotV180(const pdd &from, const pdd &to){
	double sz = rotVPure180(from, to);
	pdd tmp = counterClockRotate180(from, 90);
	if (rotVPure180(tmp, to) < 90){
		sz = -sz;
	}
//	printf("rotV180 sz:%.0lf, tmp:(%.0lf,%.0lf) from:(%.0lf,%.0lf), to:(%.0lf,%.0lf)\n", sz, tmp.X, tmp.Y,from.X,from.Y,to.X,to.Y);
	return sz;
}



int trackinObs(Circle from, Circle to, double d = 0){
	pdd v;
	pdd tmp;
	int flag;
	v.X = to.x - from.x; v.Y = to.y - from.y;
	int splitNum = 100;
	rep(i, splitNum+1){
		tmp.X = from.x + v.X*i / splitNum;
		tmp.Y = from.y + v.Y*i / splitNum;
		if ((flag = inObs(tmp, d)) != -1)
			return flag;
	}
	return -1;
}

pdd fly_Bullet(bullettypename w, pdd f, pdd v, double t){
	switch (w)
	{
	case BT_RPGBall:
		v = multi(v, (pureV(v)*t + 0.5*1.05*sqr(t)) / vlength(v));
		return mkp(f.X + v.X, f.Y + v.Y);
		break;
	default:
		return mkp(f.X + v.X*t, f.Y + v.Y*t);
		break;
	}
}

pdd fly_Bullet(bullettypename w, Circle f, pdd v, double t){
	return fly_Bullet(w, mkp(f.x, f.y), v, t);
}

bool bulletWillHit(const RobotAI_BattlefieldInformation& info, RobotAI_BulletInformation &bullet, int id = myID){
	if (fabs(bullet.vx) < eps && fabs(bullet.vy) < eps) return false;
	for (double t = 0; t < 500; t += 0.5){
		pdd engine = fly_engine(info, id, t);
		pdd nxtbull = fly_Bullet(bullet.type, bullet.circle, mkp(bullet.vx, bullet.vy), t);
		if (dis(engine, nxtbull) < info.robotInformation[id].circle.r)
			return true;
	}
	return false;
}

int bulletHitNum(const RobotAI_BattlefieldInformation& info, int id = myID){
	//123
		set<int> s;
	for (double t = 0; t < 500; t += 0.5){
		pdd engine = fly_engine(info, id, t);
		rep(i, info.num_bullet){
			if (info.bulletInformation[i].launcherID == id) continue;
			RobotAI_BulletInformation bullet = info.bulletInformation[i];
			pdd nxtbull = fly_Bullet(bullet.type, bullet.circle, mkp(bullet.vx, bullet.vy), t);

			if (dis(engine, nxtbull) < info.robotInformation[id].circle.r){
				s.insert(i);
			}
		}
	}
	return (int)s.size();
}


int obsAble(const Circle &me, const Circle &he, double wrotation, const Circle obs[]){
	double hedis = dis(me, he);
	double wrot = wrotation;
	AngleAdjust(wrot);
	rep(i, 2){
		if (dis(me, obsLoc[i]) <= obsLoc[i].r+0.5)
			return 0;
		double obsrot = rot(me, obs[i]);
		double obsdis = dis(me, obs[i]);
		double halfrot = asin(obs[i].r*1.0 / obsdis)*180.0 / pi;
		double da1, da2;
		da1 = wrotation - (obsrot - halfrot - 1.5); AngleAdjust(da1);
		da2 = wrotation - (obsrot + halfrot + 1.5); AngleAdjust(da2);

		if (da1 > eps && da2<-eps && hedis > obsdis)
			return 0;
		// 		double d = dis(me, obs[i]);
		// 		double obsrot = rot(me, obs[i]);
		// 		AngleAdjust(obsrot);
		// 		double drot = obsrot - wrot;
		// 		AngleAdjust(drot);
		// 		double l = fabs( sin(f180topi(drot)) *d);
		// 		if (l <= obs[i].r+2 && d < hedis)
		// 			return 0;
	}
	return 1;
}

Circle enemyNxtLoc(int t){
	Circle ret;
	int l = enemyVVector.size();
	int frameInterval = 35;
	int num = 1;
	pdd v = mkp(0, 0);
	

	if (t < frameInterval){
		frameInterval = t;
		num = 1;
	}
	else{
		num = t / frameInterval+0.5;
	}

	if (l < t){
		ret.x = enemyTrack[l - 1].x + enemyVVector[l - 1].X*t;
		ret.y = enemyTrack[l - 1].y + enemyVVector[l - 1].Y*t;
		return ret;
	}
	
	v.X = enemyTrack[l - 1].x - enemyTrack[l - 1 - frameInterval].x;
	v.Y = enemyTrack[l - 1].y - enemyTrack[l - 1 - frameInterval].y;
	double szRot, len = pureV(v), multilen = 1;
	pdd initV;
	if (pureV(v) < eps){
		return enemyTrack[l - 1];
	}
	if (pureV(enemyVVector[l - 1 - frameInterval]) < eps){
		ret.x = he.x; ret.y = he.y;
		rep(i, num){
			ret.x += v.X; ret.y += v.Y;
		}
		return ret;
	}
	else{
		szRot = rotV180(enemyVVector[l - 1 - frameInterval], v);		
		if (pureV(mkp(enemyInfo.vx, enemyInfo.vy)) < eps){
			int i = 1;
			for (; i < frameInterval; i++){
				if (enemyTrack[l - 1 - i].x != enemyTrack[l - 1].x || enemyTrack[l - 1 - i].y != enemyTrack[l - 1].y){
					break;
				}
			}
			if (i == frameInterval){
				ret.x = he.x; ret.y = he.y;
				rep(i, num){
					ret.x += v.X; ret.y += v.Y;
				}
				return ret;
			}
			else{
				initV = mkp(he.x - enemyTrack[l - 1 - i].x, he.y - enemyTrack[l - 1 - i].y);
			}
		}
		else{
			initV = mkp(enemyInfo.vx, enemyInfo.vy);
		}

		rep(i, num){
			initV = clockRotate180(initV, szRot);
		}
		initV = multi(initV, len / pureV(initV));
		if (pureV(mkp(myInfo.vx, myInfo.vy)) > eps && pureV(mkp(enemyVVector[l - 1 - frameInterval].X, enemyVVector[l - 1 - frameInterval].Y)) > eps)
			initV = multi(initV, pureV(mkp(myInfo.vx, myInfo.vy)) / pureV(mkp(enemyVVector[l - 1 - frameInterval].X, enemyVVector[l - 1 - frameInterval].Y)));
		ret.x = he.x+initV.X; ret.y = he.y+initV.Y;
		return ret;
	}
}

Circle hitLoc(const Circle tar, double vx, double vy, const Circle s, weapontypename w){
	double cureps = 10000;
	double curTime;
	Circle ret, tmp;
	for (double step = 0; step < 1500; step += 0.1){
		tmp.x = tar.x + step*vx;
		tmp.y = tar.y + step*vy;
		double t = timeCalc(w, dis(tmp, s) - gun_radius(w));
		if (fabs(t - step) < cureps){
			cureps = fabs(t - step);
			ret = tmp;
			curTime = step;
		}
		if (fabs(t - step) < 0.1){
			return tmp;
		}
	}
	return ret;
}

Circle hitLoc2(const Circle tar,  const Circle s, weapontypename w){
	double cureps = 10000;
	double curTime;
	Circle ret, tmp;
	for (int step = dis(me, he) / (speed_Bullet(w) + speed_Engine(enemyInfo.engineTypeName))-0.5; step < dis(me, he) / (speed_Bullet(w) - speed_Engine(enemyInfo.engineTypeName)) + 1.5; step += 1){
		tmp = enemyNxtLoc(step);
		double t = timeCalc(w, dis(tmp, s) - gun_radius(w));
		if (fabs(t - step) < cureps){
			cureps = fabs(t - step);
			ret = tmp;
			curTime = step;
		}
		if (fabs(t - step) < 0.1){
			return tmp;
		}
	}
	return ret;

}


Circle addGunLen(const RobotAI_BattlefieldInformation& info, int id=myID){
	Circle ret = info.robotInformation[id].circle;
	double len = gun_radius(info.robotInformation[id].weaponTypeName);
	double sz = f180topi(info.robotInformation[id].weaponRotation);
	pdd v;
	v.X = cos(sz)*len; v.Y = sin(sz)*len;
	ret.x += v.X; ret.y += v.Y;
	return ret;
}

//计算开火方向, 是否要开火
int calcFire(const RobotAI_BattlefieldInformation& info, int &wturn, int &fireornot){
	Circle nxt;
	//Circle nxt = hitLoc(he, enemyInfo.vx, enemyInfo.vy, me, myInfo.weaponTypeName);
	if (dis(me, he) <= he.r + gun_radius(myInfo.weaponTypeName) + eps)
		nxt = he;
	else nxt = hitLoc2(he, me, myInfo.weaponTypeName);
	int i = 0;
	if (!inBox(nxt, he.r)){
		// 		if (nxt.x < he.r) nxt.x = he.r + he.r - nxt.x;
		// 		if (nxt.x > boundary.width - he.r) nxt.x = boundary.width - he.r - (nxt.x - (boundary.width - he.r));
		// 		if (nxt.y < he.r) nxt.y = he.r + (he.r - nxt.y);
		// 		if (nxt.y > boundary.height - he.r) nxt.y = (boundary.height - he.r) - (nxt.y - (boundary.height - he.r));
		double exceed = 0;
		double x = nxt.x, y = nxt.y;
		if (enemyInfo.engineTypeName == ET_UFO){
			if (nxt.x < he.r) x = he.r*2, smax(exceed, he.r - nxt.x);
			if (nxt.x > boundary.width - he.r) x = boundary.width - he.r*2, smax(exceed, (nxt.x - (boundary.width - he.r)));
			if (nxt.y < he.r) y = he.r*2, smax(exceed, he.r - nxt.y);
			if (nxt.y > boundary.height - he.r) y = boundary.height - he.r*2, smax(exceed, (nxt.y - (boundary.height - he.r)));
		}else {
			if (nxt.x < he.r) x = he.r, smax(exceed, he.r - nxt.x);
			if (nxt.x > boundary.width - he.r) x = boundary.width - he.r, smax(exceed, (nxt.x - (boundary.width - he.r)));
			if (nxt.y < he.r) y = he.r, smax(exceed, he.r - nxt.y);
			if (nxt.y > boundary.height - he.r) y = (boundary.height - he.r), smax(exceed, (nxt.y - (boundary.height - he.r)));
		}

		printf("calcFire!inBox: nxt:(%.2lf,%.2lf),  calc:(%.2lf,%.2lf)  exceed:%.2lf\n", nxt.x, nxt.y, x, y,exceed);

		nxt.x = x; nxt.y = y;
		exceed /= 2;
		pdd v;
		v.X = me.x - nxt.x, v.Y = me.y - nxt.y;
		v = multi(v, exceed / dis(nxt, me));
		nxt.x += v.X; nxt.y += v.Y;
	}
	else if ((i = trackinObs(he,nxt, he.r/2)) != -1){
		printf("trackinObs  cur:(%.2lf,%.2lf),  nxt:(%.2lf,%.2lf)\n", he.x,he.y,nxt.x, nxt.y);

		if (dis(obsLoc[i], he) < dis(nxt, he)){
			pdd v;
			v.X = nxt.x - obsLoc[i].x; v.Y = nxt.y - obsLoc[i].y;
			while (inObs(nxt, he.r) != -1){
				nxt.x += v.X; nxt.y += v.Y;
			}
		}
		else{
			pdd v,v1,v2;
			v1.X = obsLoc[i].x - nxt.x; v1.Y = obsLoc[i].y - nxt.y;
			v2.X = he.x - nxt.x; v2.Y = he.y - nxt.y;
			v = multi(v1, v2);
			double cosin = (v1.X*v2.X + v1.Y*v2.Y) / (vlength(v1)*vlength(v2));
			double d = cosin*dis(nxt, obsLoc[i]);
			v1 = multi(v1, -1);
			v2 = multi(v2, d / dis(he, nxt));
			v = add(v1, v2);
			double len;
			if (enemyInfo.engineTypeName == ET_UFO) len = he.r*2;
			else len = he.r;
			while (inObs(nxt, len) != -1){
				nxt.x += v.X; nxt.y += v.Y;
			}
		}
		printf("calcFire!inObs: nxt:(%.2lf,%.2lf), now(%.2lf,%.2lf) \n", nxt.x, nxt.y, he.x,he.y);
	}
	
	double sz = rot(me,nxt) - myInfo.weaponRotation;
	AngleAdjust(sz);
	fireornot = 0;
	//double drot = 3.95;
	double drot = 2.75;
	if (sz > drot) wturn = 1;
	else if (sz < -drot) wturn = -1;
	else{
	//	if (info.robotInformation[myID].hp<=35) printf("calcFire case3:  sz:%.2lf\n", sz);
	//	if (fabs(sz) <= 1.8)
		fireornot = obsAble( addGunLen(info,myID) , nxt, myInfo.weaponRotation, info.obstacle);
		if (dis(me, he) <= gun_radius(myInfo.weaponTypeName) + he.r) fireornot = 1;
		wturn = 0;
		if (fireornot == 1 && (dis(me, he) < me.r + he.r + 30)){
			if (info.robotInformation[myID].hp < info.robotInformation[enemyID].hp && info.robotInformation[enemyID].hp>35)
				fireornot = 0;
		}
	}
	return 0;
}


// void go(double x, double y, const RobotAI_BattlefieldInformation& info, int &eturn, int &run) {
// 	auto me = info.robotInformation[myID];
// 	double dx = x - me.circle.x;
// 	double dy = y - me.circle.y;
// 	double theta = atan2(dy, dx)*180.0 / pi;
// 	double mt = me.engineRotation;
// 	double dt = theta - mt;
// 	AngleAdjust(dt);
// 
// 	if (dt > eps){
// 		eturn = 1;
// 	}
// 	else if (dt < -eps){
// 		eturn = -1;
// 	}
// 	else {
// 		eturn = 0;
// 	}
// 	run = 1;
// }

static Circle lastCenter;
static pdd lastAim;
static bool outCircle;
void goCircle(const Circle &center, const RobotAI_BattlefieldInformation& info, RobotAI_Order& order, double r = 0, int turn=defaultTurn, bool history=true ){
	if (fabs(r) < eps) r = dis(center, me);

	if (outCircle && dis(lastAim, me) < 10 || center.x!=lastCenter.x || center.y!=lastCenter.y)
		outCircle = false;
	
	pdd aim;
	if (outCircle && history){
		aim = lastAim;
	}
	else{
		double toc = dis(me, center);
		double sz = 10;
		if (fabs(dis(me, center) - r) > 1.2*r + eps){
			sz = acos(r*1.0 / toc)*180.0 / pi;
			AngleAdjust(sz);
			outCircle = true;
		}
		double a, b;
		a = cos(f180topi(sz))*r; b = sin(f180topi(sz))*r;
		pdd v1, v2;
		v1 = mkp(me.x - center.x, me.y - center.y);
		v1 = multi(v1, a / toc);
		v2 = multi(v1, b / a);
		v2 = clockRotate180(v2, turn * 90);
		pdd aim = add(add(mkp(center.x, center.y), v1), v2);
		aim = add(add(mkp(center.x, center.y), v1), v2);
		lastAim = aim;
		lastCenter = center;
	}

	double x = aim.X, y = aim.Y;
	double dx = x - me.x;
	double dy = y - me.y;
	double theta = atan2(dy, dx)*180.0 / pi;
	double mt = info.robotInformation[myID].engineRotation;
	double dt = theta - mt;
	AngleAdjust(dt);

	if (dt > eps) order.eturn = 1;
	else if (dt < -eps) order.eturn = -1;
	else  {
		order.eturn = 0;
		order.run = 1;
	}
}

void goCircleWithoutHistory(const Circle &center, const RobotAI_BattlefieldInformation& info, RobotAI_Order& order, double r = 0, int turn = defaultTurn){
	outCircle = false;
	if (fabs(r) < eps)
		r = dis(center, me);

	double toc = dis(me, center);
	double sz = 10;
	if (fabs(dis(me, center) - r) > 1.2*r + eps){
		sz = acos(r*1.0 / toc)*180.0 / pi;//?
		AngleAdjust(sz);
	}
	double a, b;
	a = cos(f180topi(sz))*r; b = sin(f180topi(sz))*r;
	pdd v1, v2;
	v1 = mkp(me.x - center.x, me.y - center.y);
	v1 = multi(v1, a / toc);
	v2 = multi(v1, b / a);
	v2 = clockRotate180(v2, turn*90);
	pdd aim = add(add(mkp(center.x, center.y), v1), v2);

	double x = aim.X, y = aim.Y;
	double dx = x - me.x;
	double dy = y - me.y;
	double theta = atan2(dy, dx)*180.0 / pi;
	double mt = info.robotInformation[myID].engineRotation;
	double dt = theta - mt;
	AngleAdjust(dt);

	if (dt > 3) order.eturn = 1;
	else if (dt < -3) order.eturn = -1;
	else
		order.eturn = 0,
		order.run = 1;
	
	printf("goCircleWithoutHistory_Aim.x,y :(%.0lf,%.0lf) dt:%.2lf  Hp:%d\n", x, y, dt, info.robotInformation[myID].hp);
}


// void calcLoc(const RobotAI_BattlefieldInformation& info,int &eturn, int &run){
// 	if (loc < 2){
// 		if (dis(info.robotInformation[myID].circle, locs[loc]) < info.robotInformation[myID].circle.r / 2)
// 			loc++;
// 	}
// 	
// 	
// }
// 


void init(const RobotAI_BattlefieldInformation& info, int id){
	myID = id; enemyID = 1 - id;
	me = info.robotInformation[myID].circle;
	he = info.robotInformation[enemyID].circle;
	myInfo = info.robotInformation[myID];
	enemyInfo = info.robotInformation[enemyID];

	enemyTrack.push_back(info.robotInformation[enemyID].circle);
	enemyVVector.push_back(mkp(info.robotInformation[enemyID].vx, info.robotInformation[enemyID].vy));
// 	if (enemyTrack.size() >= 200){
// 		enemyTrack.erase(enemyTrack.begin(), enemyTrack.begin() + 100);
// 	}
}

// void TurnCircle(const Circle &center, const RobotAI_BattlefieldInformation& info, RobotAI_Order& order, int turn, double r=0){
// 	if (fabs(r) < eps) r = dis(center, info.robotInformation[myID].circle);
// 
// 	const RobotAI_RobotInformation me = info.robotInformation[myID];
// 	double dir = atan2(center.y - me.circle.y, center.x - me.circle.x)*180.0 / pi - turn * 90;
// 	AngleAdjust(dir);
// 	double sz = dir - me.engineRotation;
// 	AngleAdjust(sz);
// 	
// 	if (sz > eps){
// 		order.eturn = 1;
// 	}
// 	else if (sz < -eps){
// 		order.eturn = -1;
// 	}
// 	else{
// 		order.eturn = 0;
// 	}
// 	order.run = 1;
// }

bool hasObsBetween(const RobotAI_BattlefieldInformation& info,const Circle &aim, Circle &retcircle){	
	bool flag = false;
	Circle ret;
// 	rep(i, 2){
// 		Circle obs = info.obstacle[i];
// 		double meobs = dis(me, obs);
// 		double sz = fabs(rot(me, he) - rot(me, obs));
// 		AngleAdjust(sz);
// 		double l = sin(f180topi(sz))*meobs;
// 		if (l < obs.r){
// 			if (!flag || (flag && dis(ret, he) > dis(obs, he))){
// 				ret = obs;
// 				flag = true;
// 			}
// 		}
// 	}
	rep(i, 2){
		Beam b1, b2;
		double rotMeHe = rot(me, aim);
		b1.rotation = b2.rotation = rotMeHe;
		pdd v1, v2, v;
		v.X = aim.x - me.x; v.Y = aim.y - me.y;
		v = multi(v, me.r / dis(aim, me));
		v1 = counterClockRotate180(v, 90);
		v2 = clockRotate180(v, 90);
		b1.x = me.x + v1.X; b1.y = me.y + v1.Y;
		b2.x = me.x + v2.X; b2.y = me.y + v2.Y;
		if (dis(info.obstacle[i], me) < dis(me, aim) && (HitTestBeamCircle(b1, info.obstacle[i]) || HitTestBeamCircle(b2, info.obstacle[i]))){
			if (!flag || (flag && dis(info.obstacle[i], me) < dis(ret, me))){
				ret = info.obstacle[i];
				flag = true;
			}		
		}
	}

	if (flag){
		retcircle = ret;
		return flag;
	}
	return false;
}

void adjust(const RobotAI_BattlefieldInformation& info, Circle &c){
	Circle ret = c;
	ret.x += enemyInfo.vx; ret.y += enemyInfo.vy;
	if (!inBox(ret, he.r)){
		ret.x = (me.x + he.x) / 2;
		ret.y = (me.y + he.y) / 2;
	}
}

int selectObs(const RobotAI_BattlefieldInformation& info){
	if (he.x <= 300 && he.y <= 250) return 1;
	if (he.x >= 1066 && he.y >= 430) return 0;
	rep(i, 2){
		double dis_me2obsi = dis(obsLoc[i], me), dis_he2obsi = dis(obsLoc[i], he);
		if (dis_me2obsi < dis_he2obsi) return i;
	}
	return myID;
}

int DefendThenAttack(const RobotAI_BattlefieldInformation& info, RobotAI_Order& order){

	return 1;
}
//pureVL(info,myID, counterClockRotate180(divid( mkp(he.x-me.x,he.y-me.y),dis(me,he)),90) )
int AttackWithDefend(const RobotAI_BattlefieldInformation& info, RobotAI_Order& order){
	//goCircle(info.robotInformation[enemyID].circle, info,order, 30+he.r+(speed_Bullet( info.robotInformation[enemyID].weaponTypeName )*46.0*2/6 ) );
	
	Circle obs;
	//obs.x = 0; obs.y = 0; obs.r = 0;
	int decide = 0;
	Circle aim;

	if (hasObsBetween(info, he, obs) && enemyInfo.weaponTypeName != WT_ElectricSaw  && dis(me, obs)<60+me.r+obs.r ){
		//if (fabs(obs.r - info.obstacle[1].r)<=eps)
		//goCircle(obs, info, order, obs.r + me.r, defaultTurn);
		
		goCircleWithoutHistory(obs, info, order, (obs.r + me.r), defaultTurn);
		//goCircleWithoutHistory(obs, info, order, obs.r + me.r, defaultTurn);
		//printf("AttackWithDefend_%d: 1 now-x:%.0lf,y:%.0lf  obs.x,y:%.2lf %.2lf\n", frame, me.x,me.y, obs.x, obs.y);
	}
	else{
		double circleR =// 450;
			30 + he.r + me.r + (speed_Bullet(enemyInfo.weaponTypeName)*46.0 * 2 / 6 + speed_Engine(enemyInfo.engineTypeName)*46.0 * 2 / 6);
		Circle aim = enemyInfo.circle;

		if (enemyInfo.weaponTypeName == WT_RPG){
			circleR /= 100;
		}
		if (enemyInfo.weaponTypeName == WT_Machinegun){
			adjust(info, aim);
		}
		if (enemyInfo.weaponTypeName == WT_ElectricSaw){
			circleR = 500;
		}
// 		adjust(info, aim);
// 
// 		if (dis(he, me) < 1.5*circleR && pureVL(info, enemyID, mkp(me.x - he.x, me.y - he.y)) > pureVL(info, myID, mkp(me.x - he.x, me.y - he.y))){
// 			if (frame - lastChangeFrameId > 20){
// 				lastChangeFrameId = frame;
// 				defaultTurn *= -1;
// 			}
// 			puts("Dis<1.5R");
// 		}
// 		else if (dis(he, me) < circleR && pureVL(info, enemyID, mkp(me.x - he.x, me.y - he.y)) > pureVL(info, myID, mkp(me.x - he.x, me.y - he.y))){
// 			int tmp = selectObs(info);
// 			aim = obsLocCorner[tmp];
// 			puts("Dis<1R");
// 		}
		goCircleWithoutHistory(aim, info, order, circleR, defaultTurn);
	//	printf("AttackWithDefend %d: now-x:%.0lf,y:%.0lf  aim:(%.0lf,%.0lf) Circle size: %.2lf but dis:%.0lf\n", frame, me.x, me.y,aim.x,aim.y, circleR, dis(he, me));
	}
 	
// 	if (enemyInfo.weaponTypeName != WT_Prism && enemyInfo.weaponTypeName != WT_Tesla
// 		&& enemyInfo.weaponRotation != WT_ElectricSaw
// 		&& bulletHitNum(info, myID) >= 1){
// 		goCircleWithoutHistory(, info, order, 0.1);
// 		printf("changeTurn_Change Turn!!! bulletHitNum:%d  hp:%d\n", bulletHitNum(info, myID), info.robotInformation[myID].hp);
// 	}

	if (info.robotInformation[myID].remainingAmmo == 0){
		printf("Arsenal:%d: %.2lf,%.2lf\n", myID, info.arsenal[myID].circle.x, info.arsenal[myID].circle.y);
		goCircleWithoutHistory(info.arsenal[myID].circle, info, order, 0.1);
	}
	
	return 1;
}


RobotAI_BulletInformation getNearestBullet(const RobotAI_BattlefieldInformation& info){
	RobotAI_BulletInformation ret;
	ret.vx = 0; ret.vy = 0;
	double tmp = 2000;
	rep(i, info.num_bullet){
		if (info.bulletInformation[i].launcherID != enemyID) continue;
		if (dis(info.bulletInformation[i].circle, me) < tmp){
			tmp = dis(info.bulletInformation[i].circle, me);
			ret = info.bulletInformation[i];
		}
	}
	return ret;
}



void changeTurn(const RobotAI_BattlefieldInformation& info){
	int frameInterval = 60;
	int frameNum = 10;
	if (frame - lastChangeFrameId <= frameInterval) return;
	double nx, ny;
	nx = me.x + info.robotInformation[myID].vx * frameNum;
	ny = me.y + info.robotInformation[myID].vy * frameNum;
	if (inO(me.x, me.y) && !inBox(nx, ny)){
		lastChangeFrameId = frame;
		defaultTurn *= -1;
		return;
	}

	
}

// void Aimto(const double aim, const RobotAI_BattlefieldInformation& info, int &eturn){
// 	double sz = aim - info.robotInformation[myID].weaponRotation;
// 	AngleAdjust(sz);
// 
// 	if (sz > eps){
// 		eturn = 1;
// 	}
// 	else if (sz < -eps){
// 		eturn = -1;
// 	}
// 	else{
// 		eturn = 0;
// 	}
// }

void rotateTest(const RobotAI_BattlefieldInformation& info, RobotAI_Order& order){
	Circle c;
	if (dis(me, obsLoc[0]) < dis(me, obsLoc[1])) c = obsLoc[0];
	else c = obsLoc[1];
	double r = obsLoc[0].r + myInfo.circle.r + 3;
	goCircleWithoutHistory(c, info, order, r);
	if (frame % 200 == 0){
		order.fire = 1;
		defaultTurn *= -1;
	}
	else{
		order.fire = 0;
	}
}

void RobotAI::Update(RobotAI_Order& order,const RobotAI_BattlefieldInformation& info,int myID)
{
	frame++;
	order = lastorder;
	//帧操纵函数
	//功能：在每一帧被调用，完成你的机甲在这一帧的动作决策
	//参数：order	...	机甲操纵指令，你在函数体中给它赋值以操纵机甲在这一帧的行为
	//		info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	//		(这几个参数的详细说明在开发手册可以找到，你也可以在RobotAIstruct.h中直接找到它们的代码)
	srand(777);
	
	init(info, myID);
	//calcLoc(info, order.eturn, order.run);
// 	
	calcFire(info, order.wturn, order.fire);
	AttackWithDefend(info, order);
//	rotateTest(info, order);

// 	if (info.robotInformation[enemyID].weaponTypeName == WT_MissileLauncher){
// 		AttackWithDefend(info,order.run, order.eturn);
// 	}
// 	else{
// 		DefendThenAttack(info, order.run, order.eturn);
// 	}
	lastorder = order;

	changeTurn(info);
}



void RobotAI::ChooseArmor(weapontypename& weapon,enginetypename& engine,bool a)
{
	//挑选装备函数
	//功能：在战斗开始时为你的机甲选择合适的武器炮塔和引擎载具
	//参数：weapon	...	代表你选择的武器，在函数体中给它赋值
	//		engine	...	代表你选择的引擎，在函数体中给它赋值
	//tip:	括号里的参数是枚举类型 weapontypename 或 enginetypename
	//		开发文档中有详细说明，你也可以在RobotAIstruct.h中直接找到它们的代码
	//tip:	最后一个bool是没用的。。那是一个退化的器官

	weapon = WT_Cannon;	//啊，我爱加农炮
	engine = ET_UFO;	//啊，我爱小蜘蛛
}




//-----------------------------------------------------
//2.个性信息
//-----------------------------------------------------


string RobotAI::GetName()
{
	//返回你的机甲的名字
	return "Aldnoah Zero";
}

string RobotAI::GetAuthor()
{
	//返回机甲制作人或团队的名字
	return "charlotte";
}




//返回一个(-255,255)之间的机甲武器炮塔的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetWeaponRed()
{
	//返回一个-255-255之间的整数,代表武器红色的偏移值
	return 235;
}
int RobotAI::GetWeaponGreen()
{
	//返回一个-255-255之间的整数,代表武器绿色的偏移值
	return 91;
}
int RobotAI::GetWeaponBlue()
{
	//返回一个-255-255之间的整数,代表武器蓝色的偏移值
	return 2;
}



//返回一个(-255,255)之间的机甲引擎载具的颜色偏移值（红、绿、蓝）
//你可以在flash客户端的参数预览中预览颜色搭配的效果
int RobotAI::GetEngineRed()
{
	//返回一个-255-255之间的数,代表载具红色的偏移值
	return 235;
}
int RobotAI::GetEngineGreen()
{
	//返回一个-255-255之间的整数,代表载具绿色的偏移值
	return 91;
}
int RobotAI::GetEngineBlue()
{
	//返回一个-255-255之间的整数,代表载具蓝色的偏移值
	return 2;
}









//-----------------------------------------------------
//3.用不用随你的触发函数
//-----------------------------------------------------


void RobotAI::onBattleStart(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗开始时被调用，可能可以用来初始化
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
	rep(i, 2){
		locs[i].x = info.arsenal[i].circle.x;
		locs[i].y = info.arsenal[i].circle.y;
	}
	loc = 0;
	outCircle = false;
	obsLoc[0].x = 300, obsLoc[0].y = 250;
	obsLoc[1].x = 1066, obsLoc[1].y = 430;
	obsLoc[0].r = obsLoc[1].r = 75;

	obsLocCorner[0].x = 150, obsLocCorner[0].y = 125;
	obsLocCorner[1].x = 1216, obsLocCorner[1].y = 555;
	boundary = info.boundary;
	
	enemyTrack.clear();
	enemyVVector.clear();
	defaultTurn = -1;
	if (dis(info.robotInformation[myID].circle, obsLoc[0]) < dis(info.robotInformation[myID].circle, obsLoc[1])){
		if (info.robotInformation[1 - myID].weaponTypeName == WT_ElectricSaw)
			defaultTurn = -1;
		else defaultTurn = 1;
	}
	else{
		if (info.robotInformation[1 - myID].weaponTypeName == WT_ElectricSaw)
			defaultTurn = -1;
		else defaultTurn = 1;
	}
}


void RobotAI::onBattleEnd(const RobotAI_BattlefieldInformation& info,int myID)
{
	//一场战斗结束时被调用，可能可以用来析构你动态分配的内存空间（如果你用了的话）
	//参数：info	...	战场信息
	//		myID	... 自己机甲在info中robot数组对应的下标
}


void RobotAI::onSomeoneFire(int fireID)
{
	//有机甲开火时被调用
	//参数：fireID	... 开火的机甲下标
	//printf("%d:AAFire~~~\n", fireID);
	if (fireID == 0){
		puts("Fire!!!!!!!!!!!!!!!!!!!!!!!\n\n");
	}
}


void RobotAI::onHit(int launcherID,bullettypename btn)
{
	//被子弹击中时被调用
	//参数：btn	...	击中你的子弹种类（枚举类型）
}



//TODO:这里可以实现你自己的函数