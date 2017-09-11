#pragma once
#include "cPyramid.h"
class Grid
{
public:
	Grid();
	~Grid();
	void init(int nX, int nZ, float sizeX, float sizeZ);

	float getXWidth() const;
	float getYWidth() const;

	void render(LPDIRECT3DDEVICE9 device) const;

	MAKE_VAR_AND_GETSETTER_REFERENCE(private, D3DCOLOR, _thickLineColor, ThickLineColor);
	MAKE_VAR_AND_GETSETTER_REFERENCE(private, D3DCOLOR, _thinLineColor, ThinLineColor);
private:
	D3DXMATRIX _matPosition;
	std::vector<VertexPosDiff> _vertices;
	int _thickLineInterval;
	Pyramid m_piramid[3];
	float _w, _h;
};

