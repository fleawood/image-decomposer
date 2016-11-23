#ifndef __Line_H__
#define __Line_H__

#include "Point.h"

class Line
{
public:
	Point a, b;
	Line(Point a=Point(0, 0), Point b=Point(0.0));
};

Point get_intersection(Line, Line);
bool is_parallel(Line, Line);
bool is_on_line(Point, Line);
bool is_on_segment(Point, Line);
bool is_intersected(Line, Line, Point&);
bool is_in_triangle(Point, Point, Point, Point);


#endif
