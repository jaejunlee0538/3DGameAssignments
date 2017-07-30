#include "stdafx.h"
#include "cVector3.h"
#include <cmath>

cVector3::cVector3()
{
	
}

cVector3::cVector3(float x, float y, float z)
{
	_xyz[0] = x;
	_xyz[1] = y;
	_xyz[2] = z;
}

cVector3::~cVector3()
{
}

float cVector3::Length() const
{
	return sqrt(LengthSq());
}

float cVector3::LengthSq() const
{
	return _xyz[0] * _xyz[0] + _xyz[1] * _xyz[1] + _xyz[2] * _xyz[2];
}

cVector3 cVector3::Normalize() const
{
	return (*this)*(1.0f/Length());
}

float & cVector3::operator[](size_t idx)
{
	return _xyz[idx];
}

const float & cVector3::operator[](size_t idx) const
{
	return _xyz[idx];
}

bool cVector3::operator==(const cVector3 & vec) const
{
	const static float epsilon = 1e-6;
	for (int i = 0; i < 3; ++i) {
		if (std::abs((*this)[i] - vec[i]) < epsilon)
			return false;
	}
	return true;
}

bool cVector3::operator!=(const cVector3 & vec) const
{
	return !((*this)==vec);
}

cVector3 cVector3::operator+(const cVector3 & vec) const
{
	return cVector3(x() + vec.x(), y() + vec.y(), z() + vec.z());
}

cVector3 cVector3::operator-(const cVector3 & vec) const
{
	return cVector3(x() - vec.x(), y() - vec.y(), z() - vec.z());
}

cVector3 cVector3::operator*(float s) const
{
	return cVector3(x()*s, y()*s, z()*s);
}

float cVector3::Dot(const cVector3 & v1, const cVector3 & v2)
{
	return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

cVector3 cVector3::Cross(const cVector3 & v1, const cVector3 & v2)
{
	return cVector3(v1.y()*v2.z() - v1.z()*v2.y(), v1.z()*v2.x() - v1.x()*v2.z(), v1.x()*v2.y() - v1.y()*v2.x());
}
