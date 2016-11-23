#include "common.h"
#include "CIE_xyY.h"
#include "CIE_XYZ.h"
#include "misc.h"

CIE_xyY::CIE_xyY(float x, float y, float Y)
{
	this->x = x;
	this->y = y;
	this->Y = Y;
}

CIE_xyY::CIE_xyY(CIE_XYZ src)
{
	float X, Z, S;

	X = src.X;
	Y = src.Y;
	Z = src.Z;
	S = X + Y + Z;

	if (S == 0) {
		x = 0;
		y = 0;
	}
	else {
		x = X / S;
		y = Y / S;
	}
}

CIE_xyY::operator CIE_XYZ()
{
	return CIE_XYZ(*this);
}

CIE_xyY operator + (CIE_xyY a, CIE_xyY b)
{
	CIE_xyY c;
	float alpha;

	c.Y = (a.Y + b.Y) * 0.5;

	alpha = (float)a.Y / (a.Y + b.Y);
	if (isnan(alpha)) alpha = 0.0;

	c.x = alpha * a.x + (1.0 - alpha) * b.x;
	c.y = alpha * a.y + (1.0 - alpha) * b.y;

	return c;
}

CIE_xyY operator - (CIE_xyY c, CIE_xyY b)
{
	CIE_xyY a;
	float alpha;

	a.Y = c.Y * 2.0 - b.Y;
	alpha = (float)a.Y / (a.Y + b.Y);

	a.x = (c.x - (1.0 - alpha) * b.x) / alpha;
	a.y = (c.y - (1.0 - alpha) * b.y) / alpha;

	return a;
}

float dist(CIE_xyY a, CIE_xyY b)
{
	return sqrt(sqr(a.x-b.x) + sqr(a.y-b.y));
}

float dist(pxyY a)
{
	return dist(a.first, a.second);
}

void CIE_xyY::print() const
{
	cout << '[' << x << ", " << y << ", " << Y << ']' << endl;
}

bool operator == (CIE_xyY a, CIE_xyY b)
{
	return dcmp(a.x-b.x) == 0 && dcmp(a.y-b.y) == 0 && a.Y == b.Y;
}
