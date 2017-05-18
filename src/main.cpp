#include "common.h"
#include "CIE_XYZ.h"
#include "CIE_xyY.h"
#include "Line.h"
#include "misc.h"
#include "color.h"

float Y_range(const CIE_xyY& src)
{
	return min(min(src.Y-0.0, 1.0-src.Y), 0.05);
}

pxyY decompose(const CIE_xyY& src, float range, Point p1, Point p2, Point p3)
{
	if (!is_in_triangle(Point(src.x, src.y), p1, p2, p3)) {
		return make_pair(src, src);
	}

	pxyY a, b;
	a.first = CIE_xyY(p1.x, p1.y, src.Y+range);
	b.first = CIE_xyY(p2.x, p2.y, src.Y+range);

	a.second = src - a.first;
	b.second = src - b.first;

	Point q1(a.second.x, a.second.y), q2(b.second.x, b.second.y);
	Line l(q1, q2);

	bool is_separated = false;
	if (!is_in_triangle(q1, p1, p2, p3)) {
		if (!is_intersected(l, Line(p2, p3), q1)) {
			is_separated = true;
		}
	}
	if (!is_in_triangle(q2, p1, p2, p3)) {
		if (!is_intersected(l, Line(p1, p3), q2)) {
			is_separated = true;
		}
	}
	if (is_separated) {
		return make_pair(src, src);
	}

	pxyY c, d;
	c.first = CIE_xyY(q1.x, q1.y, src.Y-range);
	d.first = CIE_xyY(q2.x, q2.y, src.Y-range);

	c.second = src - c.first;
	d.second = src - d.first;

	return dist(c) > dist(d) ? c : d;
}

bool min_dist(pxyY a, pxyY b)
{
	return dist(a) < dist(b);
}

bool is_in_range(float x, float l, float r)
{
	return x >= l && x <= r;
}

pxyY random_select_color(const CIE_xyY &src, bool is_dark)
{
	float range = Y_range(src);
	float Y = is_dark ? src.Y - range : src.Y + range;
	const CIE_xyY c1(src.x, src.y, Y), c2(src - c1);
	Point p(src.x, src.y);
	if (is_too_close(p, Blue) || is_too_close(p, Green) || is_too_close(p, Red)) {
		return make_pair(c1, c2);
	}
	CIE_xyY a, b;
	int cnt = 0;
	while (true) {
		if (++cnt >= 8) {
			return make_pair(c1, c2);
		}
		Point p = get_random_point(Red, Green, Blue), q;
		if (!is_in_triangle(p, Red, Green, Blue)) continue;
		a = CIE_xyY(p.x, p.y, Y);
		b = src - a;
		q = Point(b.x, b.y);
		if (!is_in_triangle(q, Red, Green, Blue)) continue;
		CIE_XYZ c(a), d(b);
		if (is_in_range(c.X, 0.0, XYZ_MAX_X) && is_in_range(c.Z, 0.0, XYZ_MAX_Z) &&
			is_in_range(d.X, 0.0, XYZ_MAX_X) && is_in_range(d.Z, 0.0, XYZ_MAX_Z)) break;
	}
	return make_pair(a, b);
}

void xyY_decompose(const CIE_xyY& src, CIE_xyY& dest, CIE_xyY& dest2, bool is_dark)
{
	//float range = Y_range(src);
	//pxyY a[6];
	//a[0] = decompose(src, range, Red, Green, Blue);
	//a[1] = decompose(src, range, Green, Blue, Red);
	//a[2] = decompose(src, range, Blue, Red, Green);
	//a[3] = decompose(src, -range, Red, Green, Blue);
	//a[4] = decompose(src, -range, Green, Blue, Red);
	//a[5] = decompose(src, -range, Blue, Red, Green);

	//pxyY ans = *max_element(a, a+6, min_dist);
	//dest = ans.first;
	//dest2 = ans.second;
	pxyY res = random_select_color(src, is_dark);
	dest = res.first;
	dest2 = res.second;
}

void XYZ_decompose(const CIE_XYZ& src, CIE_XYZ& dest, CIE_XYZ& dest2, bool is_dark)
{
	CIE_xyY src_, dest_, dest2_;

	src_ = static_cast<CIE_xyY>(src);
	dest_ = static_cast<CIE_xyY>(dest);
	dest2_ = static_cast<CIE_xyY>(dest2);

	xyY_decompose(src_, dest_, dest2_, is_dark);

	dest = static_cast<CIE_XYZ>(dest_);
	dest2 = static_cast<CIE_XYZ>(dest2_);
}

void neighbour_average(Mat& dest)
{
	Mat src;
	dest.copyTo(src);
	const int rows = src.rows;
	const int cols = src.cols;
	const int dx[8] = { 0, 0, 1, 1, 1, -1, -1, -1 };
	const int dy[8] = { 1, -1, -1, 0, 1, -1, 0, 1 };
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			Vec3f color = src.at<Vec3f>(i, j);
			int number = 1;
			for (int k = 0; k < 4; ++k) {
				int ti = i + dx[k], tj = j + dy[k];
				if (ti >= 0 && ti < rows && tj >= 0 && tj < cols) {
					++number;
					color += src.at<Vec3f>(ti, tj);
				}
			}
			color /= number;
			dest.at<Vec3f>(i, j) = color;
		}
	}
}

void color_decompose(const Mat& src, Mat& dest, Mat& dest2)
{
	src.copyTo(dest);
	src.copyTo(dest2);
	const int channels = src.channels();
	const int rows = src.rows;
	const int cols = src.cols;
	switch (channels) {
		case 1:
			assert(0);
			break;
		case 3: {
			int cnt = 0;
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j  < cols; ++j) {
					int x = i / BLOCK_SIZE;
					int y = j / BLOCK_SIZE;
					bool is_dark = (x + y) & 0x1;
					CIE_XYZ a, b, c;
					a = static_cast<CIE_XYZ>(src.at<Vec3f>(i, j));
					XYZ_decompose(a, b, c, is_dark);

					dest.at<Vec3f>(i,j) = static_cast<Vec3f>(b);
				}
			}
			neighbour_average(dest);
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					CIE_XYZ a, b, c;
					a = static_cast<CIE_XYZ>(src.at<Vec3f>(i, j));
					b = static_cast<CIE_XYZ>(dest.at<Vec3f>(i, j));
					CIE_xyY d(a), e(b), f;
					f = d - e;
					c = static_cast<CIE_XYZ>(f);
					dest2.at<Vec3f>(i, j) = static_cast<Vec3f>(c);
				}
			}
			break;
		}
		default:
			assert(0);
			break;
	}
}

void color_compose(const Mat &src, const Mat &src2, Mat& dest)
{
	src.copyTo(dest);
	const int channels=src.channels();
	const int rows=src.rows;
	const int cols=src.cols;
	switch (channels) {
		case 1:
			assert(0);
			break;
		case 3:
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					CIE_XYZ a, b, c;
					CIE_xyY a_, b_, c_;

					a = static_cast<CIE_XYZ>(src.at<Vec3f>(i, j));
					b = static_cast<CIE_XYZ>(src2.at<Vec3f>(i, j));
					a_ = static_cast<CIE_xyY>(a);
					b_ = static_cast<CIE_xyY>(b);

					c_ = a_ + b_;
					c = static_cast<CIE_XYZ>(c_);
					dest.at<Vec3f>(i, j) = static_cast<Vec3f>(c);
				}
			}
			break;
		default:
			assert(0);
			break;
	}
}
int main(int argc, char *argv[])
{
	srand(time(0));
	char *image_path = argv[1];
	Mat I, J, K, L, M, N, O, P, Q;
	I = imread(image_path, CV_LOAD_IMAGE_COLOR);
	I.convertTo(J, CV_32F, 1.0 / 255.0);
	cvtColor(J, K, CV_BGR2XYZ);
	color_decompose(K, L, M);
	cvtColor(L, O, CV_XYZ2BGR);
	cvtColor(M, P, CV_XYZ2BGR);
	imshow("a", J);
	imshow("b", O);
	imshow("c", P);
	while (true) {
		imshow("d", O);
		waitKey(1);
		imshow("d", P);
		waitKey(1);
	}
	return 0;
}
