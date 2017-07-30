#include "Renderable.h"
#include <Windows.h>


Renderable::Renderable()
{
	_matWorld = cMatrix::Identity(4);
}

Renderable::~Renderable()
{
}

cVector3 Renderable::GetPosition()
{
	return cVector3(_matWorld(3,0), _matWorld(3, 1), _matWorld(3, 2));
}

void Renderable::Translate(float dx, float dy, float dz)
{
	_matWorld = _matWorld * cMatrix::Translation(dx, dy, dz);
}

void Renderable::TranslateLocal(float dx, float dy, float dz)
{
	cVector4 translation = cVector4(dx, dy, dz, 0) * _matWorld;
	Translate(translation.x(), translation.y(), translation.z());
}

void Renderable::Rotate(float dx, float dy, float dz)
{
	_matWorld = cMatrix::RotationZ(dz) * cMatrix::RotationY(dy) * cMatrix::RotationX(dx) * _matWorld;
	cMatrix::NormalizeRotationPart(_matWorld);
}

void Renderable::RotateLocal(float dx, float dy, float dz)
{
	_matWorld = _matWorld * cMatrix::RotationZ(dz) * cMatrix::RotationY(dy) * cMatrix::RotationX(dx);
	cMatrix::NormalizeRotationPart(_matWorld);
}

void Renderable::SetPosition(const float & x, const float & y, const float & z)
{
	_matWorld(3, 0) = x;
	_matWorld(3, 1) = y;
	_matWorld(3, 2) = z;
}

void Renderable::SetAngles(const float & x, const float & y, const float & z)
{
	cMatrix rot = cMatrix::RotationZ(z) * cMatrix::RotationY(y) * cMatrix::RotationX(x);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			_matWorld(i, j) = rot(i, j);
		}
	}
}
