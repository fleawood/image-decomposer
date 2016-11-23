#include "common.h"
#include "Line.h"
#include "misc.h"

Line::Line(Point a, Point b)
{
	this->a = a;
	this->b = b;
}

Point get_intersection(Line l1, Line l2)
{
	Point p(l1.a), q(l2.b);
	Vector v(l1.b-p), w(q-l2.a), u(p-q);
	float t = cross(w, u) / cross(v, w);
	return p + t * v;
}

bool is_parallel(Line l1, Line l2)
{
	return dcmp(cross(l1.b-l1.a, l2.b-l2.a)) == 0;
}

bool is_on_line(Point p, Line l)
{
	return dcmp(cross(l.a-p, l.b-p)) == 0;
}

bool is_on_segment(Point p, Line l)
{
	return is_on_line(p, l) && dcmp(dot(l.a-p, l.b-p)) <= 0;
}

bool is_intersected(Line l1, Line l2, Point &p)
{
	if (is_parallel(l1, l2)) return false;
	p = get_intersection(l1, l2);
	return is_on_segment(p, l1) && is_on_segment(p, l2);
}

bool is_in_triangle(Point p, Point a, Point b, Point c)
{
	float sabc, sbca, scab, s1, s2;
	s1 = fabs(area(a, b, c));

	sabc = fabs(area(p, a, b));
	sbca = fabs(area(p, b, c));
	scab = fabs(area(p, c, a));
	s2 = sabc + sbca + scab;

	return dcmp(sabc) > 0 && dcmp(sbca) > 0 && dcmp(scab) > 0 && dcmp(s1-s2) == 0;
}
