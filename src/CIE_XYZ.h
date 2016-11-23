#ifndef __CIE_XYZ_H__
#define __CIE_XYZ_H__

#include "common.h"

class CIE_xyY;
class CIE_XYZ;

class CIE_XYZ
{
public:
	float X, Y, Z;
	CIE_XYZ(float X=0, float Y=0, float Z=0);
	CIE_XYZ(CIE_xyY);
	CIE_XYZ(Vec3f);

	operator CIE_xyY();
	operator Vec3f();
	void print() const;
};

#endif


