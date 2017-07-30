#pragma once
#include "Renderable.h"
#include <vector>
class Grid :
	public Renderable
{
public:
	Grid();
	~Grid();
	void init(int nX, int nZ, float sizeX, float sizeZ);
	float GetWidth() const;
	float GetHeight() const;
	void Render(Camera3D& camera) const;
private:
	std::vector<cVector3> _vertices;
	std::vector<int> _indices;
	float _w, _h;
};

