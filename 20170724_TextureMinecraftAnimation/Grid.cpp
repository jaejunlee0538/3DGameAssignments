#include "stdafx.h"
#include "Grid.h"

Grid::Grid()
	:_thickLineInterval(5)
{
	_w = _h = 0;
	D3DXMatrixIdentity(&_matPosition);
	_matPosition(3, 1) = 0.01f;
	_colorThick= D3DCOLOR_XRGB(0, 0, 0);
	_colorTine = D3DCOLOR_XRGB(80, 80, 80);

}


Grid::~Grid()
{
}

void Grid::init(int nX, int nZ, float sizeX, float sizeZ)
{
	if (nX & 0x01) nX += 1;
	if (nZ & 0x01) nZ += 1;
	assert(sizeX >= 0 && sizeZ >= 0 && "그리드의 크기는 양수만 허용됩니다.");
	
	float xLeft = -nX / 2 * sizeX;
	float zBottom = -nZ / 2 * sizeZ;
	for (int iX = -nX / 2; iX <= nX / 2; ++iX) {
		float x = iX * sizeX;
		D3DCOLOR lineColor;
		//if (iX != 0) {
			if (iX % _thickLineInterval == 0) {
				lineColor = _colorThick;
			}
			else {
				lineColor = _colorTine;
			}
		//}
		//else {
			//lineColor = D3DCOLOR_XRGB(0, 0, 255);
		//}
		_vertices.push_back(VertexPosDiff(x, 0, zBottom, lineColor));
		_vertices.push_back(VertexPosDiff(x, 0, -zBottom, lineColor));
		if (iX == 0) {
			_vertices.push_back(VertexPosDiff(x, 0, zBottom, D3DCOLOR_XRGB(0,0,255)));
			_vertices.push_back(VertexPosDiff(x, 0, -zBottom, D3DCOLOR_XRGB(0, 0, 255)));
		}
	}
	for (int iZ = -nZ / 2; iZ <= nZ / 2; ++iZ) {
		float z = iZ * sizeZ;
		D3DCOLOR lineColor;
		if (iZ != 0) {
			if (iZ % _thickLineInterval == 0) {
				lineColor = _colorThick;
			}
			else {
				lineColor = _colorTine;
			}
		}
		else {
			lineColor = D3DCOLOR_XRGB(255, 0, 0);
		}
		_vertices.push_back(VertexPosDiff(xLeft, 0, z, lineColor));
		_vertices.push_back(VertexPosDiff(-xLeft, 0, z, lineColor));
	}

	_vertices.push_back(VertexPosDiff(0, xLeft, 0, D3DCOLOR_XRGB(0, 255, 0)));
	_vertices.push_back(VertexPosDiff(0, -xLeft, 0, D3DCOLOR_XRGB(0, 255, 0)));

	_w = nX * sizeX;
	_h = nZ * sizeZ;
}

float Grid::GetWidth() const
{
	return _w;
}

float Grid::GetHeight() const
{
	return _h;
}

void Grid::Render() const
{
	//g_pD3DDevice->SetRenderState(D3DRS_COLOR)
	g_pD3DDevice->SetTexture(0, 0);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &_matPosition);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 
		_vertices.size() / 2,
		_vertices.data(), 
		sizeof(VertexPosDiff));
}

