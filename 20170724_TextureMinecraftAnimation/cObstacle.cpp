#include "stdafx.h"
#include "cObstacle.h"

namespace SGA {
	int cObstacle::_count = 1;
	cObstacle::cObstacle()
		:_hp(3)
	{
		_id = _count++;
	}

	cObstacle::~cObstacle()
	{
	}

	void cObstacle::init(float xLen, float yLen, float zLen, D3DXVECTOR3 originPos, std::vector<D3DTexturePtr> textures)
	{
		const static int indices[36] = { 7,6,5,7,5,4/**/,0,1,2,0,2,3,/**/4,5,1,4,1,0/**/,3,2,6,3,6,7/**/,1,5,6,1,6,2/**/,4,0,3,4,3,7 };
		float x = xLen / 2;
		float y = yLen / 2;
		float z = zLen / 2;

		_textures = textures;
		_hp = 3;
		_currentTexture = _textures.back();
		D3DXVECTOR3 vertices[8] = { D3DXVECTOR3(-x,-y,-z),D3DXVECTOR3(-x,y,-z),D3DXVECTOR3(x,y,-z),D3DXVECTOR3(x,-y,-z),D3DXVECTOR3(-x,-y,z),D3DXVECTOR3(-x,y,z),D3DXVECTOR3(x,y,z),D3DXVECTOR3(x,-y,z) };
		D3DXVECTOR3 normals[6] = { D3DXVECTOR3(0,0,-1),D3DXVECTOR3(0,0,1) ,D3DXVECTOR3(-1,0,0) ,D3DXVECTOR3(1,0,0) ,D3DXVECTOR3(0,1,0) ,D3DXVECTOR3(0,-1,0) };
		for (int i = 0; i < 8; ++i) {
			vertices[i] -= originPos;
		}
		int iFace;
		//전면, 후면, 왼쪽 측면, 오른쪽 측면, 상단, 하단
		for (int iFace = 0; iFace < 6; ++iFace) {
			float top, left, bottom, right;
			left = 0;
			top = 0;
			right = 1;
			bottom = 1;

			_vertices[iFace * 6 + 0].p = vertices[indices[iFace * 6 + 0]];
			_vertices[iFace * 6 + 1].p = vertices[indices[iFace * 6 + 1]];
			_vertices[iFace * 6 + 2].p = vertices[indices[iFace * 6 + 2]];
			_vertices[iFace * 6 + 3].p = vertices[indices[iFace * 6 + 3]];
			_vertices[iFace * 6 + 4].p = vertices[indices[iFace * 6 + 4]];
			_vertices[iFace * 6 + 5].p = vertices[indices[iFace * 6 + 5]];
			_vertices[iFace * 6 + 0].uv = D3DXVECTOR2(left, bottom);
			_vertices[iFace * 6 + 1].uv = D3DXVECTOR2(left, top);
			_vertices[iFace * 6 + 2].uv = D3DXVECTOR2(right, top);
			_vertices[iFace * 6 + 3].uv = D3DXVECTOR2(left, bottom);
			_vertices[iFace * 6 + 4].uv = D3DXVECTOR2(right, top);
			_vertices[iFace * 6 + 5].uv = D3DXVECTOR2(right, bottom);
			_vertices[iFace * 6 + 0].n = normals[iFace];
			_vertices[iFace * 6 + 1].n = normals[iFace];
			_vertices[iFace * 6 + 2].n = normals[iFace];
			_vertices[iFace * 6 + 3].n = normals[iFace];
			_vertices[iFace * 6 + 4].n = normals[iFace];
			_vertices[iFace * 6 + 5].n = normals[iFace];
		}
		Materials::initMaterial(_material, Colors::WHITE, Colors::WHITE, Colors::WHITE, Colors::WHITE, 5);
		//Lights::initPointLight(_pointLight, Colors::WHITE*0.1f, Colors::WHITE, Colors::WHITE*0.2f, 10);
	}

	void cObstacle::update(bool isDirty)
	{
		if (getPosition().y > 0) {
			float dy = getPosition().y > 1.5? 1.5 : getPosition().y;
			translate(0, -dy, 0);
		}
		//_pointLight.Position = getPosition();
		//_pointLight.Position.y += 1.0f;
		GameObject::update(isDirty);
	}

	void cObstacle::render() const
	{
		if (_currentTexture) {
			//g_pD3DDevice->SetLight(_id, &_pointLight);
			g_pD3DDevice->LightEnable(_id, TRUE);
			g_pD3DDevice->SetMaterial(&_material);
			g_pD3DDevice->SetTexture(0, _currentTexture.get());

			g_pD3DDevice->SetFVF(VertexPosNormTex::FVF);
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &getWorldMatrix());

			g_pD3DDevice->DrawPrimitiveUP(
				D3DPT_TRIANGLELIST, 12, _vertices, sizeof(VertexPosNormTex));
		}
		GameObject::render();
	}

	void cObstacle::setCurrentHP(int hp)
	{
		_hp = hp;
		if (_hp <= 0) {
			_currentTexture.reset();
		}
		else if (_hp <= 3) {
			_currentTexture = _textures[_hp - 1];
		}
	}

	int cObstacle::getCurrentHP() const
	{
		return _hp;
	}
}