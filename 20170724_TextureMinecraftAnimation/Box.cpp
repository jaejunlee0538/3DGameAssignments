#include "stdafx.h"
#include "Box.h"
cBox::cBox()
{
	D3DXMatrixIdentity(&_matWorld);
}

cBox::cBox(float width, float height, float depth)
{
	D3DXMatrixIdentity(&_matWorld);
	init(width, height, depth);
}

cBox::~cBox()
{
	
}

void cBox::init(float width, float height, float depth)
{

	const static int indices[36] = { 0,1,2,0,2,3,4,6,5,4,7,6,4,5,1,4,1,0,3,2,6,3,6,7,1,5,6,1,6,2,4,0,3,4,3,7 };
	float x = width / 2;
	float y = height / 2;
	float z = depth / 2;

	D3DXVECTOR3 vertices[8] = { D3DXVECTOR3(-x,-y,-z),D3DXVECTOR3(-x,y,-z),D3DXVECTOR3(x,y,-z),D3DXVECTOR3(x,-y,-z),D3DXVECTOR3(-x,-y,z),D3DXVECTOR3(-x,y,z),D3DXVECTOR3(x,y,z),D3DXVECTOR3(x,-y,z) };

	for (size_t iFace = 0; iFace < 6; ++iFace) {
		D3DCOLOR rgb = D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand()%256);
		_vertices[iFace * 6 + 0].p = vertices[indices[iFace * 6 + 0]];
		_vertices[iFace * 6 + 1].p = vertices[indices[iFace * 6 + 1]];
		_vertices[iFace * 6 + 2].p = vertices[indices[iFace * 6 + 2]];
		_vertices[iFace * 6 + 3].p = vertices[indices[iFace * 6 + 3]];
		_vertices[iFace * 6 + 4].p = vertices[indices[iFace * 6 + 4]];
		_vertices[iFace * 6 + 5].p = vertices[indices[iFace * 6 + 5]];

		_vertices[iFace * 6 + 0].color = rgb;
		_vertices[iFace * 6 + 1].color = rgb;
		_vertices[iFace * 6 + 2].color = rgb;
		_vertices[iFace * 6 + 3].color = rgb;
		_vertices[iFace * 6 + 4].color = rgb;
		_vertices[iFace * 6 + 5].color = rgb;
	}
}

void cBox::Render() 
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &_matWorld);
	g_pD3DDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST, 12, _vertices, sizeof(VertexPosDiff));
}

void cBox::Update(SGA::UserInputData & input)
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

void cBox::SetPosition(float x, float y, float z)
{
	_matWorld(3, 0) = x;
	_matWorld(3, 1) = y;
	_matWorld(3, 2) = z;
}

D3DXVECTOR3 cBox::GetPosition() const
{
	return D3DXVECTOR3(_matWorld(3, 0), _matWorld(3, 1), _matWorld(3, 2));
}

void cBox::Rotate(float angleX, float angleY, float angleZ)
{
	D3DXVECTOR3 pos(_matWorld(3, 0), _matWorld(3, 1), _matWorld(3, 2));
	D3DXMATRIX matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, angleY, angleX, angleZ);
	_matWorld = matRot * _matWorld;
}

void cBox::Translate(float x, float y, float z)
{
	/*_pos.x += x;
	_pos.y += y;
	_pos.z += z;*/
	_matWorld(3, 0) += x;
	_matWorld(3, 1) += y;
	_matWorld(3, 2) += z;
	D3DXMATRIXA16 m;
	D3DXMatrixScaling(&m, 1, 1, 1);
}

void cBox::TranslateLocal(float x, float y, float z)
{
	D3DXVECTOR3 moveDir(x, y, z);
	D3DXVec3TransformNormal(&moveDir, &moveDir, &_matWorld);
	Translate(moveDir.x, moveDir.y, moveDir.z);
}
