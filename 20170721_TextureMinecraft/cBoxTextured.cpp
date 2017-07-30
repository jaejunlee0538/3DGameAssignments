#include "stdafx.h"
#include "cBoxTextured.h"


cBoxTextured::cBoxTextured()
{
	D3DXMatrixIdentity(&_matWorld);
}


cBoxTextured::~cBoxTextured()
{
}

void cBoxTextured::init(float xLen, float yLen, float zLen, D3DXVECTOR2 uvOffset, D3DXVECTOR3 uvLength)
{
	const static int indices[36] = { 0,1,2,0,2,3,4,6,5,4,7,6,4,5,1,4,1,0,3,2,6,3,6,7,1,5,6,1,6,2,4,0,3,4,3,7 };
	const float uvBegin[] = { uvLength.z, uvLength.z,uvLength.z * 2 + uvLength.x, uvLength.z,0, uvLength.z ,uvLength.z + uvLength.x, uvLength.z,uvLength.z, 0, uvLength.z + uvLength.x, 0 };
	const D3DXVECTOR2 uvStrides[] = {
		D3DXVECTOR2(uvLength.x, uvLength.y),
		D3DXVECTOR2(uvLength.x, uvLength.y),
		D3DXVECTOR2(0.5*uvLength.z, uvLength.y),
		D3DXVECTOR2(0.5*uvLength.z, uvLength.y),
		D3DXVECTOR2(uvLength.x, uvLength.z),
		D3DXVECTOR2(uvLength.x, uvLength.z)
	};
	D3DXVECTOR2 faceCoord[6] = {
		D3DXVECTOR2(0.5*uvLength.z, uvLength.z), //앞
		D3DXVECTOR2(0.5*uvLength.z*2+ uvLength.x, uvLength.z),//뒤
		D3DXVECTOR2(0, uvLength.z), //좌
		D3DXVECTOR2(0.5*uvLength.z+ uvLength.x, uvLength.z), //우
		D3DXVECTOR2(0.5*uvLength.z, 0), //상
		D3DXVECTOR2(0.5*uvLength.z + uvLength.x, 0) };//하
	float x = xLen / 2;
	float y = yLen / 2;
	float z = zLen / 2;

	D3DXVECTOR3 vertices[8] = { D3DXVECTOR3(-x,-y,-z),D3DXVECTOR3(-x,y,-z),D3DXVECTOR3(x,y,-z),D3DXVECTOR3(x,-y,-z),D3DXVECTOR3(-x,-y,z),D3DXVECTOR3(-x,y,z),D3DXVECTOR3(x,y,z),D3DXVECTOR3(x,-y,z) };

	int iFace;
	//전면, 후면,왼쪽 측면, 오른쪽 측면, 상단, 하단
	for (int iFace = 0; iFace < 6; ++iFace) {
		_vertices[iFace * 6 + 0].p = vertices[indices[iFace * 6 + 0]];
		_vertices[iFace * 6 + 1].p = vertices[indices[iFace * 6 + 1]];
		_vertices[iFace * 6 + 2].p = vertices[indices[iFace * 6 + 2]];
		_vertices[iFace * 6 + 3].p = vertices[indices[iFace * 6 + 3]];
		_vertices[iFace * 6 + 4].p = vertices[indices[iFace * 6 + 4]];
		_vertices[iFace * 6 + 5].p = vertices[indices[iFace * 6 + 5]];
		_vertices[iFace * 6 + 0].uv = D3DXVECTOR2(faceCoord[iFace].x, faceCoord[iFace].y + uvStrides[iFace].y) + uvOffset;
		_vertices[iFace * 6 + 1].uv = D3DXVECTOR2(faceCoord[iFace].x, faceCoord[iFace].y) + uvOffset;
		_vertices[iFace * 6 + 2].uv = D3DXVECTOR2(faceCoord[iFace].x + uvStrides[iFace].x, faceCoord[iFace].y) + uvOffset;
		_vertices[iFace * 6 + 3].uv = D3DXVECTOR2(faceCoord[iFace].x, faceCoord[iFace].y + uvStrides[iFace].y) + uvOffset;
		_vertices[iFace * 6 + 4].uv = D3DXVECTOR2(faceCoord[iFace].x + uvStrides[iFace].x, faceCoord[iFace].y) + uvOffset;
		_vertices[iFace * 6 + 5].uv = D3DXVECTOR2(faceCoord[iFace].x + uvStrides[iFace].x, faceCoord[iFace].y + uvStrides[iFace].y) + uvOffset;
	}
}

void cBoxTextured::FlipX()
{
}

void cBoxTextured::Render()
{
	g_pD3DDevice->SetFVF(tagVertexTextured::FVF);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &_matWorld);
	g_pD3DDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST, 12, _vertices, sizeof(tagVertexTextured));
}

void cBoxTextured::Update(SGA::UserInputData & input)
{
	if (input.hasInput(SGA::UP_KEY_STAYDOWN)) {
		TranslateLocal(0, 0, 0.1f);
	}
	if (input.hasInput(SGA::DOWN_KEY_STAYDOWN)) {
		TranslateLocal(0, 0, -0.1f);
	}
	if (input.hasInput(SGA::LEFT_KEY_STAYDOWN)) {
		Rotate(0, -0.05, 0);
	}
	if (input.hasInput(SGA::RIGHT_KEY_STAYDOWN)) {
		Rotate(0, 0.05, 0);
	}
}

void cBoxTextured::SetPosition(float x, float y, float z)
{
	_matWorld(3, 0) = x;
	_matWorld(3, 1) = y;
	_matWorld(3, 2) = z;
}

D3DXVECTOR3 cBoxTextured::GetPosition() const
{
	return D3DXVECTOR3(_matWorld(3, 0), _matWorld(3, 1), _matWorld(3, 2));
}

void cBoxTextured::Rotate(float angleX, float angleY, float angleZ)
{

	D3DXVECTOR3 pos(_matWorld(3, 0), _matWorld(3, 1), _matWorld(3, 2));
	D3DXMATRIX matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, angleY, angleX, angleZ);
	_matWorld = matRot * _matWorld;
}

void cBoxTextured::Translate(float x, float y, float z)
{
	_matWorld(3, 0) += x;
	_matWorld(3, 1) += y;
	_matWorld(3, 2) += z;
	D3DXMATRIXA16 m;
	D3DXMatrixScaling(&m, 1, 1, 1);
}

void cBoxTextured::TranslateLocal(float x, float y, float z)
{
	D3DXVECTOR3 moveDir(x, y, z);
	D3DXVec3TransformNormal(&moveDir, &moveDir, &_matWorld);
	Translate(moveDir.x, moveDir.y, moveDir.z);
}
