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

	D3DXVECTOR3 poses[5] = { D3DXVECTOR3(-width / 2, 0, -height / 2) ,D3DXVECTOR3(width / 2, 0, -height / 2) ,D3DXVECTOR3(width / 2, 0, height / 2) ,
		D3DXVECTOR3(-width / 2, 0, height / 2),D3DXVECTOR3(0, depth, 0) };
	D3DXVECTOR3 normal[5] = { computeNormalFromVectors(poses[0], poses[4],poses[1]), computeNormalFromVectors(poses[1], poses[4],poses[2]), computeNormalFromVectors(poses[2], poses[4],poses[3]), computeNormalFromVectors(poses[3],poses[4],poses[0]),  computeNormalFromVectors(poses[0], poses[2], poses[1])};
	_vertices[0] = VertexPosNorm(poses[0], (normal[0] + normal[3] + normal[4]) / 3.0f);
	_vertices[1] = VertexPosNorm(poses[1], (normal[0] + normal[1] + normal[4]) / 3.0f);
	_vertices[2] = VertexPosNorm(poses[2], (normal[1] + normal[2] + normal[4]) / 3.0f);
	_vertices[3] = VertexPosNorm(poses[3], (normal[2] + normal[3] + normal[4]) / 3.0f);
	_vertices[4] = VertexPosNorm(poses[4], (normal[0] + normal[1] + normal[2] + normal[3]) / 4.0f);

	SGA::Materials::initMaterial(_material, color, color, color, SGA::Colors::BLACK, 5);

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
	g_pD3DDevice->SetMaterial(&_material);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &_matWorld);
	g_pD3DDevice->SetFVF(VertexPosNorm::FVF);
	g_pD3DDevice->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST, 0, 18, 6, _indices, D3DFMT_INDEX32, _vertices, sizeof(VertexPosNorm));
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
