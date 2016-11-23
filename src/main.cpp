#include "common.h"
#include "CIE_XYZ.h"
#include "CIE_xyY.h"
#include "Line.h"
#include "misc.h"
#include "color.h"

float Y_range(const CIE_xyY& src)
{
	return min(min(src.Y-0.0, 1.0-src.Y), 0.2);
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

void xyY_decompose(const CIE_xyY& src, CIE_xyY& dest, CIE_xyY& dest2)
{
	float range = Y_range(src);
	pxyY a[6];
	a[0] = decompose(src, range, Red, Green, Blue);
	a[1] = decompose(src, range, Green, Blue, Red);
	a[2] = decompose(src, range, Blue, Red, Green);
	a[3] = decompose(src, -range, Red, Green, Blue);
	a[4] = decompose(src, -range, Green, Blue, Red);
	a[5] = decompose(src, -range, Blue, Red, Green);

	pxyY ans = *max_element(a, a+6, min_dist);
	dest = ans.first;
	dest2 = ans.second;
}

void XYZ_decompose(const CIE_XYZ& src, CIE_XYZ& dest, CIE_XYZ& dest2)
{
	CIE_xyY src_, dest_, dest2_;

	src_ = static_cast<CIE_xyY>(src);
	dest_ = static_cast<CIE_xyY>(dest);
	dest2_ = static_cast<CIE_xyY>(dest2);

	xyY_decompose(src_, dest_, dest2_);

	dest = static_cast<CIE_XYZ>(dest_);
	dest2 = static_cast<CIE_XYZ>(dest2_);
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
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j  < cols; ++j) {
					CIE_XYZ a, b, c;
					a = static_cast<CIE_XYZ>(src.at<Vec3f>(i, j));
					XYZ_decompose(a, b, c);
					dest.at<Vec3f>(i,j) = static_cast<Vec3f>(b);
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
	char *image_path = argv[1];
	Mat I, J, K, L, M, N, O, P, Q;
	I = imread(image_path, CV_LOAD_IMAGE_COLOR);
	I.convertTo(J, CV_32F, 1.0 / 255.0);
	cvtColor(J, K, CV_BGR2XYZ);
	color_decompose(K, L, M);
	color_compose(L, M, N);
	cvtColor(L, O, CV_XYZ2BGR);
	cvtColor(M, P, CV_XYZ2BGR);
	cvtColor(N, Q, CV_XYZ2BGR);
	imshow("O", O);
	imshow("P", P);
	imshow("Q", Q);
	imshow("J", J);
	waitKey(0);
	return 0;
}
