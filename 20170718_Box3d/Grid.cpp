#include "Grid.h"
#include "Camera3D.h"
#include <assert.h>

Grid::Grid()
{
	_w = _h = 0;
}


Grid::~Grid()
{
}

void Grid::init(int nX, int nZ, float sizeX, float sizeZ)
{
	assert(nX > 0 && nZ > 0 && sizeX > 0 && sizeZ > 0 && "잘못된 매개변수 사용 - 양수만 입력 가능합니다.");
	_vertices.clear();
	_indices.clear();
	float z0 = -nZ * 0.5 * sizeZ;
	float x0 = -nX * 0.5 * sizeX;

	//X축 평행선
	for (int ix = 0; ix <= nX; ++ix) {
		_vertices.push_back(cVector3(x0, 0, z0));
		_vertices.push_back(cVector3(x0, 0, -z0));
		_indices.push_back(_indices.size());
		_indices.push_back(_indices.size());
		x0 += sizeX;
	}

	//Z축 평행선
	x0 = -nX * 0.5 * sizeX;
	for (int iz = 0; iz <= nZ; ++iz) {
		_vertices.push_back(cVector3(x0, 0, z0));
		_vertices.push_back(cVector3(-x0, 0, z0));
		_indices.push_back(_indices.size());
		_indices.push_back(_indices.size());
		z0 += sizeZ;
	}
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

void Grid::Render(Camera3D & camera) const
{
	std::vector<cVector4> vertices(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		vertices[i] = cVector4(_vertices[i], 1) * GetWorldMatrix();
	}
	camera.RenderLines(vertices.data(), _indices.data(), _indices.size() / 2);
}
