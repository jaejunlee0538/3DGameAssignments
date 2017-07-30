#include "stdafx.h"
#include "cPyramid.h"


Pyramid::Pyramid()
{
}


Pyramid::~Pyramid()
{
}

void Pyramid::init(float width, float height, float depth, D3DCOLOR color)
{
	_vertices[0] = tagVertex(-width / 2, 0, -height / 2, color);
	_vertices[1] = tagVertex(width / 2, 0, -height / 2, color);
	_vertices[2] = tagVertex(width / 2, 0, height / 2, color);
	_vertices[3] = tagVertex(-width / 2, 0, height / 2, color);
	_vertices[4] = tagVertex(0, depth, 0, color);

	_indices[0] = 0;
	_indices[1] = 1;
	_indices[2] = 2;

	_indices[3] = 0;
	_indices[4] = 2;
	_indices[5] = 3;

	_indices[6] = 0;
	_indices[7] = 4;
	_indices[8] = 1;

	_indices[9] = 1;
	_indices[10] = 4;
	_indices[11] = 2;

	_indices[12] = 2;
	_indices[13] = 4;
	_indices[14] = 3;

	_indices[15] = 3;
	_indices[16] = 4;
	_indices[17] = 0;
}

void Pyramid::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &_matWorld);
	g_pD3DDevice->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST, 0, 18, 6, _indices, D3DFMT_INDEX32, _vertices, sizeof(tagVertex));
}

void Pyramid::Update(SGA::UserInputData & input)
{
}

void Pyramid::SetPosition(float x, float y, float z)
{
	_matWorld(3, 0) = x;
	_matWorld(3, 1) = y;
	_matWorld(3, 2) = z;
}

void Pyramid::SetAngle(float angleX, float angleY, float angleZ)
{
	D3DXVECTOR3 pos(_matWorld(3, 0), _matWorld(3, 1), _matWorld(3, 2));
	D3DXMatrixRotationYawPitchRoll(&_matWorld, angleY, angleX, angleZ);
	_matWorld(3, 0) = pos.x;
	_matWorld(3, 1) = pos.y;
	_matWorld(3, 2) = pos.z;
}
