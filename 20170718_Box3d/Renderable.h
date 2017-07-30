#pragma once
#include "cVector3.h"
#include "cVector4.h"
#include "cMatrix.h"
#include "Math3D.h"
class Camera3D;
class Renderable
{
public:
	Renderable();
	virtual ~Renderable();
	virtual void Render(Camera3D& camera) const = 0;

	cVector3 GetPosition();
	void Translate(float dx, float dy, float dz);
	void TranslateLocal(float dx, float dy, float dz);
	void Rotate(float dx, float dy, float dz);
	void RotateLocal(float dx, float dy, float dz);

	void SetPosition(const float& x, const float& y, const float& z);
	void SetAngles(const float& x, const float& y, const float& z);
	const cMatrix& GetWorldMatrix() const {
		return _matWorld;
	}
protected:
	cMatrix _matWorld;
};

