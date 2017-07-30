#include "stdafx.h"
#include "cVector4.h"
#include "cVector3.h"
#include <assert.h>
#include <cmath>

cVector4::cVector4()
{

}

cVector4::cVector4(float x, float y, float z, float w)
{
	_xyzw[0] = x;
	_xyzw[1] = y;
	_xyzw[2] = z;
	_xyzw[3] = w;
}

cVector4::cVector4(const cVector3 & vec, float w)
{
	_xyzw[0] = vec.x();
	_xyzw[1] = vec.y();
	_xyzw[2] = vec.z();
	_xyzw[3] = w;
}

cVector4::~cVector4()
{
}

float cVector4::Length() const
{
	return sqrt(LengthSq());
}

float cVector4::LengthSq() const
{
	return _xyzw[0] * _xyzw[0] + _xyzw[1] * _xyzw[1] + _xyzw[2] * _xyzw[2];
}

cVector4 cVector4::Normalize() const
{
	return (*this)*(1.0f / Length());
}

void cVector4::DivideByW()
{
	//assert(std::abs(_xyzw[3]) > 0.000001f);
	_xyzw[0] /= _xyzw[3];
	_xyzw[1] /= _xyzw[3];
	_xyzw[2] /= _xyzw[3];
	_xyzw[3] = 1.0f;
}

float & cVector4::operator[](size_t idx)
{
	return _xyzw[idx];
}

const float & cVector4::operator[](size_t idx) const
{
	return _xyzw[idx];
}

bool cVector4::operator==(const cVector4 & vec) const
{
	const static float epsilon = 1e-6;
	for (int i = 0; i < 4; ++i) {
		if (std::abs((*this)[i] - vec[i]) < epsilon)
			return false;
	}
	return true;
}

bool cVector4::operator!=(const cVector4 & vec) const
{
	return !((*this) == vec);
}

cVector4 cVector4::operator+(const cVector4 & vec) const
{
	return cVector4(x() + vec.x(), y() + vec.y(), z() + vec.z(), (w() > vec.w()) ? w() : vec.w());
}

cVector4 cVector4::operator-(const cVector4 & vec) const
{
	return cVector4(x() - vec.x(), y() - vec.y(), z() - vec.z(), (w() > vec.w()) ? w() : vec.w());
}

cVector4 cVector4::operator*(float s) const
{
	return cVector4(x()*s, y()*s, z()*s, w());
}

float cVector4::Dot(const cVector4 & v1, const cVector4 & v2)
{
	return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

cVector4 cVector4::Cross(const cVector4 & v1, const cVector4 & v2)
{
	return cVector4(
		v1.y()*v2.z() - v1.z()*v2.y(), 
		v1.z()*v2.x() - v1.x()*v2.z(), 
		v1.x()*v2.y() - v1.y()*v2.x(), 
		0);
}
