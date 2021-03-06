#pragma once
#include <cmath>
#include <Windows.h>
#include "Line2D.h"

#define DEG_TO_RAD 0.017453f //1도의 라디안 값
#define PI 3.141592654f
#define PI2 6.283185f
#define PI8 float(PI / 8.0f)
#define PI16 float(PI / 16.0f)
#define PI32 float(PI / 32.0f)
#define PI64 float(PI / 64.0f)
#define PI128 float(PI / 128.0f)

#define FLOAT_EPSILON 0.001f

//실수를 정수형으로 변환한다
#define FLOAT_TO_INT(f1) static_cast<int>(f1 + FLOAT_EPSILON)

//두 실수가 같은지 확인한다
//fabs는 소수점 차의 절대값을 반환한다
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

	//두 지점의 거리를 구한다.
	float getDistance(float startX, float startY, float endX, float endY);

	//두 지점의 각을 라디안으로 구한다.
	float getAngle(float startX, float startY, float endX, float endY);

	void normalizeXY(float& x, float& y);

	/*
	p의 길이를 반환한다.
	*/
	float norm(const Point2D& p);

	float norm(const float& x, const float& y);

	/*
	점 p1과 p2의 내적을 반환한다.
	*/
	float dotProduct(const Point2D& p1, const Point2D& p2);

	/*
	v1과 v2사이의 외적을 반환한다.
	*/
	float crossProduct(const Point2D& v1, const Point2D& v2);

	/*
	점 p를 원점(0,0)에 대하여 angle만큼 회전시킨 점을 반환한다.
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
	a1->a2, b1->b2인 두 선분이 교차하면 true를 반환한다.
	*/
	bool isSegmentsIntersect(const Point2D& a1, const Point2D& a2, const Point2D& b1, const Point2D& b2);

	/*
	두 직선의 방정식 line1과 line2가 만나는 교점을 반환한다.
	두 직선이 수평하지 않아야 한다!
	*/
	Point2D getIntersectingPointLineLine(const Line2D& line1, const Line2D& line2);

	bool getIntersectingPointLineLine(const Line2D& line1, const Line2D& line2, Point2D& pointOut);

	/*
	직선 'line'과 점 p를 지나고 'line'과 직교하는 직선 사이의 교점을 반환한다.
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
	직선 'line'위에 있고 점 p0와 dist만큼의 거리에 있는 점을 반환한다.
	p0부터 dist만큼 떨어진 점은 총 2개이다.
	if 'dist' > 0
		p0보다 오른쪽에 있는 점을 반환.
	else if 'dist' < 0
		p0보다 왼쪽에 있는 점을 반환.
	else
		p0를 반환
	*/
	Point2D getPointOnLine(const Line2D& line, const Point2D& p0, const float& dist);
}