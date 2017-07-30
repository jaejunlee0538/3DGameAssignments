#pragma once
#include "Renderable.h"
#include <vector>
#include "cVector3.h"
class Box : public Renderable
{
public:
	Box();
	Box(float width, float height, float depth);
	~Box();
	void init(float width, float height, float depth);
	void Render(Camera3D& camera) const;
private:
	cVector3 _vertices[8];
	int _indices[36];
};