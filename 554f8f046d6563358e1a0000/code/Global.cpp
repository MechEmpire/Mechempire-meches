#include "Global.h"

int Solve1_2Function(double a, double b, double c, double& root1, double& root2)
{
	double delta = b*b - 4 * a*c;
	if (delta < 0)
		return 0;
	if (fabs(a - 0) > EPS)
	{
		root1 = (-b + sqrt(delta)) / (2 * a);
		root2 = (-b - sqrt(delta)) / (2 * a);
		return 1;
	}
	else
	{
		root1 = root2 = c / b;
		return 2;
	}
}

double Det2x2(double a00, double a01, double a10, double a11)
{
	return a00*a11 - a01*a10;
}

int Solve2_1Function(double x1, double y1, double z1, double x2, double y2, double z2, Vector2D ans)
{
	double D = Det2x2(x1, y1, x2, y2);
	if (fabs(D)<EPS)
	{
		return 0;
	}
	double Dx = Det2x2(z1, y1, z2, y2);
	double Dy = Det2x2(x1, z1, x2, z2);
	ans.x = Dx / D;
	ans.y = Dy / D;
	return 1;
}

int GetTangentPoints(Circle c, Vector2D v, Vector2D& ans1, Vector2D& ans2)
{
	if (HitTestCirclePoint(c, v.x, v.y))
	{
		return 0;
	}
	Vector2D center(c.x, c.y);
	double c_cos = c.r / center.Distance(v);
	double c_angle = acos(c_cos);
	double v_angle = atan2(v.y - c.y, v.x - c.x);
	ans1.x = c.r*cos(c_angle - v_angle) + c.x;
	ans1.y = c.r*sin(c_angle - v_angle) + c.y;
	ans2.x = c.r*cos(c_angle + v_angle) + c.x;
	ans2.y = c.r*sin(c_angle + v_angle) + c.y;
	return 1;
}
double min(double a, double b)
{
	return a < b ? a : b;
}
double max(double a, double b)
{
	return a > b ? a : b;
}
bool SegmentCross(Vector2D a, Vector2D b, Vector2D c, Vector2D d)
{
	//segment ab and segment cd
	if (min(a.x, b.x) > max(c.x, d.x) ||
		min(a.y, b.y) > max(c.y, d.y) ||
		min(c.x, d.x) > max(a.x, b.x) ||
		min(c.y, d.y) > max(a.y, b.y))
		return 0;
	double h, i, j, k;
	h = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	i = (b.x - a.x) * (d.y - a.y) - (b.y - a.y) * (d.x - a.x);
	j = (d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x);
	k = (d.x - c.x) * (b.y - c.y) - (d.y - c.y) * (b.x - c.x);
	return h * i <= eps && j * k <= eps;
}