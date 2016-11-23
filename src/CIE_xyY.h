#ifndef __CIE_xyY_H__
#define __CIE_xyY_H__

#include "common.h"

class CIE_XYZ;
class CIE_xyY;

typedef pair<CIE_xyY, CIE_xyY> pxyY;

class CIE_xyY
{
public:
	float x, y;
	float Y;
	CIE_xyY(float x=0, float y=0, float Y=0);
	CIE_xyY(CIE_XYZ);

	operator CIE_XYZ();

	void print() const;
};

CIE_xyY operator + (CIE_xyY, CIE_xyY);
CIE_xyY operator - (CIE_xyY, CIE_xyY);

float dist(CIE_xyY, CIE_xyY);
float dist(pxyY);

bool operator == (CIE_xyY, CIE_xyY);

#endif
