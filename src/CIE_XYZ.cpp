#include "common.h"
#include "CIE_XYZ.h"
#include "CIE_xyY.h"

CIE_XYZ::CIE_XYZ(float X, float Y, float Z)
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
}

CIE_XYZ::CIE_XYZ(CIE_xyY src)
{
	float x, y, z;
	x = src.x;
	y = src.y;
	z = 1.0 - x - y;
	Y = src.Y;
	X = Y * x / y;
	Z = Y * z / y;
}

CIE_XYZ::CIE_XYZ(Vec3f vec)
{
	X = vec[0];
	Y = vec[1];
	Z = vec[2];
}

CIE_XYZ::operator CIE_xyY()
{
	return CIE_xyY(*this);
}

CIE_XYZ::operator Vec3f()
{
	return Vec3f(X, Y, Z);
}

void CIE_XYZ::print() const
{
	cout << '[' << X << ", " << Y << ", " << Z << ']' << endl;
}
