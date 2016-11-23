#ifndef __misc_H__
#define __misc_H__

#include "common.h"

const float eps = 1e-6;

template<typename T> T sqr(T x)
{
	return x * x;
}

int dcmp(float x);

#endif
