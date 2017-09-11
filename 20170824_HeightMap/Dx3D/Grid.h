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

	void render() const;

	SYNTHESIZE_PASS_BY_REF(D3DCOLOR, _thickLineColor, ThickLineColor);
	SYNTHESIZE_PASS_BY_REF(D3DCOLOR, _thinLineColor, ThinLineColor);
private:
	D3DXMATRIX _matPosition;
	std::vector<ST_PC_VERTEX> _vertices;
	int _thickLineInterval;
	Pyramid m_piramid[3];
	float _w, _h;
};

