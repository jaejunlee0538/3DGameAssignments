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
	return sqrt(_xyz[0]* _xyz[0] + _xyz[1] * _xyz[1] + _xyz[2] * _xyz[2]);
}

float cVector3::LengthSq() const
{
	return _xyz[0] * _xyz[0] + _xyz[1] * _xyz[1] + _xyz[2] * _xyz[2];
}

cVector3 cVector3::Normalize() const
{
	return (*this)*(1.0f/Length());
}

void cVector3::Print()
{
	std::cout << "[" << X() << "," << Y() << "," << Z() << "]" << std::endl;
}

float & cVector3::X()
{
	return _xyz[0];
}

const float & cVector3::X() const
{
	return _xyz[0];
}

float & cVector3::Y()
{
	return _xyz[1];
}

const float & cVector3::Y() const
{
	return _xyz[1];
}

float & cVector3::Z()
{
	return _xyz[2];
}

const float & cVector3::Z() const
{
	return _xyz[2];
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
	return cVector3(X() + vec.X(), Y() + vec.Y(), Z() + vec.Z());
}

cVector3 cVector3::operator-(const cVector3 & vec) const
{
	return cVector3(X() - vec.X(), Y() - vec.Y(), Z() - vec.Z());
}

cVector3 cVector3::operator*(float s) const
{
	return cVector3(X()*s, Y()*s, Z()*s);
}

float cVector3::Dot(const cVector3 & v1, const cVector3 & v2)
{
	return v1.X()*v2.X() + v1.Y()*v2.Y() + v1.Z()*v2.Z();
}

cVector3 cVector3::Cross(const cVector3 & v1, const cVector3 & v2)
{
	return cVector3(v1.Y()*v2.Z() - v1.Z()*v2.Y(), v1.Z()*v2.X() - v1.X()*v2.Z(), v1.X()*v2.Y() - v1.Y()*v2.X());
}
