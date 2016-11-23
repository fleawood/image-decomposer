#ifndef __Point_H__
#define __Point_H__

class Point
{
public:
	float x, y;
	Point(float x=0, float y=0);
};

typedef Point Vector;

Vector operator + (Vector, Vector);
Vector operator - (Vector, Vector);
Vector operator * (Vector, float);
Vector operator * (float, Vector);

float dot(Vector, Vector);
float cross(Vector, Vector);
float area(Point, Point, Point);

#endif
