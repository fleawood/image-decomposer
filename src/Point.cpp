#include "common.h"
#include "Point.h"

Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector operator + (Vector a, Vector b)
{
	return Vector(a.x + b.x, a.y + b.y);
}

Vector operator - (Vector a, Vector b)
{
	return Vector(a.x - b.x, a.y - b.y);
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
	return cross(b - a, c - a)*0.5;
}

float length(Vector a)
{
	return sqrt(dot(a, a));
}

float dist(Point a, Point b)
{
	return length(b - a);
}

Point get_random_point(Point a, Point b, Point c)
{
	Vector u = b - a, v = c - a;
	float x = (float)rand() / RAND_MAX;
	float y = (float)rand() / RAND_MAX;
	Vector w(x, y), z(1.0 - x, 1.0 - y);
	if (length(w) > length(z)) swap(w, z);
	return a + Point(u.x*w.x + v.x*w.y, u.y*w.x + v.y*w.y);
}

bool is_too_close(Point a, Point b)
{
	return dist(a, b) < 2e-1;
}
