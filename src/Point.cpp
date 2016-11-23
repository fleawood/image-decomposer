#include "common.h"
#include "Point.h"

Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector operator + (Vector a, Vector b)
{
	return Vector(a.x+b.x, a.y+b.y);
}

Vector operator - (Vector a, Vector b)
{
	return Vector(a.x-b.x, a.y-b.y);
}

Vector operator * (Vector a, float t)
{
	return Vector(a.x*t, a.y*t);
}

Vector operator * (float t, Vector a)
{
	return Vector(a.x*t, a.y*t);
}

float dot(Vector a, Vector b)
{
	return a.x*b.x + a.y*b.y;
}

float cross(Vector a, Vector b)
{
	return a.x*b.y - a.y*b.x;
}

float area(Point a, Point b, Point c)
{
	return cross(b-a, c-a)*0.5;
}
