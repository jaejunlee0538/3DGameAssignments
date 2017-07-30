#pragma once
#include <cmath>
#include <Windows.h>
#include "Line2D.h"

#define DEG_TO_RAD 0.017453f //1���� ���� ��
#define PI 3.141592654f
#define PI2 6.283185f
#define PI8 float(PI / 8.0f)
#define PI16 float(PI / 16.0f)
#define PI32 float(PI / 32.0f)
#define PI64 float(PI / 64.0f)
#define PI128 float(PI / 128.0f)

#define FLOAT_EPSILON 0.001f

//�Ǽ��� ���������� ��ȯ�Ѵ�
#define FLOAT_TO_INT(f1) static_cast<int>(f1 + FLOAT_EPSILON)

//�� �Ǽ��� ������ Ȯ���Ѵ�
//fabs�� �Ҽ��� ���� ���밪�� ��ȯ�Ѵ�
#define FLOAT_EQUAL(f1, f2) (fabs(f1 - f2) <= FLOAT_EPSILON)

namespace SGA {
	typedef POINTFLOAT Point2D;
	template <typename T>
	int getSign(T val) {
		if (val > 0)
			return 1;
		if (val < 0)
			return -1;
		return 0;
	}

	Point2D operator+(const Point2D& p1, const Point2D& p2);
	Point2D operator-(const Point2D& p1, const Point2D& p2);
	Point2D operator*(float s, const Point2D& p1);

	POINT POINTFLOAT2POINT(const POINTFLOAT& pt);
	POINTFLOAT POINT2POINTFLOAT(const POINT& pt);

	float deg2rad(float deg);
	float rad2deg(float rad);

	bool isEqualFloat(const float& num1, const float& num2, const float& epsilon = 0.001f);

	bool isEqualfloat(const float& num1, const float& num2, const float& epsilon = 0.001);

	//�� ������ �Ÿ��� ���Ѵ�.
	float getDistance(float startX, float startY, float endX, float endY);

	//�� ������ ���� �������� ���Ѵ�.
	float getAngle(float startX, float startY, float endX, float endY);

	void normalizeXY(float& x, float& y);

	/*
	p�� ���̸� ��ȯ�Ѵ�.
	*/
	float norm(const Point2D& p);

	float norm(const float& x, const float& y);

	/*
	�� p1�� p2�� ������ ��ȯ�Ѵ�.
	*/
	float dotProduct(const Point2D& p1, const Point2D& p2);

	/*
	v1�� v2������ ������ ��ȯ�Ѵ�.
	*/
	float crossProduct(const Point2D& v1, const Point2D& v2);

	/*
	�� p�� ����(0,0)�� ���Ͽ� angle��ŭ ȸ����Ų ���� ��ȯ�Ѵ�.
	*/
	Point2D rotate2D(const Point2D& p, const float& angle);

	void rotate2D(Point2D& p, const float& angle, const Point2D& anchor);

	void getRotatedRECT(const RECT& rect, const float& angle, POINTFLOAT * points);
	void getRotatedRECT(const RECT& rect, const float& angle, POINT * points);
	void getRotatedRECT(const RECT& rect, const float& angle, POINTFLOAT * points, const POINTFLOAT& pivot);
	void getRotatedRECT(const RECT& rect, const float& angle, POINT * points, const POINT& pivot);
	/*
	return true if two lines,L1 and L2, are (exactly or almost) parallel.
	return false otherwise.
	*/
	bool isParallel(const Line2D& L1, const Line2D& L2);

	/*
	return true, if 'line' is (almost) vertical.
	*/
	bool isVertical(const Line2D& line);

	/*
	a1->a2, b1->b2�� �� ������ �����ϸ� true�� ��ȯ�Ѵ�.
	*/
	bool isSegmentsIntersect(const Point2D& a1, const Point2D& a2, const Point2D& b1, const Point2D& b2);

	/*
	�� ������ ������ line1�� line2�� ������ ������ ��ȯ�Ѵ�.
	�� ������ �������� �ʾƾ� �Ѵ�!
	*/
	Point2D getIntersectingPointLineLine(const Line2D& line1, const Line2D& line2);

	bool getIntersectingPointLineLine(const Line2D& line1, const Line2D& line2, Point2D& pointOut);

	/*
	���� 'line'�� �� p�� ������ 'line'�� �����ϴ� ���� ������ ������ ��ȯ�Ѵ�.
	*/
	Point2D getIntersectingPointPointLine(const Point2D& p, const Line2D& line);

	/*
	return the distance between a point 'p' and a line 'L'.
	*/
	float getDistancePointLine(const Point2D& p, const Line2D& L);

	/*
	s=0.0      : begin
	s=1.0      : end
	0.0<s<1.0  : interpolate
	s>1.0      : extrapolate
	s<0.0      : negative extrapolate
	*/
	Point2D getInterpolatedPoint(const Point2D& begin, const Point2D& end, float s);

	/*
	���� 'line'���� �ְ� �� p0�� dist��ŭ�� �Ÿ��� �ִ� ���� ��ȯ�Ѵ�.
	p0���� dist��ŭ ������ ���� �� 2���̴�.
	if 'dist' > 0
		p0���� �����ʿ� �ִ� ���� ��ȯ.
	else if 'dist' < 0
		p0���� ���ʿ� �ִ� ���� ��ȯ.
	else
		p0�� ��ȯ
	*/
	Point2D getPointOnLine(const Line2D& line, const Point2D& p0, const float& dist);
}