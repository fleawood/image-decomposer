#include "common.h"
#include "misc.h"

int dcmp(float x)
{
	return fabs(x) < eps ? 0 : x > 0 ? 1 : -1;
}
